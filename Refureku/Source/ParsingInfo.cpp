#include "ParsingInfo.h"

#include <iostream>	//TO DELETE

using namespace refureku;

void ParsingInfo::startStructParsing(CXCursor const& structCursor)
{
	classStructLevel++;
	currentClassCursor = structCursor;
	shouldCheckValidity = true;
	accessSpecifier = AccessSpecifier::Public;

	std::cout << "START STRUCT" << std::endl;
}

void ParsingInfo::startClassParsing(CXCursor const& classCursor)
{
	classStructLevel++;
	currentClassCursor = classCursor;
	shouldCheckValidity = true;
	accessSpecifier = AccessSpecifier::Private;

	std::cout << "START CLASS" << std::endl;
}

void ParsingInfo::startFieldParsing(CXCursor const& fieldCursor)
{
	isParsingField = true;
	currentEnumFieldMethodCursor = fieldCursor;
	shouldCheckValidity = true;
	std::cout << "START FIELD" << std::endl;
}

void ParsingInfo::startMethodParsing(CXCursor const& methodCursor)
{
	isParsingMethod = true;
	currentEnumFieldMethodCursor = methodCursor;
	shouldCheckValidity = true;
	std::cout << "START METHOD" << std::endl;
}

void ParsingInfo::startEnumParsing(CXCursor const& enumCursor)
{
	isParsingEnum = true;
	currentEnumFieldMethodCursor = enumCursor;
	shouldCheckValidity = true;
	std::cout << "START ENUM" << std::endl;
}

void ParsingInfo::endStructOrClassParsing()
{
	classStructLevel--;
	currentClassCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END STRUCT" << std::endl;
}

void ParsingInfo::endFieldParsing()
{
	isParsingField = false;
	currentEnumFieldMethodCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END FIELD" << std::endl;
}

void ParsingInfo::endMethodParsing()
{
	isParsingMethod = false;
	currentEnumFieldMethodCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END METHOD" << std::endl;
}

void ParsingInfo::endEnumParsing()
{
	isParsingEnum = false;
	currentEnumFieldMethodCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	std::cout << "END ENUM" << std::endl;
}

void ParsingInfo::updateAccessSpecifier(CXCursor const& enumCursor)
{
	switch (clang_getCXXAccessSpecifier(enumCursor))
	{
		//case CX_CXXAccessSpecifier::
	}
}