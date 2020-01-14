#pragma once

#include "InfoStructures/EntityInfo.h"

#include <vector>
#include <unordered_map>
#include <clang-c/Index.h>

#include "Misc/EAccessSpecifier.h"
#include "InfoStructures/FieldInfo.h"
#include "InfoStructures/MethodInfo.h"

namespace refureku
{
	/**
	*	This class defines common data / behaviors shared by class and structs
	*/
	class StructClassInfo : public EntityInfo
	{
		private:
			void initContainers() noexcept;

		public:
			bool						isFinal;

			std::vector<std::string>	parents;
			std::unordered_map<EAccessSpecifier, std::vector<FieldInfo>>	fields;
			std::unordered_map<EAccessSpecifier, std::vector<MethodInfo>>	methods;

			StructClassInfo()															noexcept;
			StructClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			StructClassInfo(StructClassInfo const&)										= default;
			StructClassInfo(StructClassInfo&&)											= default;
			~StructClassInfo()															= default;
	};
}