#pragma once

#include "InfoStructures/StructClassInfo.h"

namespace refureku
{
	class ClassInfo : public StructClassInfo
	{
		public:
			ClassInfo()															= default;
			ClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			ClassInfo(ClassInfo const&)											= default;
			ClassInfo(ClassInfo&&)												= default;
			~ClassInfo()														= default;
	};
}