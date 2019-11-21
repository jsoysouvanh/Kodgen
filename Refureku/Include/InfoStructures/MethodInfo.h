#pragma once

#include "InfoStructures/EntityInfo.h"

namespace refureku
{
	class MethodInfo : public EntityInfo
	{
		public:
			MethodInfo()														= default;
			MethodInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			MethodInfo(MethodInfo const&)										= default;
			MethodInfo(MethodInfo&&)											= default;
			~MethodInfo()														= default;
	};
}