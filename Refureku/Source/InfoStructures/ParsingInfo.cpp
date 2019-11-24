#include "InfoStructures/ParsingInfo.h"

#include <cassert>
#include <iostream>	//TO DELETE

#include "Helpers.h"

using namespace refureku;

void ParsingInfo::startStructParsing(CXCursor const& structCursor) noexcept
{
	_classStructLevel++;
	_currentClassCursor = structCursor;
	shouldCheckValidity = true;
	_accessSpecifier = EAccessSpecifier::Public;

	//std::cout << "START STRUCT" << std::endl;
}

void ParsingInfo::startClassParsing(CXCursor const& classCursor) noexcept
{
	_classStructLevel++;
	_currentClassCursor = classCursor;
	shouldCheckValidity = true;
	_accessSpecifier = EAccessSpecifier::Private;

	//std::cout << "START CLASS" << std::endl;
}

void ParsingInfo::startFieldParsing(CXCursor const& fieldCursor) noexcept
{
	_isParsingField = true;
	_currentFieldCursor = fieldCursor;
	shouldCheckValidity = true;
	//std::cout << "START FIELD" << std::endl;
}

void ParsingInfo::startMethodParsing(CXCursor const& methodCursor) noexcept
{
	_isParsingMethod = true;
	_currentMethodCursor = methodCursor;
	shouldCheckValidity = true;
	//std::cout << "START METHOD" << std::endl;
}

void ParsingInfo::startEnumParsing(CXCursor const& enumCursor) noexcept
{
	_isParsingEnum = true;
	_currentEnumCursor = enumCursor;
	shouldCheckValidity = true;
	//std::cout << "START ENUM" << std::endl;
}

void ParsingInfo::endStructOrClassParsing() noexcept
{
	_classStructLevel--;
	_currentClassCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	//std::cout << "END CLASS/STRUCT" << std::endl;
}

void ParsingInfo::endFieldParsing() noexcept
{
	_isParsingField = false;
	_currentFieldCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	//std::cout << "END FIELD" << std::endl;
}

void ParsingInfo::endMethodParsing() noexcept
{
	_isParsingMethod = false;
	_currentMethodCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	//std::cout << "END METHOD" << std::endl;
}

void ParsingInfo::endEnumParsing() noexcept
{
	_isParsingEnum = false;
	_currentEnumCursor = clang_getNullCursor();
	shouldCheckValidity = false;
	//std::cout << "END ENUM" << std::endl;
}

void ParsingInfo::updateAccessSpecifier(CXCursor const& enumCursor) noexcept
{
	_accessSpecifier = static_cast<EAccessSpecifier>(1 << clang_getCXXAccessSpecifier(enumCursor));
}

void ParsingInfo::addParsingError(EParsingError parsingError) noexcept
{
	_parsingResult.parsingErrors.emplace_back(ParsingError(parsingError));
}

