#pragma once

#include <clang-c/Index.h>

#include "Misc/Optional.h"
#include "Properties/PropertyGroup.h"
#include "Parsing/EntityParser.h"

namespace kodgen
{
	class FieldParser : public kodgen::EntityParser
	{
		private:
			virtual CXChildVisitResult				setAsCurrentEntityIfValid(CXCursor const& fieldAnnotationCursor)	noexcept override final;
			virtual	opt::optional<PropertyGroup>	isEntityValid(CXCursor const& currentCursor)						noexcept override final;

		public:
			FieldParser()					= default;
			FieldParser(FieldParser const&) = default;
			FieldParser(FieldParser&&)		= default;
			virtual ~FieldParser()			= default;

			virtual CXChildVisitResult	parse(CXCursor const& cursor)	noexcept override final;

			using EntityParser::startParsing;
	};
}


#include "Parsing/EntityParser.h"
#include "Parsing/ParsingResults/FieldParsingResult.h"

namespace kodgen
{
	class FieldParser2 : public EntityParser2
	{
		private:
			/**
			*	@brief This method is called at each node (cursor) of the parsing.
			*
			*	@param cursor		Current cursor to parse.
			*	@param parentCursor	Parent of the current cursor.
			*	@param clientData	Pointer to a data provided by the client. Must contain a FieldParser*.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			static CXChildVisitResult		parseEntity(CXCursor		cursor,
														CXCursor		parentCursor,
														CXClientData	clientData)					noexcept;

			/**
			*	@brief Retrieve the properties from the provided cursor if possible.
			*
			*	@param cursor Property cursor we retrieve information from.
			*
			*	@return A filled PropertyGroup if valid, else nullopt.
			*/
			opt::optional<PropertyGroup>	getProperties(CXCursor const& cursor)					noexcept;

			/**
			*	@brief Set the parsed field if it is a valid one.
			*
			*	@param annotationCursor The cursor used to check field validity.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult				setParsedEntity(CXCursor const& annotationCursor)		noexcept;

			/**
			*	@brief Init the context object of this parser.
			*
			*	@param fieldCursor		Root cursor of the field to parse.
			*	@param parsingSettings	ParsingSettings to use to parse this field.
			*	@param propertyParser	PropertyParser to use to parse properties.
			*	@param out_result		Result filled while parsing the field.
			*/
			void							initContext(CXCursor const&			fieldCursor,
														ParsingSettings const&	parsingSettings,
														PropertyParser&			propertyParser,
														FieldParsingResult&		out_result)			noexcept;

			/**
			*	@brief Helper to get the ParsingResult contained in the context as a FieldParsingResult.
			*
			*	@return The cast FieldParsingResult.
			*/
			inline FieldParsingResult*		getParsingResult()										noexcept;

		public:

			/**
			*	@brief Parse the field starting at the provided AST cursor.
			*
			*	@param fieldCursor		AST cursor to the field to parse.
			*	@param parsingSettings	ParsingSettings used to parse the field.
			*	@param propertyParser	PropertyParser used to parse the field properties.
			*	@param out_result		Result filled while parsing the field.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult	parse(CXCursor const&			fieldCursor,
									  ParsingSettings const&	parsingSettings,
									  PropertyParser&			propertyParser,
									  FieldParsingResult&		out_result) noexcept;
	};

	#include "Parsing/FieldParser.inl"
}