#include "InfoStructures/TypeInfo.h"

#include <iostream>
#include <cassert>

#include "Helpers.h"

using namespace refureku;

TypeInfo::TypeInfo(CXType cursorType) noexcept
{
	assert(cursorType.kind != CXTypeKind::CXType_Invalid);

	initialize(Helpers::getString(clang_getTypeSpelling(cursorType)));
}

void TypeInfo::incrementParsingStep(EParsingStep& ps) noexcept
{
	ps = static_cast<EParsingStep>(static_cast<EParsingStepUnderlyingType>(ps) + 1);
}

void TypeInfo::initialize(std::string&& fullTypeName) noexcept
{
	fullName = std::forward<std::string>(fullTypeName);

	//Copy the full name for parsing
	std::string parsedType = fullName;

	EParsingStep parsingStep = EParsingStep::ConstQualifier;

	while (!parsedType.empty())
	{
		switch (parsingStep)
		{
			case EParsingStep::ConstQualifier:
				updateConst(parsedType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::VolatileQualifier:
				updateVolatile(parsedType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::NamespaceAndNestedClass:
				updatePureName(parsedType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::Pointer:
				updatePointer(parsedType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::Reference:
				updateReference(parsedType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::Count:
				assert(false);	//If assert here, the type has a yet unsupported qualifier for this parser
				break;
		}
	}

	isPointer = !isReference && !pointers.empty();
}

void TypeInfo::updateConst(std::string& parsingStr) noexcept
{
	if (parsingStr.substr(0, 6) == _constQualifier)
	{
		isConst = true;
		parsingStr.erase(0, 6);
	}
}

void TypeInfo::updateVolatile(std::string& parsingStr) noexcept
{
	if (parsingStr.substr(0, 9) == _volatileQualifier)
	{
		isVolatile = true;
		parsingStr.erase(0, 9);
	}
}

void TypeInfo::updatePureName(std::string& parsingStr) noexcept
{
	//Here comes the actual type name, remove namespace / nested class name
	size_t charIndex;
	while ((charIndex = parsingStr.find_first_of(':')) != parsingStr.npos)	// : implies ::
	{
		parsingStr.erase(0, charIndex + 2);		// +2 to remove the ::
	}

	charIndex = parsingStr.find_first_of(' ');
	if (charIndex != parsingStr.npos)
	{
		pureName = parsingStr.substr(0, charIndex);
		parsingStr.erase(0, charIndex + 1);
	}
	else
	{
		pureName = parsingStr;
		parsingStr.clear();
	}
}

void TypeInfo::updatePointer(std::string& parsingStr) noexcept
{
	while (!parsingStr.empty() && parsingStr.front() == _pointerSymbol)
	{
		if (parsingStr.size() >= 2 && parsingStr[1] == 'c')	// c implies const
		{
			pointers.push_back(EPointerType::ConstPointer);
			parsingStr.erase(0, 7);
		}
		else
		{
			pointers.push_back(EPointerType::Pointer);
			parsingStr.erase(0, 2);	//* + white space
		}
	}
}

void TypeInfo::updateReference(std::string& parsingStr)	noexcept
{
	assert(parsingStr.size() == 1);

	if (parsingStr.front() == '&')
	{
		isReference = true;
		parsingStr.clear();
	}
}

std::ostream& refureku::operator<<(std::ostream& out_stream, TypeInfo const& typeInfo) noexcept
{
	out_stream << typeInfo.fullName;

	return out_stream;
}