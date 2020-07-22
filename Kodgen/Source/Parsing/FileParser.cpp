#include "Parsing/FileParser.h"

#include <iostream>
#include <cassert>

#include "Misc/Helpers.h"
#include "Misc/DisableWarningMacros.h"
#include "Misc/TomlUtility.h"
#include "InfoStructures/ParsingInfo.h"

using namespace kodgen;

//FileParser::FileParser() noexcept:
//	_clangIndex{clang_createIndex(0, 0)}
//{
//	//Propagate the parsing info to child parsers
//	_classParser.setParsingInfo(&_parsingInfo);
//	_enumParser.setParsingInfo(&_parsingInfo);
//}
//
//FileParser::~FileParser() noexcept
//{
//	clang_disposeIndex(_clangIndex);
//}
//
//DISABLE_WARNING_PUSH
//DISABLE_WARNING_UNSCOPED_ENUM
//
//CXChildVisitResult FileParser::staticParseCursor(CXCursor c, CXCursor /* parent */, CXClientData clientData) noexcept
//{
//	FileParser*	parser = reinterpret_cast<FileParser*>(clientData);
//
//	//Parse the given file ONLY, ignore headers
//	if (clang_Location_isFromMainFile(clang_getCursorLocation (c)))
//	{
//		return parser->parseCursor(c);
//	}
//	
//	return CXChildVisitResult::CXChildVisit_Continue;
//}
//
//DISABLE_WARNING_POP
//
//CXChildVisitResult FileParser::parseCursor(CXCursor currentCursor) noexcept
//{
//	//Check for namespace, class or enum
//	switch (currentCursor.kind)
//	{
//		case CXCursorKind::CXCursor_Namespace:
//			return CXChildVisitResult::CXChildVisit_Recurse;
//			//return parseNamespace(currentCursor);
//
//		case CXCursorKind::CXCursor_ClassDecl:
//			return parseClass(currentCursor, false);
//
//		case CXCursorKind::CXCursor_StructDecl:
//			return parseClass(currentCursor, true);
//
//		case CXCursorKind::CXCursor_EnumDecl:
//			return parseEnum(currentCursor);
//
//		default:
//			break;
//	}
//
//	return CXChildVisitResult::CXChildVisit_Continue;
//}
//
//CXChildVisitResult FileParser::parseNamespace(CXCursor namespaceCursor) noexcept
//{
//	assert(namespaceCursor.kind == CXCursorKind::CXCursor_Namespace);
//
//	_namespaceParser.startParsing(namespaceCursor);
//
//	clang_visitChildren(namespaceCursor, [](CXCursor c, CXCursor, CXClientData clientData)
//						{
//							return reinterpret_cast<NamespaceParser*>(clientData)->parse(c);
//
//						}, &_namespaceParser);
//
//	return _namespaceParser.endParsing();
//}
//
//CXChildVisitResult FileParser::parseClass(CXCursor classCursor, bool isStruct) noexcept
//{
//	if (isStruct)
//	{
//		assert(classCursor.kind == CXCursorKind::CXCursor_StructDecl);
//
//		_classParser.startStructParsing(classCursor);
//	}
//	else
//	{
//		assert(classCursor.kind == CXCursorKind::CXCursor_ClassDecl);
//
//		_classParser.startClassParsing(classCursor);
//	}
//
//	clang_visitChildren(classCursor, [](CXCursor c, CXCursor, CXClientData clientData)
//						{
//							return reinterpret_cast<ClassParser*>(clientData)->parse(c);
//
//						}, &_classParser);
//
//	return _classParser.endParsing();
//}
//
//CXChildVisitResult FileParser::parseEnum(CXCursor enumCursor) noexcept
//{
//	assert(enumCursor.kind == CXCursorKind::CXCursor_EnumDecl);
//
//	_enumParser.startParsing(enumCursor);
//
//	clang_visitChildren(enumCursor, [](CXCursor c, CXCursor, CXClientData clientData)
//						{
//							return reinterpret_cast<EnumParser*>(clientData)->parse(c);
//
//						}, &_enumParser);
//
//	return _enumParser.endParsing();
//}
//
//void FileParser::refreshBuildCommandStrings() noexcept
//{
//	ParsingSettings const& ps = _parsingInfo.parsingSettings;
//
//	_classPropertyMacro		= "-D" + ps.propertyParsingSettings.classPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGC:\"#__VA_ARGS__)))";
//	_structPropertyMacro	= "-D" + ps.propertyParsingSettings.structPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGS:\"#__VA_ARGS__)))";
//	_fieldPropertyMacro		= "-D" + ps.propertyParsingSettings.fieldPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGF:\"#__VA_ARGS__)))";
//	_methodPropertyMacro	= "-D" + ps.propertyParsingSettings.methodPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGM:\"#__VA_ARGS__)))";
//	_enumPropertyMacro		= "-D" + ps.propertyParsingSettings.enumPropertyRules.macroName			+ "(...)=__attribute__((annotate(\"KGE:\"#__VA_ARGS__)))";
//	_enumValuePropertyMacro	= "-D" + ps.propertyParsingSettings.enumValuePropertyRules.macroName	+ "(...)=__attribute__((annotate(\"KGEV:\"#__VA_ARGS__)))";
//
//	_projectIncludeDirs.clear();
//	_projectIncludeDirs.reserve(ps.projectIncludeDirectories.size());
//
//	for (fs::path const& includeDir : ps.projectIncludeDirectories)
//	{
//		_projectIncludeDirs.emplace_back("-I" + includeDir.string());
//	}
//}
//
//std::vector<char const*> FileParser::makeParseArguments() noexcept
//{
//	std::vector<char const*>	result;
//
//	refreshBuildCommandStrings();
//
//	/**
//	*	3 to include -xc++, -sed=c++1z & _parsingMacro
//	*
//	*	6 because we make an additional parameter per possible entity
//	*	Class, Struct, Field, Method, Enum, EnumValue
//	*/
//	result.reserve(3u + 6u + _projectIncludeDirs.size());
//
//	//Parsing C++
//	result.emplace_back("-xc++");
//
//	//Use C++17
//	result.emplace_back("-std=c++1z"); 
//
//	//Macro set when we are parsing with Kodgen
//	result.emplace_back(_kodgenParsingMacro.data());
//
//	result.emplace_back(_classPropertyMacro.data());
//	result.emplace_back(_structPropertyMacro.data());
//	result.emplace_back(_fieldPropertyMacro.data());
//	result.emplace_back(_methodPropertyMacro.data());
//	result.emplace_back(_enumPropertyMacro.data());
//	result.emplace_back(_enumValuePropertyMacro.data());
//	
//	for (std::string const& includeDir : _projectIncludeDirs)
//	{
//		result.emplace_back(includeDir.data());
//	}
//
//	return result;
//}
//
//bool FileParser::parse(fs::path const& parseFile, FileParsingResult& out_result) noexcept
//{
//	bool isSuccess = false;
//
//	reset();
//
//	preParse(parseFile);
//
//	if (fs::exists(parseFile) && !fs::is_directory(parseFile))
//	{
//		std::vector<char const*> const parseArguments = makeParseArguments();
//
//		#if KODGEN_DEV
//
//		if (_logger != nullptr)
//		{
//			for (char const* arg : parseArguments)
//			{
//				_logger->log(std::string(arg) + " ", ILogger::ELogSeverity::Info);
//			}
//		}
//
//		#endif
//
//		//Parse the given file
//		CXTranslationUnit translationUnit = clang_parseTranslationUnit(_clangIndex, parseFile.string().c_str(), parseArguments.data(), static_cast<int32>(parseArguments.size()), nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete | CXTranslationUnit_KeepGoing);
//
//		if (translationUnit != nullptr)
//		{
//			//Get the root cursor for this translation unit
//			CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
//			
//			if (clang_visitChildren(cursor, &FileParser::staticParseCursor, this) || _parsingInfo.hasErrorOccured())
//			{
//				//ERROR
//			}
//			else
//			{
//				isSuccess = true;
//			}
//			
//			#if KODGEN_DEV
//			
//			if (_logger != nullptr)
//			{
//				CXDiagnosticSet diagnostics = clang_getDiagnosticSetFromTU(translationUnit);
//
//				_logger->log("DIAGNOSTICS START...", ILogger::ELogSeverity::Info);
//
//				for (unsigned i = 0u; i < clang_getNumDiagnosticsInSet(diagnostics); i++)
//				{
//					CXDiagnostic diagnostic(clang_getDiagnosticInSet(diagnostics, i));
//
//					_logger->log(Helpers::getString(clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions())), ILogger::ELogSeverity::Warning);
//
//					clang_disposeDiagnostic(diagnostic);
//				}
//
//				_logger->log("DIAGNOSTICS END...", ILogger::ELogSeverity::Info);
//
//				clang_disposeDiagnosticSet(diagnostics);
//			}
//
//			#endif
//
//			clang_disposeTranslationUnit(translationUnit);
//		}
//		else
//		{
//			_parsingInfo.parsingResult.errors.emplace_back(ParsingError(EParsingError::TranslationUnitInitFailed));
//		}
//	}
//	else
//	{
//		_parsingInfo.parsingResult.errors.emplace_back(ParsingError(EParsingError::InexistantFile));
//	}
//
//	out_result = std::move(_parsingInfo.parsingResult);
//
//	postParse(parseFile, out_result);
//
//	return isSuccess;
//}
//
//void FileParser::preParse(fs::path const&) noexcept
//{
//	/**
//	*	Default implementation does nothing special
//	*/
//}
//
//void FileParser::postParse(fs::path const&, FileParsingResult const&) noexcept
//{
//	/**
//	*	Default implementation does nothing special
//	*/
//}
//
//void FileParser::reset() noexcept
//{
//	_classParser.reset();
//	_enumParser.reset();
//
//	_parsingInfo.reset();
//}
//
//std::string const& FileParser::getParsingMacro() noexcept
//{
//	return _parsingMacro;
//}
//
//ParsingSettings& FileParser::getParsingSettings() noexcept
//{
//	return _parsingInfo.parsingSettings;
//}
//
//bool FileParser::loadSettings(fs::path const& pathToSettingsFile) noexcept
//{
//	try
//	{
//		toml::value settings = toml::parse(pathToSettingsFile.string());
//
//		if (settings.contains("FileParserSettings"))
//		{
//			//Get the FileParserSettings table
//			toml::value const& parserSettings = toml::find(settings, "FileParserSettings");
//
//			//Update Parsing settings
//			TomlUtility::updateSetting(parserSettings, "shouldAbortParsingOnFirstError", _parsingInfo.parsingSettings.shouldAbortParsingOnFirstError);
//			TomlUtility::updateSetting(parserSettings, "projectIncludeDirectories", _parsingInfo.parsingSettings.projectIncludeDirectories);
//
//			//Update Property settings
//			if (parserSettings.contains("Properties"))
//			{
//				_parsingInfo.parsingSettings.propertyParsingSettings.loadSettings(toml::find(parserSettings, "Properties"));
//			}
//		}
//
//		return true;
//	}
//	catch (std::runtime_error const&)
//	{
//	}
//	catch (toml::syntax_error const& e)
//	{
//		if (_logger != nullptr)
//		{
//			_logger->log("Syntax error in settings file.\n" + std::string(e.what()), ILogger::ELogSeverity::Error);
//		}
//	}
//
//	return false;
//}
//
//void FileParser::provideLogger(ILogger& logger) noexcept
//{
//	_logger = &logger;
//}


