#pragma once

#include <clang-c/Index.h>

#include "Misc/FundamentalTypes.h"
#include "Misc/EAccessSpecifier.h"
#include "InfoStructures/ParsingInfo.h"
#include "Parsing/EntityParser.h"
#include "Parsing/FieldParser.h"
#include "Parsing/MethodParser.h"

namespace kodgen
{
	class ClassParser final : public EntityParser
	{
		private:
			EntityInfo::EType		_structOrClass	= EntityInfo::EType::Count;

			FieldParser				fieldParser;
			MethodParser			methodParser;

			void				initClassInfos(StructClassInfo& toInit)	const	noexcept;
			CXChildVisitResult	parseField(CXCursor fieldCursor)				noexcept;
			CXChildVisitResult	parseMethod(CXCursor methodCursor)				noexcept;

		protected:
			virtual opt::optional<PropertyGroup>	isEntityValid(CXCursor const& currentCursor)								noexcept override final;
			virtual CXChildVisitResult				setAsCurrentEntityIfValid(CXCursor const& classAnnotationCursor)			noexcept override final;
			
			void									addToParents(CXCursor cursor, ParsingInfo& parsingInfo)				const	noexcept;	
			void									updateAccessSpecifier(CXCursor const& cursor)						const	noexcept;

		public:
			ClassParser()					= default;
			ClassParser(ClassParser const&) = default;
			ClassParser(ClassParser&&)		= default;
			~ClassParser()					= default;

			virtual CXChildVisitResult	endParsing()										noexcept override final;
			virtual CXChildVisitResult	parse(CXCursor const& currentCursor)				noexcept override final;
			virtual void				reset()												noexcept override final;
			virtual void				setParsingInfo(ParsingInfo* info)					noexcept override final;

			void						startClassParsing(CXCursor const& currentCursor)	noexcept;
			void						startStructParsing(CXCursor const& currentCursor)	noexcept;
	};
}

#include <variant>
#include <cassert>

#include "Parsing/EntityParser.h"
#include "Parsing/EnumParser.h"
#include "Parsing/FieldParser.h"
#include "Parsing/MethodParser.h"
#include "Parsing/PropertyParser.h"
#include "Parsing/ParsingResults/ClassParsingResult.h"
#include "Parsing/ParsingResults/EnumParsingResult.h"
#include "Parsing/ParsingResults/FieldParsingResult.h"
#include "Parsing/ParsingResults/MethodParsingResult.h"
#include "InfoStructures/FieldInfo.h"
#include "InfoStructures/MethodInfo.h"
#include "InfoStructures/StructClassInfo.h"
#include "InfoStructures/EntityInfo.h"
#include "InfoStructures/NamespaceInfo.h"
#include "Misc/EAccessSpecifier.h"
#include "Misc/Optional.h"

namespace kodgen
{
	class ClassParser2 : public EntityParser2
	{
		private:
			/** Parser used to parse enums. */
			EnumParser2		enumParser;	

			/** Parser used to parse fields. */
			FieldParser2	fieldParser;

			/** Parser used to parse methods. */
			MethodParser2	methodParser;

			/**
			*	@brief This method is called at each node (cursor) of the parsing.
			*
			*	@param cursor		Current cursor to parse.
			*	@param parentCursor	Parent of the current cursor.
			*	@param clientData	Pointer to a data provided by the client. Must contain a ClassParser*.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			static CXChildVisitResult		parseEntity(CXCursor		cursor,
														CXCursor		parentCursor,
														CXClientData	clientData)						noexcept;

			/**
			*	@brief Push a new clean context to prepare struct/class parsing.
			*
			*	@param classCursor		Root cursor of the struct/class to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result to fill during parsing.
			*/
			void							pushContext(CXCursor const&			classCursor,
														ParsingContext const&	parentContext,
														ClassParsingResult&		out_result)				noexcept;

			/**
			*	@brief Retrieve the properties from the provided cursor if possible.
			*
			*	@param cursor Property cursor we retrieve information from.
			*
			*	@return A filled PropertyGroup if valid, else nullopt.
			*/
			opt::optional<PropertyGroup>	getProperties(CXCursor const& cursor)						noexcept;
			
