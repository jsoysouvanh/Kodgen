#pragma once

#include "InfoStructures/EntityInfo.h"
#include "InfoStructures/TypeInfo.h"

namespace refureku
{
	class FieldInfo : public EntityInfo
	{
		public:
			struct FieldQualifier
			{
				bool Static			: 1;

			}			qualifiers;

			TypeInfo	type;

			FieldInfo()															= default;
			FieldInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			FieldInfo(FieldInfo const&)											= default;
			FieldInfo(FieldInfo&&)												= default;
			~FieldInfo()														= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, FieldInfo const& fieldInfo) noexcept;
}