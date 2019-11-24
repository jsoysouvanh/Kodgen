#pragma once

#include <vector>
#include <clang-c/Index.h>

#include "FundamentalTypes.h"
#include "EAccessSpecifier.h"
#include "ParsingSettings.h"
#include "ParsingResult.h"
#include "Properties/PropertyParser.h"
#include "InfoStructures/ClassInfo.h"
#include "InfoStructures/TypeInfo.h"

namespace refureku
{
	class ParsingInfo
	{
		private:
			/**
			*	
			*/
			ParsingSettings const* _parsingSettings = nullptr;

			/**
			*	PropertyParser.
			*	used by the Parser to retrieve information
			*/
			PropertyParser			_propertyParser;

			/*	Level of class/struct parsing
			*	0 if not parsing a class/struct,
			*	1 if parsing a class/struct,
			*	2 is parsing a nested class/struct
			*	etc...
			**/
			uint8					_classStructLevel				= 0;

			/*
			*	Clang cursor to the currently parsing class
			**/
			CXCursor				_currentClassCursor				= clang_getNullCursor();

			/*
			*	Is currently parsing an enum
			**/
			bool					_isParsingEnum					= false;

			/*
			*	Is currently parsing a field
			**/
			bool					_isParsingField					= false;

			/*
			*	Is currently parsing a method
			**/
			bool					_isParsingMethod				= false;

			/*
			*	Clang cursor to the currently parsing entity (either an enum, a field or a method)
			**/
			union
			{
				CXCursor	_currentEnumCursor = clang_getNullCursor();
				CXCursor	_currentFieldCursor;
				CXCursor	_currentMethodCursor;
			};

			/*
			*	Current class modifier
			**/
			EAccessSpecifier	_accessSpecifier	= EAccessSpecifier::Private;

			/**
			*	Final collected data
			*/
			ParsingResult		_parsingResult;

			CXChildVisitResult		tryToAddField(CXCursor const& fieldAnnotationCursor)			noexcept;
			CXChildVisitResult		tryToAddMethod(CXCursor const& methodAnnotationCursor)			noexcept;

			std::optional<TypeInfo>	extractTypeInfo(CXCursor const& typedCursor)			const	noexcept;

			/**
			*	Returns a filled PropertyGroup if the provided cursor describes a valid class, else an empty one
			*/
			std::optional<PropertyGroup> isClassValid(CXCursor currentCursor) noexcept;

			/**
			*	Returns a filled PropertyGroup if the provided cursor describes a valid field, else an empty one
			*/
			std::optional<PropertyGroup> isFieldValid(CXCursor currentCursor) noexcept;

			/**
			*	Returns a filled PropertyGroup if the provided cursor describes a valid method, else an empty one
			*/
			std::optional<PropertyGroup> isMethodValid(CXCursor currentCursor) noexcept;

		public:		
			/*
			*	Should check for any annotation
			*	TODO: move as private member with getter
			**/
			bool			shouldCheckValidity				= false;

			/*
			*	All collected class data
			**/
			std::vector<ClassInfo>	classInfos;

			/*
			*	//TODO struct data
			**/

			/*
			*	//TODO enumInfos;
			**/

			ParsingInfo()	= default;
			~ParsingInfo()	= default;

			void					startStructParsing(CXCursor const& structCursor)			noexcept;
			void					startClassParsing(CXCursor const& classCursor)				noexcept;
			void					startFieldParsing(CXCursor const& fieldCursor)				noexcept;
			void					startMethodParsing(CXCursor const& methodCursor)			noexcept;
			void					startEnumParsing(CXCursor const& enumCursor)				noexcept;

			void					endStructOrClassParsing()									noexcept;
			void					endFieldParsing()											noexcept;
			void					endMethodParsing()											noexcept;
			void					endEnumParsing()											noexcept;

			void					updateAccessSpecifier(CXCursor const& enumCursor)			noexcept;
			void					addParsingError(EParsingError parsingError)					noexcept;

			CXChildVisitResult		tryToAddClass(CXCursor const& classAnnotationCursor)		noexcept;

			CXChildVisitResult		parseField(CXCursor const& fieldCursor)						noexcept;

			CXChildVisitResult		parseMethod(CXCursor const& methodCursor)					noexcept;

			bool					hasErrorOccured()									const	noexcept;
			ParsingResult			extractParsingResult()										noexcept;

			uint8					getClassStructLevel()								const	noexcept;
			CXCursor const&			getCurrentClassCursor()								const	noexcept;
			CXCursor const&			getCurrentEnumCursor()								const	noexcept;
			CXCursor const&			getCurrentFieldCursor()								const	noexcept;
			CXCursor const&			getCurrentMethodCursor()							const	noexcept;
			bool					isParsingEnum()										const	noexcept;
			bool					isParsingField()									const	noexcept;
			bool					isParsingMethod()									const	noexcept;
			EAccessSpecifier		getAccessSpecifier()								const	noexcept;

			ParsingSettings const*	getParsingSettings()								const	noexcept;
			void					setParsingSettings(ParsingSettings const* parsingSettings)	noexcept;
			//PropertyParser&			getPropertyParser()											noexcept;
	};
}