			/**
			*	@brief Set the parsed struct/class if it is a valid one.
			*
			*	@param annotationCursor The cursor used to check struct/class validity.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult				setParsedEntity(CXCursor const& annotationCursor)			noexcept;

			/**
			*	@brief Update the access specifier in the parsing context.
			*
			*	@param cursor AST cursor to the new access specifier.
			*/
			void							updateAccessSpecifier(CXCursor const& cursor)				noexcept;
			
			/**
			*	@brief Add a base class (parent class) to the currently parsed struct/class info.
			*
			*	@param cursor AST cursor to the base class.
			*/
			void							addBaseClass(CXCursor cursor)								noexcept;

			/**
			*	@brief Add the provided struct/class result to the current class context result.
			*
			*	@param result ClassParsingResult to add.
			*/
			void							addClassResult(ClassParsingResult&& result)					noexcept;

			/**
			*	@brief Add the provided struct/class result to the current class context result.
			*
			*	@param result ClassParsingResult to add.
			*/
			void							addEnumResult(EnumParsingResult&& result)					noexcept;

			/**
			*	@brief Add the provided field result to the current class context result.
			*
			*	@param result FieldParsingResult to add.
			*/
			void							addFieldResult(FieldParsingResult&& result)					noexcept;

			/**
			*	@brief Add the provided method result to the current class context result.
			*
			*	@param result MethodParsingResult to add.
			*/
			void							addMethodResult(MethodParsingResult&& result)				noexcept;

			/**
			*	@brief Helper to get the ParsingResult contained in the context as a ClassParsingResult.
			*
			*	@return The cast ClassParsingResult.
			*/
			inline ClassParsingResult*		getParsingResult()											noexcept;

		protected:
			/**
			*	@brief Parse a struct or class.
			*
			*	@param namespaceCursor	AST cursor to the struct/class to parse.
			*	@param out_visitResult	An enum which indicates how to choose the next cursor to parse in the AST.
			*
			*	@return A structure containing information about the parsed struct/class.
			*/
			ClassParsingResult	parseClass(CXCursor const& classCursor, CXChildVisitResult& out_visitResult)	noexcept;

			/**
			*	@brief Parse an enum.
			*
			*	@param enumCursor		AST cursor to the enum to parse.
			*	@param out_visitResult	An enum which indicates how to choose the next cursor to parse in the AST.
			*
			*	@return A structure containing information about the parsed enum.
			*/
			EnumParsingResult	parseEnum(CXCursor const& enumCursor, CXChildVisitResult& out_visitResult)		noexcept;

			/**
			*	@brief Parse a field.
			*
			*	@param fieldCursor		AST cursor to the field to parse.
			*	@param out_visitResult	An enum which indicates how to choose the next cursor to parse in the AST.
			*
			*	@return A structure containing information about the parsed field.
			*/
			FieldParsingResult	parseField(CXCursor const& fieldCursor, CXChildVisitResult& out_visitResult)	noexcept;

			/**
			*	@brief Parse a function or method.
			*
			*	@param methodCursor		AST cursor to the function/method to parse.
			*	@param out_visitResult	An enum which indicates how to choose the next cursor to parse in the AST.
			*
			*	@return A structure containing information about the parsed function/method.
			*/
			MethodParsingResult	parseMethod(CXCursor const& methodCursor, CXChildVisitResult& out_visitResult)	noexcept;

		public:
			ClassParser2()						= default;
			ClassParser2(ClassParser2 const&)	= default;
			ClassParser2(ClassParser2&&)		= default;
			~ClassParser2()						= default;

			/**
			*	@brief Parse the struct/class starting at the provided AST cursor.
			*
			*	@param classCursor		AST cursor to the struct/class to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result filled while parsing the struct/class.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult	parse(CXCursor const&			classCursor,
									  ParsingContext const&		parentContext,
									  ClassParsingResult&		out_result)		noexcept;
	};

	#include "Parsing/ClassParser.inl"
}