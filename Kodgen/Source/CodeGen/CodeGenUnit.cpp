#include "Kodgen/CodeGen/CodeGenUnit.h"

#define HANDLE_NESTED_ENTITY_ITERATION_RESULT(result)																\
	if (result == ETraversalBehaviour::Break)																		\
	{																												\
		break;																										\
	}																												\
	else if (result == ETraversalBehaviour::AbortWithFailure || result == ETraversalBehaviour::AbortWithSuccess)	\
	{																												\
		return result;																								\
	}

using namespace kodgen;

CodeGenUnit::CodeGenUnit(CodeGenUnit const& other) noexcept:
	_isCopy{true},
	settings{other.settings},
	logger{other.logger}
{
	//Replace each module by a new clone of themself so that
	//each CodeGenUnit instance owns their own modules
	for (int i = 0; i < other._generationModules.size(); i++)
	{
		addModule(*static_cast<CodeGenModule*>(other._generationModules[i]->clone()));
	}
}

CodeGenUnit::~CodeGenUnit() noexcept
{
	clearGenerationModules();
}

bool CodeGenUnit::checkSettings() const noexcept
{
	bool result = true;
	bool canLog	= logger != nullptr;

	if (settings == nullptr)
	{
		if (canLog)
		{
			logger->log("CodeGenUnit settings have not been set.", ILogger::ELogSeverity::Error);
		}

		result &= false;
	}
	else
	{
		if (settings->getOutputDirectory().empty())
		{
			if (canLog)
			{
				logger->log("Output directory is empty, it must be specified for the files to be generated.", ILogger::ELogSeverity::Error);
			}

			result &= false;
		}
		else if (!fs::exists(settings->getOutputDirectory()))
		{
			//Before doing anything, make sure the output directory exists
			//If it doesn't, create it

			//Try to create them if it doesn't exist
			try
			{
				fs::create_directories(settings->getOutputDirectory());

				if (canLog)
				{
					logger->log("Specified output directory doesn't exist. Create " + FilesystemHelpers::sanitizePath(settings->getOutputDirectory()).string());
				}
			}
			catch (fs::filesystem_error const& exception)
			{
				if (canLog)
				{
					logger->log("Tried to create directory " + settings->getOutputDirectory().string() + " but failed: " + std::string(exception.what()), ILogger::ELogSeverity::Error);
				}

				//Directory failed to be created, so settings are incorrect
				result &= false;
			}
		}
	}

	return result;
}

bool CodeGenUnit::isFileNewerThan(fs::path const& file, fs::path const& referenceFile) const noexcept
{
	assert(fs::exists(file));
	assert(fs::is_regular_file(file));
	assert(fs::exists(referenceFile));
	assert(fs::is_regular_file(referenceFile));

	return fs::last_write_time(file) > fs::last_write_time(referenceFile);
}

bool CodeGenUnit::generateCode(FileParsingResult const& parsingResult) noexcept
{
	CodeGenEnv codeGenEnv;
	return generateCodeInternal(parsingResult, codeGenEnv);
}

bool CodeGenUnit::generateCodeInternal(FileParsingResult const&	parsingResult, CodeGenEnv& env) noexcept
{
	//Pre-generation step
	bool result = preGenerateCode(parsingResult, env);

	//Generation step (per module/entity pair), runs only if the pre-generation step succeeded
	if (result)
	{
		//Generation step: iterate over each module and entity and generate code
		result &= foreachModuleEntityPair([](CodeGenModule& codeGenModule, EntityInfo const& entity, CodeGenUnit& codeGenUnit, CodeGenEnv& env)
										  {
											  return codeGenUnit.runCodeGenModuleOnEntity(codeGenModule, entity, env);
										  }, env) != ETraversalBehaviour::AbortWithFailure;
	}

	//Run post-generation step only if the the pre-generation and generation steps succeeded
	return result ? postGenerateCode(env) : false;
}

bool CodeGenUnit::preGenerateCode(FileParsingResult const& parsingResult, CodeGenEnv& env) noexcept
{
	//Setup generation environment
	env._fileParsingResult	= &parsingResult;
	env._logger				= logger;

	//Initialize each module
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		if (!codeGenModule->initialize(env))
		{
			return false;
		}
	}

	return true;
}

bool CodeGenUnit::postGenerateCode(CodeGenEnv& /* env */) noexcept
{
	//Default implementation does nothing
	return true;
}

