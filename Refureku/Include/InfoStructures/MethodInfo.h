#pragma once

#include "InfoStructures/EntityInfo.h"

namespace refureku
{
	class MethodInfo : public EntityInfo
	{
		public:
		//Replace all this by a uint8 for memory opti
			bool	isDefault;
			bool	isStatic;
			bool	isVirtual;
			bool	isVirtualPure;
			bool	isConst;

			MethodInfo()														= default;
			MethodInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			MethodInfo(MethodInfo const&)										= default;
			MethodInfo(MethodInfo&&)											= default;
			~MethodInfo()														= default;
	};
}