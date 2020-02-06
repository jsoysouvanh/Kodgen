#include "Parsing/Parser.h"

#include <iostream>

#include "Misc/Helpers.h"
#include "InfoStructures/ParsingInfo.h"

using namespace refureku;

Parser::Parser() noexcept:
	_clangIndex{clang_createIndex(0, 0)}
{
}

Parser::~Parser() noexcept
{
	clang_disposeIndex(_clangIndex);
}

CXChildVisitResult Parser::staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData) noexcept
{
	Parser*	parser = reinterpret_cast<Parser*>(clientData);

	//Parse the given file ONLY, ignore headers
	if (clang_Location_isFromMainFile(clang_getCursorLocation (c)) || clang_getCursorKind(c) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		parser->updateParsingState(parent);

		return parser->parseCursor(c);
	}
	
	return CXChildVisitResult::CXChildVisit_Continue;
}

void Parser::updateParsingState(CXCursor parent) noexcept
{
	if (_classParser.getParsingLevel())
	{
		_classParser.updateParsingState(parent, _parsingInfo);
	}
	else if (_enumParser.getParsingLevel())
	{
		_enumParser.updateParsingState(parent, _parsingInfo);
	}
}

CXChildVisitResult Parser::parseCursor(CXCursor currentCursor) noexcept
{
	if (_classParser.getParsingLevel())			//Currently parsing a class of struct
	{
		return _classParser.parse(currentCursor, _parsingInfo);
	}
	else if (_enumParser.getParsingLevel())		//Currently parsing an enum
	{	
		return _enumParser.parse(currentCursor, _parsingInfo);
	}
	else										//Looking for something to parse
	{
		return parseDefault(currentCursor);
	}
}

CXChildVisitResult Parser::parseDefault(CXCursor currentCursor) noexcept
{
	CXCursorKind cursorKind	= clang_getCursorKind(currentCursor);

	//Check for namespace, class or enum
	switch (cursorKind)
	{
		case CXCursorKind::CXCursor_Namespace:
			//TODO
			break;

		case CXCursorKind::CXCursor_ClassDecl:
			_classParser.startClassParsing(currentCursor, _parsingInfo);
			break;

		case CXCursorKind::CXCursor_StructDecl:
			_classParser.startStructParsing(currentCursor, _parsingInfo);
			break;

		case CXCursorKind::CXCursor_EnumDecl:
			_enumParser.startParsing(currentCursor);
			break;

		default:
			return CXChildVisitResult::CXChildVisit_Continue; 
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

bool Parser::parse(fs::path const& parseFile, ParsingResult& out_result) noexcept
{
	bool isSuccess = false;

	reset();

	preParse(parseFile);

	if (fs::exists(parseFile) && !fs::is_directory(parseFile))
	{
		//Parse the given file
		CXTranslationUnit translationUnit = clang_parseTranslationUnit(_clangIndex, parseFile.string().c_str(), _parseArguments, sizeof(_parseArguments) / sizeof(char const*), nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete);

		if (translationUnit != nullptr)
		{
			//Get the root cursor for this translation unit
			CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
			
			if (clang_visitChildren(cursor, &Parser::staticParseCursor, this) || _parsingInfo.hasErrorOccured())
			{
				//ERROR
			}
			else
			{
				_parsingInfo.flushCurrentStructOrClass();
				_parsingInfo.flushCurrentEnum();

				isSuccess = true;
			}

			#ifndef NDEBUG
			
			CXDiagnosticSet diagnostics = clang_getDiagnosticSetFromTU(translationUnit);

			std::cout << "DIAGNOSTICS START..." << std::endl;
			for (unsigned i = 0u; i < clang_getNumDiagnosticsInSet(diagnostics); i++)
			{
				CXDiagnostic diagnostic(clang_getDiagnosticInSet(diagnostics, i));
				std::cout << Helpers::getString(clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions())) << std::endl;
				clang_disposeDiagnostic(diagnostic);
			}
			std::cout << "DIAGNOSTICS END..." << std::endl;

			clang_disposeDiagnosticSet(diagnostics);

			#endif

			clang_disposeTranslationUnit(translationUnit);
		}
		else
		{
			_parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(EParsingError::TranslationUnitInitFailed));
		}
	}
	else
	{
		_parsingInfo.parsingResult.parsingErrors.emplace_back(ParsingError(EParsingError::InexistantFile));
	}

	out_result = std::move(_parsingInfo.parsingResult);

	postParse(parseFile, out_result);

	return isSuccess;
}

void Parser::preParse(fs::path const& parseFile) noexcept
{
	/**
	*	Default implementation does nothing special
	*/

	//Fake use to remove warning
	static_cast<void>(parseFile);
}

void Parser::postParse(fs::path const& parseFile, ParsingResult const& result) noexcept
{
	/**
	*	Default implementation does nothing special
	*/

	//Fake use to remove warning
	static_cast<void>(parseFile);
	static_cast<void>(result);
}

void Parser::reset() noexcept
{
	_classParser.reset();
	_enumParser.reset();

	_parsingInfo.reset();
}

ParsingSettings& Parser::getParsingSettings() noexcept
{
	return _parsingInfo.parsingSettings;
}