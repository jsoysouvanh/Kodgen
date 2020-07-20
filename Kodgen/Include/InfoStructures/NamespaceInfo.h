#pragma once

#include <string>
#include <vector>

#include "InfoStructures/StructClassInfo.h"
#include "InfoStructures/EnumInfo.h"

namespace kodgen
{
	struct NamespaceInfo : public EntityInfo
	{
		public:
			/**
			*	Nested namespaces, classes, structs and enums
			*/
			std::vector<NamespaceInfo>		namespaces;
			std::vector<StructClassInfo>	classes;
			std::vector<StructClassInfo>	structs;
			std::vector<EnumInfo>			enums;
	};
}