ETraversalBehaviour CodeGenUnit::foreachModuleEntityPair(ETraversalBehaviour (*visitor)(CodeGenModule&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
											   CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	ETraversalBehaviour result;

	//Call visitor on all possible generation module/entity pair
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		for (NamespaceInfo const& namespace_ : env.getFileParsingResult()->namespaces)
		{
			result = foreachModuleEntityPairInNamespace(namespace_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (StructClassInfo const& struct_ : env.getFileParsingResult()->structs)
		{
			result = foreachModuleEntityPairInStruct(struct_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (StructClassInfo const& class_ : env.getFileParsingResult()->classes)
		{
			result = foreachModuleEntityPairInStruct(class_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (EnumInfo const& enum_ : env.getFileParsingResult()->enums)
		{
			result = foreachModuleEntityPairInEnum(enum_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (VariableInfo const& variable : env.getFileParsingResult()->variables)
		{
			result = visitor(*codeGenModule, variable, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (FunctionInfo const& function : env.getFileParsingResult()->functions)
		{
			result = visitor(*codeGenModule, function, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}
	}

	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachModuleEntityPairInNamespace(NamespaceInfo const& namespace_,
														  ETraversalBehaviour (*visitor)(CodeGenModule&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
														  CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);
	
	//Iterate over all generation modules
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		//Execute the visitor function on the current namespace
		ETraversalBehaviour result = visitor(*codeGenModule, namespace_, *this, env);

		if (result != ETraversalBehaviour::Recurse)
		{
			return result;
		}

		//Iterate and execute the provided visitor function recursively on all nested entities
		for (NamespaceInfo const& nestedNamespace : namespace_.namespaces)
		{
			result = foreachModuleEntityPairInNamespace(nestedNamespace, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (StructClassInfo const& struct_ : namespace_.structs)
		{
			result = foreachModuleEntityPairInStruct(struct_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (StructClassInfo const& class_ : namespace_.classes)
		{
			result = foreachModuleEntityPairInStruct(class_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (EnumInfo const& enum_ : namespace_.enums)
		{
			result = foreachModuleEntityPairInEnum(enum_, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (VariableInfo const& variable : namespace_.variables)
		{
			result = visitor(*codeGenModule, variable, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (FunctionInfo const& function : namespace_.functions)
		{
			result = visitor(*codeGenModule, function, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}
	}

	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachModuleEntityPairInStruct(StructClassInfo const& struct_,
													   ETraversalBehaviour (*visitor)(CodeGenModule&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
													   CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	//Iterate over all generation modules
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		//Execute the visitor function on the current struct/class
		ETraversalBehaviour result = visitor(*codeGenModule, struct_, *this, env);

		if (result != ETraversalBehaviour::Recurse)
		{
			return result;
		}

		//Iterate and execute the provided visitor function recursively on all nested entities
		for (std::shared_ptr<NestedStructClassInfo> const& nestedStruct : struct_.nestedStructs)
		{
			result = foreachModuleEntityPairInStruct(*nestedStruct, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (std::shared_ptr<NestedStructClassInfo> const& nestedClass : struct_.nestedClasses)
		{
			result = foreachModuleEntityPairInStruct(*nestedClass, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (NestedEnumInfo const& nestedEnum : struct_.nestedEnums)
		{
			result = foreachModuleEntityPairInEnum(nestedEnum, visitor, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (FieldInfo const& field : struct_.fields)
		{
			result = visitor(*codeGenModule, field, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (MethodInfo const& method : struct_.methods)
		{
			result = visitor(*codeGenModule, method, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}
	}
	
	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachModuleEntityPairInEnum(EnumInfo const& enum_,
													 ETraversalBehaviour (*visitor)(CodeGenModule&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
													 CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	//Iterate over all generation modules
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		//Execute the visitor function on the current enum
		ETraversalBehaviour result = visitor(*codeGenModule, enum_, *this, env);

		if (result != ETraversalBehaviour::Recurse)
		{
			return result;
		}

		//Iterate and execute the provided visitor function recursively on all nested entities
		for (EnumValueInfo const& enumValue : enum_.enumValues)
		{
			result = visitor(*codeGenModule, enumValue, *this, env);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}
	}
	
	return ETraversalBehaviour::Recurse;
}

void CodeGenUnit::clearGenerationModules() noexcept
{
	if (_isCopy)
	{
		//Free copied modules
		for (CodeGenModule* codeGenModule : _generationModules)
		{
			delete codeGenModule;
		}
	}

	_generationModules.clear();
}

void CodeGenUnit::addModule(CodeGenModule& generationModule) noexcept
{
	//Add modules sorted by generation order
	_generationModules.insert
	(
		std::upper_bound(_generationModules.begin(), _generationModules.end(), &generationModule, [](CodeGenModule const* lhs, CodeGenModule const* rhs)
		{
			return lhs->getGenerationOrder() < rhs->getGenerationOrder();
		}),
		&generationModule
	);
}

bool CodeGenUnit::removeModule(CodeGenModule const& generationModule) noexcept
{
	auto it = std::find(_generationModules.cbegin(), _generationModules.cend(), &generationModule);

	if (it != _generationModules.cend())
	{
		_generationModules.erase(it);

		return true;
	}

	return false;
}

CodeGenUnitSettings const* CodeGenUnit::getSettings() const noexcept
{
	return settings;
}

CodeGenUnit& CodeGenUnit::operator=(CodeGenUnit const& other) noexcept
{
	settings = other.settings;
	logger = other.logger;

	//Correctly release memory if the instance is already a copy
	if (_isCopy)
	{
		clearGenerationModules();
	}
	else
	{
		_isCopy = true;
	}

	//At this point, there should be no registered module
	assert(_generationModules.empty());

	//Replace each module by a new clone of themself so that
	//each CodeGenUnit instance owns their own modules
	for (int i = 0; i < other._generationModules.size(); i++)
	{
		addModule(*static_cast<CodeGenModule*>(other._generationModules[i]->clone()));
	}

	return *this;
}