//=========================================================================================

FileParser2::FileParser2() noexcept:
	_clangIndex{clang_createIndex(0, 0)}
{
}

FileParser2::~FileParser2() noexcept
{
	clang_disposeIndex(_clangIndex);
}

ParsingContext& FileParser2::pushContext(CXTranslationUnit const& translationUnit, FileParsingResult& out_result) noexcept
{
	_propertyParser.setup(parsingSettings.propertyParsingSettings);

	ParsingContext newContext;

	newContext.rootCursor		= clang_getTranslationUnitCursor(translationUnit);
	newContext.propertyParser	= &_propertyParser;
	newContext.parsingSettings	= &parsingSettings;
	newContext.parsingResult	= &out_result;

	contextsStack.push(std::move(newContext));

	return getContext();
}

bool FileParser2::parse(fs::path const& toParseFile, FileParsingResult& out_result) noexcept
{
	bool isSuccess = false;

	preParse(toParseFile);

	if (fs::exists(toParseFile) && !fs::is_directory(toParseFile))
	{
		std::vector<char const*> const compilationArguments = makeCompilationArguments();

		#if KODGEN_DEV

		logCompilationArguments();

		#endif

		//Parse the given file
		CXTranslationUnit translationUnit = clang_parseTranslationUnit(_clangIndex, toParseFile.string().c_str(), compilationArguments.data(), static_cast<int32>(compilationArguments.size()), nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete | CXTranslationUnit_KeepGoing);

		if (translationUnit != nullptr)
		{
			ParsingContext& context = pushContext(translationUnit, out_result);

			if (clang_visitChildren(context.rootCursor, &FileParser2::parseEntity, this) || !out_result.errors.empty())
			{
				//ERROR
			}
			else
			{
				isSuccess = true;
			}

			#if KODGEN_DEV

			logDiagnostic(translationUnit);

			#endif

			popContext();

			//There should not have any context left once parsing has finished
			assert(contextsStack.empty());

			clang_disposeTranslationUnit(translationUnit);
		}
		else
		{
			out_result.errors.emplace_back(ParsingError(EParsingError::TranslationUnitInitFailed));
		}
	}
	else
	{
		out_result.errors.emplace_back(ParsingError(EParsingError::InexistantFile));
	}

	postParse(toParseFile, out_result);

	return isSuccess;
}

