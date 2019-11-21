#pragma once

#include "InfoStructures/EntityInfo.h"

namespace refureku
{
	class FieldInfo : public EntityInfo
	{
		public:
			FieldInfo()															= default;
			FieldInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			FieldInfo(FieldInfo const&)											= default;
			FieldInfo(FieldInfo&&)												= default;
			~FieldInfo()														= default;
	};
}