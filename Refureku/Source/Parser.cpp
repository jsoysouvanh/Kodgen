#include "Parser.h"

#include <iostream>

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

std::string Parser::getString(CXString&& clangString) noexcept
{
	std::string result = clang_getCString(clangString);
	clang_disposeString(clangString);

	return result;
}

CXChildVisitResult Parser::staticParseCursor(CXCursor c, CXCursor parent, CXClientData clientData) noexcept
{
	ParsingInfo* parsingInfo = reinterpret_cast<ParsingInfo*>(clientData);

	std::string cursorName = Parser::getString(clang_getCursorSpelling(c));
	std::string cursorKindAsString = Parser::getString(clang_getCursorKindSpelling(clang_getCursorKind(c)));

	CXCursorKind cursorKind = clang_getCursorKind(c);

	Parser::updateParsingState(parent, parsingInfo);

	//std::cout << "Parent is : " << Parser::getString(clang_getCursorKindSpelling(clang_getCursorKind(parent))) << std::endl;
	//std::cout << "Cursor kind : " << cursorKindAsString << " : " << cursorName << std::endl;
	
	return Parser::parseCursor(c, parent, parsingInfo);
	//return CXChildVisitResult::CXChildVisit_Recurse;
}

void Parser::updateParsingState(CXCursor parent, ParsingInfo* parsingInfo) noexcept
{
	bool checkClassLeave = false;

	if (parsingInfo->classStructLevel)
	{
		if (parsingInfo->isParsingField)
		{
			//Check if we're not parsing a field anymore
			if (!clang_equalCursors(parsingInfo->currentEnumFieldMethodCursor, parent))
			{
				parsingInfo->endFieldParsing();

				checkClassLeave = true;
			}

		}
		else if (parsingInfo->isParsingMethod)
		{
			//Check if we're not parsing a method anymore
			if (!clang_equalCursors(parsingInfo->currentEnumFieldMethodCursor, parent))
			{
				parsingInfo->endMethodParsing();

				checkClassLeave = true;
			}
		}

		if (checkClassLeave)
		{
			//Check if we left the "currently parsing" class by checking the parent
			if (!clang_equalCursors(parsingInfo->currentClassCursor, parent))
			{
				parsingInfo->endStructOrClassParsing();
			}
		}
	}
}

CXChildVisitResult Parser::parseCursor(CXCursor currentCursor, CXCursor parentCursor, ParsingInfo* parsingInfo) noexcept
{
	if (parsingInfo->classStructLevel)		//Currently parsing a class of struct
	{
		return parseClassContent(currentCursor, parsingInfo);
	}
	else if (parsingInfo->isParsingEnum)	//Currently parsing an enum
	{	
		return parseEnumContent(currentCursor, parsingInfo);
	}
	else									//Looking for something to parse
	{
		return parseDefault(currentCursor, parsingInfo);
	}
}