CXChildVisitResult FileParser2::parseEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	FileParser2* parser = reinterpret_cast<FileParser2*>(clientData);

	//Parse the given file ONLY, ignore headers
	if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
	{
		switch (cursor.kind)
		{
			case CXCursorKind::CXCursor_Namespace:
				return parser->parseNamespace(cursor);

			case CXCursorKind::CXCursor_StructDecl:
				[[fallthrough]];
			case CXCursorKind::CXCursor_ClassDecl:
				return parser->parseClass(cursor);

			case CXCursorKind::CXCursor_EnumDecl:
				return parser->parseEnum(cursor);
				break;

			//TODO: Handle global variables
			//TODO: Handle free functions

			default:
				break;
		}
	}

	return CXChildVisitResult::CXChildVisit_Continue;
}

CXChildVisitResult FileParser2::parseNamespace(CXCursor const& namespaceCursor) noexcept
{
	FileParsingResult* result = getParsingResult();

	NamespaceParsingResult	namespaceResult;
	CXChildVisitResult		childVisitResult = NamespaceParser2::parse(namespaceCursor, getContext(), namespaceResult);

	FileParsingResult* result2 = getParsingResult();

	addNamespaceResult(std::move(namespaceResult));

	return childVisitResult;
}

CXChildVisitResult FileParser2::parseClass(CXCursor const& classCursor) noexcept
{
	ClassParsingResult	classResult;

	CXChildVisitResult	childVisitResult = ClassParser2::parse(classCursor, getContext(), classResult);

	addClassResult(std::move(classResult));

	return childVisitResult;
}

