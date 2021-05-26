/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template <template <typename> typename FileParserFactoryType, typename FileParserType, typename CodeGenUnitType>
void FileGenerator::processFilesMultithread(FileParserFactoryType<FileParserType>& fileParserFactory, CodeGenUnitType& codeGenUnit, std::set<fs::path> const& toProcessFiles, FileGenerationResult& out_genResult, uint32 threadCount) const noexcept
{
	ThreadPool								threadPool(threadCount, ETerminationMode::FinishAll);
	std::vector<std::shared_ptr<TaskBase>>	generationTasks;

	//Reserve enough space for all tasks
	generationTasks.reserve(toProcessFiles.size());

	//Launch all parsing -> generation processes
	for (fs::path const& file : toProcessFiles)
	{
		//Add file to the list of parsed files before starting the task to avoid having to synchronize threads
		out_genResult.parsedFiles.push_back(file);

		auto parsingTask = threadPool.submitTask([&](TaskBase*) -> FileParsingResult
												  {
													 //Generate a parser for this task
													 FileParserType		fileParser = fileParserFactory.generateFileParser();
													 FileParsingResult	parsingResult;

													 fileParser.parse(file, fileParserFactory.getCompilationArguments(), parsingResult);

													 return parsingResult;
												  });

		generationTasks.emplace_back(threadPool.submitTask([&](TaskBase* parsingTask) -> FileGenerationResult
									 {
										FileGenerationResult out_generationResult;
										
										//Copy the generation unit model to have a fresh one for this generation unit
										CodeGenUnitType	generationUnit = codeGenUnit;

										//Get the result of the parsing task
										FileParsingResult parsingResult = TaskHelper::getDependencyResult<FileParsingResult>(parsingTask, 0u);

										//Generate the file if no errors occured during parsing
										if (parsingResult.errors.empty())
										{
											out_generationResult.completed = generationUnit.generateCode(parsingResult);
										}
										
										return out_generationResult;
									 
									 }, { parsingTask }));
	}

	//Merge all generation results together
	for (std::shared_ptr<TaskBase>& generationTask : generationTasks)
	{
		out_genResult.mergeResult(TaskHelper::getResult<FileGenerationResult>(generationTask.get()));
	}
}

template <template <typename> typename FileParserFactoryType, typename FileParserType, typename CodeGenUnitType>
void FileGenerator::processFilesMonothread(FileParserFactoryType<FileParserType>& fileParserFactory, CodeGenUnitType& codeGenUnit, std::set<fs::path> const& toProcessFiles, FileGenerationResult& out_genResult) const noexcept
{
	FileParserType fileParser = fileParserFactory.generateFileParser();

	for (fs::path const& file : toProcessFiles)
	{
		FileParsingResult parsingResult;

		out_genResult.parsedFiles.push_back(file);

		//Parse file
		if (fileParser.parse(file, fileParserFactory.getCompilationArguments(), parsingResult))
		{
			//Generate file according to parsing result
			out_genResult.completed &= codeGenUnit.generateCode(parsingResult);
		}
	}
}

template <template <typename> typename FileParserFactoryType, typename FileParserType, typename CodeGenUnitType>
FileGenerationResult FileGenerator::generateFiles(FileParserFactoryType<FileParserType>& fileParserFactory, CodeGenUnitType& codeGenUnit, bool forceRegenerateAll, uint32 threadCount) noexcept
{
	//Check FileParserFactory validity
	static_assert(std::is_base_of_v<FileParserFactoryBase, FileParserFactoryType<FileParserType>>, "fileParserFactory type must be a derived class of kodgen::FileParserFactory");

	//Check FileGenerationUnit validity
	static_assert(std::is_base_of_v<CodeGenUnit, CodeGenUnitType>, "codeGenUnit type must be a derived class of kodgen::CodeGenUnit.");
	static_assert(std::is_copy_constructible_v<CodeGenUnitType>, "The CodeGenUnit you provide must be copy-constructible.");

	FileGenerationResult genResult;
	genResult.completed = true;

	if (!checkGenerationSetup(codeGenUnit))
	{
		genResult.completed = false;
	}
	else
	{
		//Start timer here
		auto				start			= std::chrono::high_resolution_clock::now();
		std::set<fs::path>	filesToProcess	= identifyFilesToProcess(codeGenUnit, genResult, forceRegenerateAll);

		//Don't setup anything if there are no files to generate
		if (filesToProcess.size() > 0u)
		{
			//Initialize the file parser factory compilation arguments
			fileParserFactory._init();

			generateMacrosFile(fileParserFactory, codeGenUnit);

			threadCount = getThreadCount(threadCount);

			//At this point thread count can't be 0
			assert(threadCount > 0);

			if (threadCount == 1u)
			{
				processFilesMonothread(fileParserFactory, codeGenUnit, filesToProcess, genResult);
			}
			else
			{
				processFilesMultithread(fileParserFactory, codeGenUnit, filesToProcess, genResult, threadCount);
			}
		}

		genResult.duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0f;
	}
	
	return genResult;
}