#include "InfoStructures/TypeInfo.h"

#include <iostream>
#include <cassert>

#include "Misc/Helpers.h"

using namespace refureku;

TypeInfo::TypeInfo(CXType cursorType) noexcept
{
	assert(cursorType.kind != CXTypeKind::CXType_Invalid);

	initialize(cursorType);
}

void TypeInfo::incrementParsingStep(EParsingStep& ps) noexcept
{
	ps = static_cast<EParsingStep>(static_cast<EParsingStepUnderlyingType>(ps) + 1);
}

void TypeInfo::initialize(CXType cursorType) noexcept
{
	CXType	canonicalType = clang_getCanonicalType(cursorType);

	fullName			= Helpers::getString(clang_getTypeSpelling(cursorType));
	canonicalFullName	= Helpers::getString(clang_getTypeSpelling(canonicalType));

	//Remove class or struct keyword
	removeForwardDeclaredClassQualifier(fullName);

	//Copy the full name for parsing
	std::string parsedType					= fullName;
	std::string parsedCanonicalType			= canonicalFullName;
	bool		shouldParseNonCanonicalType	= !clang_equalTypes(canonicalType, cursorType);

	EParsingStep parsingStep = EParsingStep::ConstQualifier;

	while (!parsedCanonicalType.empty())
	{
		switch (parsingStep)
		{
			case EParsingStep::ConstQualifier:
				updateConst(parsedCanonicalType, parsedType, shouldParseNonCanonicalType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::VolatileQualifier:
				updateVolatile(parsedCanonicalType, parsedType, shouldParseNonCanonicalType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::NamespaceAndNestedClass:
				updatePureName(parsedCanonicalType, parsedType, shouldParseNonCanonicalType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::Pointer:
				updatePointer(parsedCanonicalType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::Reference:
				updateReference(parsedCanonicalType);
				incrementParsingStep(parsingStep);
				break;

			case EParsingStep::Count:
				assert(false);	//If assert here, the type has a yet unsupported qualifier for this parser
				break;
		}
	}

	isPointer = !isReference && !pointers.empty();
}

void TypeInfo::removeForwardDeclaredClassQualifier(std::string& parsingStr) const noexcept
{
	std::string expectedKeyword = parsingStr.substr(0, 7);

	//Check for the struct keyword
	if (expectedKeyword == _structQualifier)
	{
		parsingStr.erase(0, 7);
	}
	else
	{
		//Check for the class keyword
		expectedKeyword.pop_back();

		if (expectedKeyword == _classQualifier)
		{
			parsingStr.erase(0, 6);
		}
	}
}

void TypeInfo::updateConst(std::string& parsingCanonicalStr, std::string& parsingStr, bool shouldConsider2ndArg) noexcept
{
	if (parsingCanonicalStr.substr(0, 6) == _constQualifier)
	{
		isConst = true;
		parsingCanonicalStr.erase(0, 6);
	}

	if (shouldConsider2ndArg && parsingStr.substr(0, 6) == _constQualifier)
	{
		parsingStr.erase(0, 6);
	}
}

void TypeInfo::updateVolatile(std::string& parsingCanonicalStr, std::string& parsingStr, bool shouldConsider2ndArg) noexcept
{
	if (parsingCanonicalStr.substr(0, 9) == _volatileQualifier)
	{
		isVolatile = true;
		parsingCanonicalStr.erase(0, 9);
	}

	if (shouldConsider2ndArg && parsingStr.substr(0, 9) == _volatileQualifier)
	{
		parsingStr.erase(0, 9);
	}
}

void TypeInfo::updatePureName(std::string& parsingCanonicalStr, std::string& parsingStr, bool shouldConsider2ndArg) noexcept
{
	//Here comes the actual type name, remove namespace / nested class name
	size_t charIndex;
	while ((charIndex = parsingCanonicalStr.find_first_of(':')) != parsingCanonicalStr.npos)	// : implies ::
	{
		parsingCanonicalStr.erase(0, charIndex + 2);		// +2 to remove the ::
	}

	canonicalPureName = parsingCanonicalStr;

	charIndex = parsingCanonicalStr.find_first_of(' ');
	if (charIndex != parsingCanonicalStr.npos)
	{
		parsingCanonicalStr.erase(0, charIndex + 1);
	}
	else
	{
		parsingCanonicalStr.clear();
	}

	if (shouldConsider2ndArg)
	{
		//Do the same for the non-canonical type
		while ((charIndex = parsingStr.find_first_of(':')) != parsingStr.npos)	// : implies ::
		{
			parsingStr.erase(0, charIndex + 2);		// +2 to remove the ::
		}

		pureName = parsingStr;
	}
	else
	{
		pureName = canonicalPureName;
	}
}

void TypeInfo::updatePointer(std::string& parsingCanonicalStr) noexcept
{
	while (!parsingCanonicalStr.empty() && parsingCanonicalStr.front() == '*')
	{
		if (parsingCanonicalStr.size() >= 2 && parsingCanonicalStr[1] == 'c')	// c implies const
		{
			pointers.push_back(EPointerType::ConstPointer);
			parsingCanonicalStr.erase(0, 7);
		}
		else
		{
			pointers.push_back(EPointerType::Pointer);
			parsingCanonicalStr.erase(0, 2);	//* + white space
		}
	}
}

void TypeInfo::updateReference(std::string& parsingCanonicalStr)	noexcept
{
	assert(parsingCanonicalStr.size() == 1);

	if (parsingCanonicalStr.front() == '&')
	{
		isReference = true;
		parsingCanonicalStr.clear();
	}
}

std::ostream& refureku::operator<<(std::ostream& out_stream, TypeInfo const& typeInfo) noexcept
{
	out_stream << typeInfo.fullName;

	return out_stream;
}