CXChildVisitResult FileParser2::parseEnum(CXCursor const& enumCursor) noexcept
{
	//TODO

	return CXChildVisitResult::CXChildVisit_Continue;
}

void FileParser2::addNamespaceResult(NamespaceParsingResult&& result) noexcept
{
	if (result.parsedNamespace.has_value())
	{
		getParsingResult()->namespaces.emplace_back(std::move(result.parsedNamespace).value());
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		getContext().parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void FileParser2::addClassResult(ClassParsingResult&& result) noexcept
{
	if (result.parsedClass.has_value())
	{
		switch (result.parsedClass->entityType)
		{
			case EntityInfo::EType::Struct:
				getParsingResult()->structs.emplace_back(std::move(result.parsedClass).value());
				break;

			case EntityInfo::EType::Class:
				getParsingResult()->classes.emplace_back(std::move(result.parsedClass).value());
				break;

			default:
				assert(false);	//Should never reach this line
				break;
		}
	}

	//Append errors if any
	if (!result.errors.empty())
	{
		getContext().parsingResult->errors.insert(getParsingResult()->errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));
	}
}

void FileParser2::preParse(fs::path const&) noexcept
{
	/**
	*	Default implementation does nothing special
	*/
}

void FileParser2::postParse(fs::path const&, FileParsingResult const&) noexcept
{
	/**
	*	Default implementation does nothing special
	*/
}

void FileParser2::refreshBuildCommandStrings() noexcept
{
	_namespacePropertyMacro	= "-D" + parsingSettings.propertyParsingSettings.namespacePropertyRules.macroName	+ "(...)=__attribute__((annotate(\"KGN:\"#__VA_ARGS__)))";
	_classPropertyMacro		= "-D" + parsingSettings.propertyParsingSettings.classPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGC:\"#__VA_ARGS__)))";
	_structPropertyMacro	= "-D" + parsingSettings.propertyParsingSettings.structPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGS:\"#__VA_ARGS__)))";
	_fieldPropertyMacro		= "-D" + parsingSettings.propertyParsingSettings.fieldPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGF:\"#__VA_ARGS__)))";
	_methodPropertyMacro	= "-D" + parsingSettings.propertyParsingSettings.methodPropertyRules.macroName		+ "(...)=__attribute__((annotate(\"KGM:\"#__VA_ARGS__)))";
	_enumPropertyMacro		= "-D" + parsingSettings.propertyParsingSettings.enumPropertyRules.macroName			+ "(...)=__attribute__((annotate(\"KGE:\"#__VA_ARGS__)))";
	_enumValuePropertyMacro	= "-D" + parsingSettings.propertyParsingSettings.enumValuePropertyRules.macroName	+ "(...)=__attribute__((annotate(\"KGEV:\"#__VA_ARGS__)))";

	_projectIncludeDirs.clear();
	_projectIncludeDirs.reserve(parsingSettings.projectIncludeDirectories.size());

	for (fs::path const& includeDir : parsingSettings.projectIncludeDirectories)
	{
		_projectIncludeDirs.emplace_back("-I" + includeDir.string());
	}
}

std::vector<char const*> FileParser2::makeCompilationArguments() noexcept
{
	std::vector<char const*>	result;

	refreshBuildCommandStrings();

	/**
	*	3 to include -xc++, -std=c++1z & _kodgenParsingMacro
	*
	*	7 because we make an additional parameter per possible entity
	*	Namespace, Class, Struct, Field, Method, Enum, EnumValue
	*/
	result.reserve(3u + 7u + _projectIncludeDirs.size());

	//Parsing C++
	result.emplace_back("-xc++");

	//Use C++17
	result.emplace_back("-std=c++1z"); 

	//Macro set when we are parsing with Kodgen
	result.emplace_back(_kodgenParsingMacro.data());

	result.emplace_back(_classPropertyMacro.data());
	result.emplace_back(_structPropertyMacro.data());
	result.emplace_back(_fieldPropertyMacro.data());
	result.emplace_back(_methodPropertyMacro.data());
	result.emplace_back(_enumPropertyMacro.data());
	result.emplace_back(_enumValuePropertyMacro.data());

	for (std::string const& includeDir : _projectIncludeDirs)
	{
		result.emplace_back(includeDir.data());
	}

	return result;
}

void FileParser2::logDiagnostic(CXTranslationUnit const& translationUnit) const noexcept
{
	if (logger != nullptr)
	{
		CXDiagnosticSet diagnostics = clang_getDiagnosticSetFromTU(translationUnit);

		logger->log("DIAGNOSTICS START...", ILogger::ELogSeverity::Info);

		for (unsigned i = 0u; i < clang_getNumDiagnosticsInSet(diagnostics); i++)
		{
			CXDiagnostic diagnostic(clang_getDiagnosticInSet(diagnostics, i));

			logger->log(Helpers::getString(clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions())), ILogger::ELogSeverity::Warning);

			clang_disposeDiagnostic(diagnostic);
		}

		logger->log("DIAGNOSTICS END...", ILogger::ELogSeverity::Info);

		clang_disposeDiagnosticSet(diagnostics);
	}
}

