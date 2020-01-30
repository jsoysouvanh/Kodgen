#pragma once

#include <iostream>
#include <vector>

#include "Misc/FundamentalTypes.h"

#include "InfoStructures/EntityInfo.h"
#include "TypeInfo.h"

namespace refureku
{
	class MethodInfo : public EntityInfo
	{
		public:
			struct MethodQualifier
			{
				bool Default		: 1;
				bool Static			: 1;
				bool Virtual		: 1;
				bool PureVirtual	: 1;
				bool Inline			: 1;
				bool Override		: 1;
				bool Final			: 1;
				bool Const			: 1;

			}						qualifiers;

			TypeInfo				returnType;
			std::vector<TypeInfo>	parameters;

			MethodInfo()														= default;
			MethodInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept;
			MethodInfo(MethodInfo const&)										= default;
			MethodInfo(MethodInfo&&)											= default;
			~MethodInfo()														= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, MethodInfo const& methodInfo) noexcept;
}