CXChildVisitResult ParsingInfo::tryToAddClass(CXCursor const& classAnnotationCursor) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isClassValid(classAnnotationCursor))
	{
		_parsingResult.classes.emplace_back(ClassInfo(Helpers::getString(clang_getCursorDisplayName(_currentClassCursor)), std::move(*propertyGroup)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (_propertyParser.getParsingError() == EParsingError::Count)
		{
			endStructOrClassParsing();
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			_parsingResult.parsingErrors.emplace_back(ParsingError(_propertyParser.getParsingError(), clang_getCursorLocation(classAnnotationCursor)));

			return _parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

std::optional<PropertyGroup> ParsingInfo::isClassValid(CXCursor currentCursor) noexcept
{
	shouldCheckValidity = false;
	_propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _propertyParser.getClassProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

CXChildVisitResult ParsingInfo::parseField(CXCursor const& fieldCursor) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (shouldCheckValidity)
	{
		return tryToAddField(fieldCursor);
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult ParsingInfo::tryToAddField(CXCursor const& fieldAnnotationCursor) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isFieldValid(fieldAnnotationCursor))
	{
		FieldInfo& field = _parsingResult.classes.back().fields.at(_accessSpecifier).emplace_back(FieldInfo(Helpers::getString(clang_getCursorDisplayName(_currentFieldCursor)), std::move(*propertyGroup)));
		field.type = *extractTypeInfo(clang_getCanonicalType(clang_getCursorType(_currentFieldCursor)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (_propertyParser.getParsingError() == EParsingError::Count)
		{
			//endStructOrClassParsing();
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			_parsingResult.parsingErrors.emplace_back(ParsingError(_propertyParser.getParsingError(), clang_getCursorLocation(fieldAnnotationCursor)));

			return _parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

std::optional<PropertyGroup> ParsingInfo::isFieldValid(CXCursor currentCursor) noexcept
{
	shouldCheckValidity = false;
	_propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _propertyParser.getFieldProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

CXChildVisitResult ParsingInfo::parseMethod(CXCursor const& methodCursor) noexcept
{
	//Check for any annotation attribute if the flag is raised
	if (shouldCheckValidity)
	{
		return tryToAddMethod(methodCursor);
	}

	CXCursorKind cursorKind = clang_getCursorKind(methodCursor);
	std::string cursorName = Helpers::getString(clang_getCursorSpelling(methodCursor));
	std::string cursorKindAsString = Helpers::getString(clang_getCursorKindSpelling(clang_getCursorKind(methodCursor)));
	//std::cout << "ParsingInfo::parseMethod: Cursor kind : " << cursorKindAsString << " : " << cursorName << std::endl;

	switch (cursorKind)
	{
		case CXCursorKind::CXCursor_CXXFinalAttr:
			_currentMethodInfo->qualifiers |= MethodInfo::EMethodQualifier::Final;
			break;

		case CXCursorKind::CXCursor_CXXOverrideAttr:
			_currentMethodInfo->qualifiers |= MethodInfo::EMethodQualifier::Override;
			break;

		case CXCursorKind::CXCursor_ParmDecl:
			//TODO: handle parameters here
			break;

		default:
			std::cout << "Unknown method sub cursor: " << cursorKindAsString << std::endl;
			break;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult ParsingInfo::tryToAddMethod(CXCursor const& methodAnnotationCursor) noexcept
{
	if (std::optional<PropertyGroup> propertyGroup = isMethodValid(methodAnnotationCursor))
	{
		_currentMethodInfo = &_parsingResult.classes.back().methods.at(_accessSpecifier).emplace_back(MethodInfo(Helpers::getString(clang_getCursorDisplayName(_currentMethodCursor)), std::move(*propertyGroup)));
		setupMethod(_currentMethodCursor, *_currentMethodInfo);

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (_propertyParser.getParsingError() == EParsingError::Count)
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}
		else	//Fatal parsing error occured
		{
			_parsingResult.parsingErrors.emplace_back(ParsingError(_propertyParser.getParsingError(), clang_getCursorLocation(methodAnnotationCursor)));

			return _parsingSettings->shouldAbortParsingOnFirstError ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;
		}
	}
}

void ParsingInfo::setupMethod(CXCursor const& methodCursor, MethodInfo& methodInfo)	noexcept
{
	CXType methodType = clang_getCursorType(methodCursor);

	assert(methodType.kind == CXTypeKind::CXType_FunctionProto);

	//Define return type
	methodInfo.returnType =	*extractTypeInfo(clang_getCanonicalType(clang_getResultType(methodType)));

	//Define method qualifiers
	if (clang_CXXMethod_isDefaulted(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Default;
	}
	if (clang_CXXMethod_isStatic(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Static;
	}
	if (clang_CXXMethod_isVirtual(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Virtual;
	}
	if (clang_CXXMethod_isPureVirtual(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::PureVirtual;
	}
	if (clang_CXXMethod_isConst(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Const;
	}
	if (clang_Cursor_isFunctionInlined(methodCursor))
	{
		methodInfo.qualifiers |= MethodInfo::EMethodQualifier::Inline;
	}
}

std::optional<PropertyGroup> ParsingInfo::isMethodValid(CXCursor currentCursor) noexcept
{
	shouldCheckValidity = false;
	_propertyParser.clean();

	if (clang_getCursorKind(currentCursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return _propertyParser.getMethodProperties(Helpers::getString(clang_getCursorSpelling(currentCursor)));
	}

	return std::nullopt;
}

std::optional<TypeInfo> ParsingInfo::extractTypeInfo(CXType const& cursorType) const noexcept
{
	TypeInfo result;

	//CXType cursorType = clang_getCursorType(typedCursor);

	if (cursorType.kind != CXTypeKind::CXType_Invalid)
	{
		result.initialize(Helpers::getString(clang_getTypeSpelling(cursorType)));
	}
	else
	{
		assert(false);	//Should never reach this point
	}

	return result;
}

bool ParsingInfo::hasErrorOccured() const noexcept
{
	return !_parsingResult.parsingErrors.empty();
}

ParsingResult ParsingInfo::extractParsingResult() noexcept
{
	return std::move(_parsingResult);
}

uint8 ParsingInfo::getClassStructLevel() const noexcept
{
	return _classStructLevel;
}

CXCursor const& ParsingInfo::getCurrentClassCursor() const noexcept
{
	return _currentClassCursor;
}

bool ParsingInfo::isParsingEnum() const noexcept
{
	return _isParsingEnum;
}

bool ParsingInfo::isParsingField() const noexcept
{
	return _isParsingField;
}

bool ParsingInfo::isParsingMethod() const noexcept
{
	return _isParsingMethod;
}

CXCursor const& ParsingInfo::getCurrentEnumCursor()const noexcept
{
	return _currentEnumCursor;
}

CXCursor const& ParsingInfo::getCurrentFieldCursor()const noexcept
{
	return _currentFieldCursor;
}

CXCursor const& ParsingInfo::getCurrentMethodCursor()const noexcept
{
	return _currentMethodCursor;
}

EAccessSpecifier ParsingInfo::getAccessSpecifier() const noexcept
{
	return _accessSpecifier;
}

void ParsingInfo::setParsingSettings(ParsingSettings const* parsingSettings) noexcept
{
	_parsingSettings = parsingSettings;
	_propertyParser.setup(&parsingSettings->propertyParsingSettings);
}

ParsingSettings const* ParsingInfo::getParsingSettings() const noexcept
{
	return _parsingSettings;
}