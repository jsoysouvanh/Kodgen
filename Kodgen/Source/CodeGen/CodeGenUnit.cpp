#include "Kodgen/CodeGen/CodeGenUnit.h"

#define HANDLE_NESTED_ENTITY_ITERATION_RESULT(result)														\
	if (result == EIterationResult::Break)																	\
	{																										\
		break;																								\
	}																										\
	else if (result == EIterationResult::AbortWithFailure || result == EIterationResult::AbortWithSuccess)	\
	{																										\
		return result;																						\
	}

using namespace kodgen;

bool CodeGenUnit::generateCode(FileParsingResult const& parsingResult) noexcept
{
	return	preGenerateCode(parsingResult) &&
		generateCodeInternal(parsingResult) &&
		postGenerateCode(parsingResult);
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

			//Try to create them is it doesn't exist
			try
			{
				fs::create_directories(settings->getOutputDirectory());

				if (canLog)
				{
					logger->log("Specified output directory doesn't exist. Create " + FilesystemHelpers::sanitizePath(settings->getOutputDirectory()).string(), ILogger::ELogSeverity::Info);
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

bool CodeGenUnit::preGenerateCode(FileParsingResult const& /* parsingResult */) noexcept
{
	//Default implementation does nothing
	return true;
}

bool CodeGenUnit::postGenerateCode(FileParsingResult const& /* parsingResult */) noexcept
{
	//Default implementation does nothing
	return true;
}

CodeGenUnit::EIterationResult CodeGenUnit::foreachEntity(EIterationResult (*visitor)(EntityInfo const&, CodeGenData&), CodeGenData& data) noexcept
{
	assert(visitor != nullptr);

	EIterationResult result;

	for (NamespaceInfo const& namespace_ : data.parsingResult->namespaces)
	{
		result = foreachEntityInNamespace(namespace_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (StructClassInfo const& struct_ : data.parsingResult->structs)
	{
		result = foreachEntityInStruct(struct_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (StructClassInfo const& class_ : data.parsingResult->classes)
	{
		result = foreachEntityInStruct(class_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (EnumInfo const& enum_ : data.parsingResult->enums)
	{
		result = foreachEntityInEnum(enum_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (VariableInfo const& variable : data.parsingResult->variables)
	{
		result = visitor(variable, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (FunctionInfo const& function : data.parsingResult->functions)
	{
		result = visitor(function, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	return EIterationResult::Recurse;
}

CodeGenUnit::EIterationResult CodeGenUnit::foreachEntityInNamespace(NamespaceInfo const& namespace_, EIterationResult (*visitor)(EntityInfo const&, CodeGenData&), CodeGenData& data) noexcept
{
	assert(visitor != nullptr);
	
	//Execute the visitor function on the current namespace
	EIterationResult result = visitor(namespace_, data);

	if (result != EIterationResult::Recurse)
	{
		return result;
	}

	//Iterate and execute the provided visitor function recursively on all nested entities
	for (NamespaceInfo const& nestedNamespace : namespace_.namespaces)
	{
		result = foreachEntityInNamespace(nestedNamespace, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (StructClassInfo const& struct_ : namespace_.structs)
	{
		result = foreachEntityInStruct(struct_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (StructClassInfo const& class_ : namespace_.classes)
	{
		result = foreachEntityInStruct(class_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (EnumInfo const& enum_ : namespace_.enums)
	{
		result = foreachEntityInEnum(enum_, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (VariableInfo const& variable : namespace_.variables)
	{
		result = visitor(variable, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (FunctionInfo const& function : namespace_.functions)
	{
		result = visitor(function, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	return EIterationResult::Recurse;
}

CodeGenUnit::EIterationResult CodeGenUnit::foreachEntityInStruct(StructClassInfo const& struct_, EIterationResult (*visitor)(EntityInfo const&, CodeGenData&), CodeGenData& data) noexcept
{
	assert(visitor != nullptr);

	//Execute the visitor function on the current struct/class
	EIterationResult result = visitor(struct_, data);

	if (result != EIterationResult::Recurse)
	{
		return result;
	}

	//Iterate and execute the provided visitor function recursively on all nested entities
	for (std::shared_ptr<NestedStructClassInfo> const& nestedStruct : struct_.nestedStructs)
	{
		result = foreachEntityInStruct(*nestedStruct, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (std::shared_ptr<NestedStructClassInfo> const& nestedClass : struct_.nestedClasses)
	{
		result = foreachEntityInStruct(*nestedClass, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (NestedEnumInfo const& nestedEnum : struct_.nestedEnums)
	{
		result = foreachEntityInEnum(nestedEnum, visitor, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (FieldInfo const& field : struct_.fields)
	{
		result = visitor(field, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	for (MethodInfo const& method : struct_.methods)
	{
		result = visitor(method, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	return EIterationResult::Recurse;
}

CodeGenUnit::EIterationResult CodeGenUnit::foreachEntityInEnum(EnumInfo const& enum_, EIterationResult (*visitor)(EntityInfo const&, CodeGenData&), CodeGenData& data) noexcept
{
	assert(visitor != nullptr);

	//Execute the visitor function on the current enum
	EIterationResult result = visitor(enum_, data);

	if (result != EIterationResult::Recurse)
	{
		return result;
	}
	
	//Iterate and execute the provided visitor function recursively on all nested entities
	for (EnumValueInfo const& enumValue : enum_.enumValues)
	{
		result = visitor(enumValue, data);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	return EIterationResult::Recurse;
}

CodeGenUnitSettings const* CodeGenUnit::getSettings() const noexcept
{
	return settings;
}