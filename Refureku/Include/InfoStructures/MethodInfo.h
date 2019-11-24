#pragma once

#include <iostream>

#include "FundamentalTypes.h"

#include "InfoStructures/EntityInfo.h"
#include "TypeInfo.h"

namespace refureku
{
	class MethodInfo : public EntityInfo
	{
		public:
			using EMethodQualifierUnderlyingType = uint16;

			enum EMethodQualifier : EMethodQualifierUnderlyingType
			{
				Default			= 1 << 0,
				Static			= 1 << 1,
				Virtual			= 1 << 2,
				PureVirtual		= 1 << 3,
				Inline			= 1 << 4,
				Override		= 1 << 5,
				Final			= 1 << 6,
				Const			= 1 << 7,

				Count			= 1 << 8
			};

			EMethodQualifierUnderlyingType	qualifiers : sizeof(EMethodQualifierUnderlyingType);
			TypeInfo						returnType;

			MethodInfo()														= default;
			MethodInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			MethodInfo(MethodInfo const&)										= default;
			MethodInfo(MethodInfo&&)											= default;
			~MethodInfo()														= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, MethodInfo const& methodInfo) noexcept;
}