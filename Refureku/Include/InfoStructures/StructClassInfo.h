#pragma once

#include "InfoStructures/EntityInfo.h"

#include <vector>
#include <clang-c/Index.h>

#include "EAccessSpecifier.h"
#include "InfoStructures/FieldInfo.h"
#include "InfoStructures/MethodInfo.h"

namespace refureku
{
	/**
	*	This class defines common data / behaviors shared by class and structs
	*/
	class StructClassInfo : public EntityInfo
	{
		public:
			bool						isFinal;

			std::vector<std::string>	parents;
			std::vector<FieldInfo>		fields;
			std::vector<MethodInfo>		methods;

			StructClassInfo()															= default;
			StructClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			StructClassInfo(StructClassInfo const&)										= default;
			StructClassInfo(StructClassInfo&&)											= default;
			~StructClassInfo()															= default;

			void AddField(EAccessSpecifier accessSpecifier, CXCursor fieldCursor)	noexcept;
			void AddMethod(EAccessSpecifier accessSpecifier, CXCursor methodCursor)	noexcept;
	};
}