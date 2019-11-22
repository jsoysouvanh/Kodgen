#pragma once

#include <string>
#include <iostream>

#include "Properties/PropertyGroup.h"

namespace refureku
{
	class EntityInfo
	{
		public:
			std::string		name = "";
			PropertyGroup	properties;

			EntityInfo()														= default;
			EntityInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			EntityInfo(EntityInfo const&)										= default;
			EntityInfo(EntityInfo&&)											= default;
			~EntityInfo()														= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, EntityInfo const&) noexcept;
}