CXChildVisitResult Parser::parseDefault(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	CXCursorKind		cursorKind	= clang_getCursorKind(currentCursor);
	std::string			cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	//Check for namespace, class or enum
	switch (cursorKind)
	{
		case CXCursorKind::CXCursor_Namespace:
			//TODO
			break;

		case CXCursorKind::CXCursor_ClassDecl:
			parsingInfo->startClassParsing(currentCursor);
			break;

		case CXCursorKind::CXCursor_StructDecl:
			parsingInfo->startStructParsing(currentCursor);
			break;

		case CXCursorKind::CXCursor_EnumDecl:
			parsingInfo->startEnumParsing(currentCursor);
			break;

		default:
			CXChildVisitResult::CXChildVisit_Continue; 
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult Parser::parseClassContent(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	if (parsingInfo->isParsingField)
	{
		return parseField(currentCursor, parsingInfo);
	}
	else if (parsingInfo->isParsingMethod)
	{
		return parseMethod(currentCursor, parsingInfo);
	}
	else if (parsingInfo->shouldCheckValidity)	//Check for any annotation attribute if the flag is raised
	{
		if (isClassValid(currentCursor, parsingInfo))
		{
			//TODO create a class

			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else
		{
			if (parsingInfo->getPropertyParser().getParsingError() == PropertyParsingError::Count)
			{
				parsingInfo->endStructOrClassParsing();
				return CXChildVisitResult::CXChildVisit_Continue;
			}
			else	//Fatal parsing error occured
			{
				std::cout << "Fatal error occured" << std::endl;
				//TODO setupError
				CXSourceLocation location = clang_getCursorLocation(currentCursor);



				/*
				CINDEX_LINKAGE void clang_getExpansionLocation(CXSourceLocation location,
												CXFile *file,
												unsigned *line,
												unsigned *column,
												unsigned *offset);
									
				CINDEX_LINKAGE void clang_getPresumedLocation(CXSourceLocation location,
												CXString *filename,
												unsigned *line,
												unsigned *column);

				 CINDEX_LINKAGE void clang_getSpellingLocation(CXSourceLocation location,
												CXFile *file,
												unsigned *line,
												unsigned *column,
												unsigned *offset);
				
				CINDEX_LINKAGE CXSourceLocation clang_getRangeStart(CXSourceRange range);

				CINDEX_LINKAGE CXSourceLocation clang_getRangeEnd(CXSourceRange range);

				CINDEX_LINKAGE CXToken *clang_getToken(CXTranslationUnit TU,
				CXSourceLocation Location);
				CINDEX_LINKAGE CXString clang_getTokenSpelling(CXTranslationUnit, CXToken);
				*/
				
				return CXChildVisitResult::CXChildVisit_Break;
			}
		}
	}

	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	std::string		cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	//Check for class field or method
	switch (cursorKind)
	{
		case CXCursorKind::CXCursor_CXXAccessSpecifier:
			parsingInfo->updateAccessSpecifier(currentCursor);
			break;
		
		case CXCursorKind::CXCursor_FieldDecl:
			parsingInfo->startFieldParsing(currentCursor);
			break;

		case CXCursorKind::CXCursor_CXXMethod:
			parsingInfo->startMethodParsing(currentCursor);
			break;

		default:
			return CXChildVisitResult::CXChildVisit_Continue;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

bool Parser::isClassValid(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	std::string		cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	parsingInfo->shouldCheckValidity = false;

	if (cursorKind == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return parsingInfo->getPropertyParser().getClassProperties(Parser::getString(clang_getCursorSpelling(currentCursor))).has_value();
	}

	return (cursorKind == CXCursorKind::CXCursor_AnnotateAttr && true/* TODO If notation is valid for a class, add the class*/);
}

CXChildVisitResult Parser::parseField(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (parsingInfo->shouldCheckValidity)
	{
		if (isFieldValid(currentCursor, parsingInfo))
		{
			std::cout << "VALID FIELD" << std::endl;
			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
	}

	std::string cursorName = Parser::getString(clang_getCursorSpelling(currentCursor));
	std::string cursorKindAsString = Parser::getString(clang_getCursorKindSpelling(clang_getCursorKind(currentCursor)));
	std::cout << "Cursor kind : " << cursorKindAsString << " : " << cursorName << std::endl;

	return CXChildVisitResult::CXChildVisit_Recurse;
}

bool Parser::isFieldValid(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	//std::string		cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	parsingInfo->shouldCheckValidity = false;

	return (cursorKind == CXCursorKind::CXCursor_AnnotateAttr && true/* TODO If notation is valid for a field, add the field*/);
}

CXChildVisitResult Parser::parseMethod(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (parsingInfo->shouldCheckValidity)
	{
		if (isMethodValid(currentCursor, parsingInfo))
		{
			std::cout << "VALID METHOD" << std::endl;
			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
	}

	std::string cursorName = Parser::getString(clang_getCursorSpelling(currentCursor));
	std::string cursorKindAsString = Parser::getString(clang_getCursorKindSpelling(clang_getCursorKind(currentCursor)));
	std::cout << "Cursor kind : " << cursorKindAsString << " : " << cursorName << std::endl;

	return CXChildVisitResult::CXChildVisit_Recurse;
}

bool Parser::isMethodValid(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	//std::string		cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	parsingInfo->shouldCheckValidity = false;

	return (cursorKind == CXCursorKind::CXCursor_AnnotateAttr && true/* TODO If notation is valid for a method, add the method*/);
}

CXChildVisitResult Parser::parseEnumContent(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	std::string		cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	//Check for any annotation if the flag is raised
	if (parsingInfo->shouldCheckValidity)
	{
		if (isEnumValid(currentCursor, parsingInfo))
		{
			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

bool Parser::isEnumValid(CXCursor currentCursor, ParsingInfo* parsingInfo) noexcept
{
	CXCursorKind	cursorKind	= clang_getCursorKind(currentCursor);
	//std::string		cursorName	= Parser::getString(clang_getCursorSpelling(currentCursor));

	parsingInfo->shouldCheckValidity = false;

	return (cursorKind == CXCursorKind::CXCursor_AnnotateAttr && true/* TODO If notation is valid for an enum, add the enum*/);
}

bool Parser::parse(fs::path const& parseFile) noexcept
{
	bool isSuccess = false;

	if (fs::exists(parseFile) && !fs::is_directory(parseFile))
	{
		//Parse the given file
		CXTranslationUnit translationUnit = clang_parseTranslationUnit(_clangIndex, parseFile.string().c_str(), _parseArguments, sizeof(_parseArguments) / sizeof(char const*), nullptr, 0, CXTranslationUnit_SkipFunctionBodies);

		if (translationUnit != nullptr)
		{
			ParsingInfo parsingInfo;
			
			parsingInfo.setParsingSettings(&parsingSettings);

			//Get the root cursor for this translation unit
			CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
			
			if (clang_visitChildren(cursor, &Parser::staticParseCursor, &parsingInfo))
			{
				//ERROR
				std::cout << "Parse ended with errors" << std::endl;
			}
			else
			{
				//SUCCESS
			}

			isSuccess = true;
		}

		clang_disposeTranslationUnit(translationUnit);
	}

	return isSuccess;
}