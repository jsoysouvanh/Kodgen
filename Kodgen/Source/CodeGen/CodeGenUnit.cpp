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

bool CodeGenUnit::generateCode(FileParsingResult const& parsingResult, CodeGenEnv& env) noexcept
{
	//Pre-generation step
	bool result = preGenerateCode(parsingResult, env);

	//Generation step: iterate over each module and entity and generate code
	result &= foreachEntity([](CodeGenModule const& codeGenModule, EntityInfo const& entity, CodeGenUnit& codeGenUnit, CodeGenEnv& env)
				{
					return codeGenUnit.runCodeGenModuleOnEntity(codeGenModule, entity, env);
				}, env) != ETraversalBehaviour::AbortWithFailure;

	//Run post-generation step only if the the pre-generation and generation steps succeeded
	return result ? postGenerateCode(parsingResult, env) : false;
}

bool CodeGenUnit::preGenerateCode(FileParsingResult const& /* parsingResult */, CodeGenEnv& env) noexcept
{
	if (_generationModules.get() != nullptr)
	{
		//Generate code for each module
		for (CodeGenModule const* codeGenModule : *_generationModules)
		{
			if (!codeGenModule->initialize(env))
			{
				return false;
			}
		}
	}

	return true;
}

bool CodeGenUnit::postGenerateCode(FileParsingResult const& /* parsingResult */, CodeGenEnv& /* env */) noexcept
{
	//Default implementation does nothing
	return true;
}

ETraversalBehaviour CodeGenUnit::foreachEntity(ETraversalBehaviour (*visitor)(CodeGenModule const&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
											   CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	ETraversalBehaviour result;

	if (_generationModules.get() != nullptr)
	{
		//Call visitor on all possible generation module/entity pair
		for (CodeGenModule const* codeGenModule : *_generationModules)
		{
			for (NamespaceInfo const& namespace_ : env.parsingResult->namespaces)
			{
				result = foreachEntityInNamespace(namespace_, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (StructClassInfo const& struct_ : env.parsingResult->structs)
			{
				result = foreachEntityInStruct(struct_, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (StructClassInfo const& class_ : env.parsingResult->classes)
			{
				result = foreachEntityInStruct(class_, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (EnumInfo const& enum_ : env.parsingResult->enums)
			{
				result = foreachEntityInEnum(enum_, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (VariableInfo const& variable : env.parsingResult->variables)
			{
				result = visitor(*codeGenModule, variable, *this, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (FunctionInfo const& function : env.parsingResult->functions)
			{
				result = visitor(*codeGenModule, function, *this, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}
		}
	}

	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachEntityInNamespace(NamespaceInfo const& namespace_,
														  ETraversalBehaviour (*visitor)(CodeGenModule const&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
														  CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);
	
	if (_generationModules.get() != nullptr)
	{
		//Iterate over all generation modules
		for (CodeGenModule const* codeGenModule : *_generationModules)
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
				result = foreachEntityInNamespace(nestedNamespace, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (StructClassInfo const& struct_ : namespace_.structs)
			{
				result = foreachEntityInStruct(struct_, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (StructClassInfo const& class_ : namespace_.classes)
			{
				result = foreachEntityInStruct(class_, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (EnumInfo const& enum_ : namespace_.enums)
			{
				result = foreachEntityInEnum(enum_, visitor, env);

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
	}

	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachEntityInStruct(StructClassInfo const& struct_,
													   ETraversalBehaviour (*visitor)(CodeGenModule const&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
													   CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	if (_generationModules.get() != nullptr)
	{
		//Iterate over all generation modules
		for (CodeGenModule const* codeGenModule : *_generationModules)
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
				result = foreachEntityInStruct(*nestedStruct, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (std::shared_ptr<NestedStructClassInfo> const& nestedClass : struct_.nestedClasses)
			{
				result = foreachEntityInStruct(*nestedClass, visitor, env);

				HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
			}

			for (NestedEnumInfo const& nestedEnum : struct_.nestedEnums)
			{
				result = foreachEntityInEnum(nestedEnum, visitor, env);

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
	}
	
	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachEntityInEnum(EnumInfo const& enum_,
													 ETraversalBehaviour (*visitor)(CodeGenModule const&, EntityInfo const&, CodeGenUnit&, CodeGenEnv&),
													 CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	if (_generationModules.get() != nullptr)
	{
		//Iterate over all generation modules
		for (CodeGenModule const* codeGenModule : *_generationModules)
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
	}
	
	return ETraversalBehaviour::Recurse;
}

bool CodeGenUnit::initializeGenerationModulesIfNecessary() noexcept
{
	//Shared_ptr is not initialized yet, so initialize it
	if (_generationModules.use_count() == 0)
	{
		_generationModules = std::make_shared<std::vector<CodeGenModule const*>>();

		return true;
	}

	return false;
}

void CodeGenUnit::addModule(CodeGenModule const& generationModule) noexcept
{
	initializeGenerationModulesIfNecessary();
	assert(_generationModules.get() != nullptr);

	//Add modules sorted by generation order
	_generationModules->insert
	(
		std::upper_bound(_generationModules->begin(), _generationModules->end(), &generationModule, [](CodeGenModule const* lhs, CodeGenModule const* rhs)
		{
			return lhs->getGenerationOrder() < rhs->getGenerationOrder();
		}),
		&generationModule
	);
}

bool CodeGenUnit::removeModule(CodeGenModule const& generationModule) noexcept
{
	//Don't bother looking for something to remove if the generation modules were not initialized
	if (_generationModules.get() != nullptr)
	{
		auto it = std::find(_generationModules->cbegin(), _generationModules->cend(), &generationModule);

		if (it != _generationModules->cend())
		{
			_generationModules->erase(it);

			return true;
		}
	}

	return false;
}

CodeGenUnitSettings const* CodeGenUnit::getSettings() const noexcept
{
	return settings;
}