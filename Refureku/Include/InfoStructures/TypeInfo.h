#pragma once

#include "clang-c/Index.h"
#include <string>
#include <vector>

#include "FundamentalTypes.h"

namespace refureku
{
	class TypeInfo
	{
		private:
			using EParsingStepUnderlyingType = uint16;

			static constexpr char*	_constQualifier		= "const ";
			static constexpr char*	_volatileQualifier	= "volatile ";
			static constexpr char	_pointerSymbol		= '*';
			static constexpr char	_referenceSymbol	= '&';

			enum class EParsingStep : EParsingStepUnderlyingType
			{
				ConstQualifier = 0,
				VolatileQualifier,
				NamespaceAndNestedClass,
				Pointer,
				Reference,

				Count
			};

			void updateConst(std::string& parsingStr)		noexcept;
			void updateVolatile(std::string& parsingStr)	noexcept;
			void updatePureName(std::string& parsingStr)	noexcept;
			void updatePointer(std::string& parsingStr)		noexcept;
			void updateReference(std::string& parsingStr)	noexcept;

			void incrementParsingStep(EParsingStep& ps)		noexcept;

		public:
			enum class EPointerType : uint8
			{
				Pointer,
				ConstPointer
			};
			
			std::vector<EPointerType>	pointers;

			/**
			*	The full name represents the type name, containing all its qualifiers
			*	such as const, volatile or pointer (*) / reference (&) symbols
			*
			*	i.e. const volatile ExampleNamespace::ExampleClass *const*&
			*/
			std::string	fullName	= "";

			/**
			*	In contrast with the full name, the pure name represents the type name
			*	without all its qualifiers
			*
			*	If the fullName is
			*		const volatile ExampleNamespace::ExampleClass *const*&
			*	the pureName would be ExampleClass
			*/
			std::string	pureName	= "";
			bool		isConst		= false;
			bool		isVolatile	= false;
			bool		isPointer	= false;
			bool		isReference	= false;

			TypeInfo()						= default;
			TypeInfo(CXType cursorType)		noexcept;
			TypeInfo(TypeInfo const&)		= default;
			TypeInfo(TypeInfo&&)			= default;
			~TypeInfo()						= default;

			/**
			*	Init all internal flags according to the provided full name
			*/
			void initialize(std::string&& fullTypeName) noexcept;

			TypeInfo& operator=(TypeInfo const&)	= default;
			TypeInfo& operator=(TypeInfo&&)			= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, TypeInfo const& typeInfo) noexcept;
}