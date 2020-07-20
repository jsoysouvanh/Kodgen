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

namespace kodgen
{
	class NamespaceParser2 : public ClassParser2
	{
		//TODO
	};
}