void FileParser2::logCompilationArguments() noexcept
{
	if (logger != nullptr)
	{
		std::vector<char const*> const compilationArguments = makeCompilationArguments();

		for (char const* arg : compilationArguments)
		{
			logger->log(std::string(arg) + " ", ILogger::ELogSeverity::Info);
		}
	}
}

bool FileParser2::loadSettings(fs::path const& pathToSettingsFile) noexcept
{
	try
	{
		toml::value settings = toml::parse(pathToSettingsFile.string());

		if (settings.contains("FileParserSettings"))
		{
			//Get the FileParserSettings table
			toml::value const& parserSettings = toml::find(settings, "FileParserSettings");

			//Update Parsing settings
			TomlUtility::updateSetting(parserSettings, "shouldAbortParsingOnFirstError", parsingSettings.shouldAbortParsingOnFirstError);
			TomlUtility::updateSetting(parserSettings, "projectIncludeDirectories", parsingSettings.projectIncludeDirectories);

			//Update Property settings
			if (parserSettings.contains("Properties"))
			{
				parsingSettings.propertyParsingSettings.loadSettings(toml::find(parserSettings, "Properties"));
			}
		}

		return true;
	}
	catch (std::runtime_error const& /* exception */)
	{
	}
	catch (toml::syntax_error const& exception)
	{
		if (logger != nullptr)
		{
			logger->log("Syntax error in settings file.\n" + std::string(exception.what()), ILogger::ELogSeverity::Error);
		}
	}

	return false;
}