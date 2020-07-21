#pragma once

#include <clang-c/Index.h>

#include "Misc/FundamentalTypes.h"
#include "InfoStructures/ParsingInfo.h"
#include "Properties/PropertyGroup.h"
#include "Parsing/EntityParser.h"
#include "Parsing/ClassParser.h"
#include "Parsing/EnumParser.h"

namespace kodgen
{
	class NamespaceParser final : public EntityParser
	{
		private:
			ClassParser	_classParser;
			EnumParser	_enumParser;
			//Add Field / Method parser?
			
			//void					initClassInfos(StructClassInfo& toInit)	const	noexcept;
			//CXChildVisitResult	parseField(CXCursor fieldCursor)				noexcept;
			//CXChildVisitResult	parseMethod(CXCursor methodCursor)				noexcept;

		protected:
			virtual opt::optional<PropertyGroup>	isEntityValid(CXCursor const& currentCursor)								noexcept override final;
			virtual CXChildVisitResult				setAsCurrentEntityIfValid(CXCursor const& classAnnotationCursor)			noexcept override final;

			void									addToParents(CXCursor cursor, ParsingInfo& parsingInfo)				const	noexcept;	
			void									updateAccessSpecifier(CXCursor const& cursor)						const	noexcept;

		public:
			NamespaceParser()						= default;
			NamespaceParser(NamespaceParser const&) = default;
			NamespaceParser(NamespaceParser&&)		= default;
			~NamespaceParser()						= default;

			virtual CXChildVisitResult	endParsing()										noexcept override final;
			virtual CXChildVisitResult	parse(CXCursor const& currentCursor)				noexcept override final;
			virtual void				reset()												noexcept override final;
			virtual void				setParsingInfo(ParsingInfo* info)					noexcept override final;

			void						startClassParsing(CXCursor const& currentCursor)	noexcept;
			void						startStructParsing(CXCursor const& currentCursor)	noexcept;
	};
}

#include "Parsing/ClassParser.h"
#include "Parsing/ParsingResults/NamespaceParsingResult.h"

namespace kodgen
{
	class NamespaceParser2 : public ClassParser2
	{
		private:
			/**
			*	@brief This method is called at each node (cursor) of the parsing.
			*
			*	@param cursor		Current cursor to parse.
			*	@param parentCursor	Parent of the current cursor.
			*	@param clientData	Pointer to a data provided by the client. Must contain a NamespaceParser*.
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
			*	@brief Set the parsed namespace if it is a valid one.
			*
			*	@param annotationCursor The cursor used to check namespace validity.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult				setParsedEntity(CXCursor const& annotationCursor)		noexcept;

			/**
			*	@brief Init the context object of this parser.
			*
			*	@param namespaceCursor	Root cursor of the namespace to parse.
			*	@param parsingSettings	ParsingSettings to use to parse this namespace.
			*	@param propertyParser	PropertyParser to use to parse properties.
			*	@param out_result		Result to fill during parsing.
			*/
			void							initContext(CXCursor const&				namespaceCursor,
														ParsingSettings const&		parsingSettings,
														PropertyParser&				propertyParser,
														NamespaceParsingResult&		out_result)		noexcept;

			/**
			*	@brief Helper to get the ParsingResult contained in the context as a NamespaceParsingResult.
			*
			*	@return The cast NamespaceParsingResult.
			*/
			inline NamespaceParsingResult*	getParsingResult()										noexcept;

		public:
			NamespaceParser2()							= default;
			NamespaceParser2(NamespaceParser2 const&)	= default;
			NamespaceParser2(NamespaceParser2&&)		= default;
			~NamespaceParser2()							= default;

			/**
			*	@brief Parse the namespace starting at the provided AST cursor.
			*
			*	@param namespaceCursor	AST cursor to the namespace to parse.
			*	@param parsingSettings	ParsingSettings used to parse the namespace.
			*	@param propertyParser	PropertyParser used to parse the namespace properties.
			*	@param out_result		Result filled while parsing the namespace.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult	parse(CXCursor const&			namespaceCursor,
									  ParsingSettings const&	parsingSettings,
									  PropertyParser&			propertyParser,
									  NamespaceParsingResult&	out_result)		noexcept;
	};

	#include "Parsing/NamespaceParser.inl"
}