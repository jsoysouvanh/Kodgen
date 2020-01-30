#pragma once

#include "clang-c/Index.h"
#include <string>
#include <vector>

#include "Misc/FundamentalTypes.h"

namespace refureku
{
	class TypeInfo
	{
		private:
			using EParsingStepUnderlyingType = uint16;

			static constexpr char const*	_classQualifier		= "class ";
			static constexpr char const*	_structQualifier	= "struct ";
			static constexpr char const*	_constQualifier		= "const ";
			static constexpr char const*	_volatileQualifier	= "volatile ";

			enum class EParsingStep : EParsingStepUnderlyingType
			{
				ConstQualifier = 0,
				VolatileQualifier,
				NamespaceAndNestedClass,
				Pointer,
				Reference,

				Count
			};

			/**
			*	Init all internal flags according to the provided type
			*/
			void initialize(CXType cursorType)																				noexcept;
			void removeForwardDeclaredClassQualifier(std::string& parsingStr)										const	noexcept;

			void updateConst(std::string& parsingCanonicalStr, std::string& parsingStr, bool shouldConsider2ndArg)			noexcept;
			void updateVolatile(std::string& parsingCanonicalStr, std::string& parsingStr, bool shouldConsider2ndArg)		noexcept;
			void updatePureName(std::string& parsingCanonicalStr, std::string& parsingStr, bool shouldConsider2ndArg)		noexcept;
			void updatePointer(std::string& parsingCanonicalStr)															noexcept;
			void updateReference(std::string& parsingCanonicalStr)															noexcept;

			void incrementParsingStep(EParsingStep& ps)																		noexcept;

		public:
			enum class EPointerType : uint8
			{
				Pointer,
				ConstPointer
			};
			
			/**
			*	This array contains info about the "pointer depth" of the canonical type
			*
			*	If the type is SomeType *const**const*, the pointers array would be
			*	{ Pointer, ConstPointer, Pointer, ConstPointer }	(left to right)
			*/
			std::vector<EPointerType>	pointers;

			/**
			*	The full name represents the type name, containing all its qualifiers
			*	such as const, volatile or nested info (namespace, outer class)
			*
			*	i.e. const volatile ExampleNamespace::ExampleClass *const*&
			*/
			std::string					fullName			= "";

			/**
			*	The canonical full name is the full name simplified by unwinding
			*	all aliases / typedefs
			*/
			std::string					canonicalFullName	= "";

			/**
			*	In contrast with the full name, the pure name represents the type name
			*	without all its pre-qualifiers
			*
			*	If the fullName is
			*		const volatile ExampleNamespace::ExampleClass *const*&
			*	the pureName would be ExampleClass *const*&
			*/
			std::string					pureName			= "";

			/**
			*	The canonical pure name is the pure name simplified by unwinding
			*	all aliases / typedefs
			*/
			std::string					canonicalPureName	= "";

			/**
			*	Is the type const qualified
			*/
			bool						isConst				= false;

			/**
			*	Is the type volatile qualified
			*/
			bool						isVolatile			= false;

			/**
			*	Is this type a pointer ?
			*	If true, you can check the pointers array for more info
			*/
			bool						isPointer			= false;

			/**
			*	Is this type a reference
			*/
			bool						isReference			= false;

			TypeInfo()						= default;
			TypeInfo(CXType cursorType)		noexcept;
			TypeInfo(TypeInfo const&)		= default;
			TypeInfo(TypeInfo&&)			= default;
			~TypeInfo()						= default;

			TypeInfo& operator=(TypeInfo const&)	= default;
			TypeInfo& operator=(TypeInfo&&)			= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, TypeInfo const& typeInfo) noexcept;
}