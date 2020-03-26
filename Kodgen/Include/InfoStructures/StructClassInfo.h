#pragma once

#include <vector>
#include <unordered_map>
#include <clang-c/Index.h>

#include "InfoStructures/EntityInfo.h"
#include "Misc/EAccessSpecifier.h"
#include "InfoStructures/FieldInfo.h"
#include "InfoStructures/MethodInfo.h"

namespace kodgen
{
	/**
	*	This class defines common data / behaviors shared by class and structs
	*/
	class StructClassInfo : public EntityInfo
	{
		private:
			void initContainers() noexcept;

		public:
			struct ParentInfo
			{
				EAccessSpecifier	inheritanceAccess;
				TypeInfo			parent;
			};

			struct ClassQualifiers
			{
				bool isFinal		: 1;
			}																qualifiers;

			//std::vector<ParentInfo>										parents;
			std::unordered_map<EAccessSpecifier, std::vector<TypeInfo>>		parents;	//TODO: change that
			std::vector<FieldInfo>											fields;
			std::vector<MethodInfo>											methods;


			StructClassInfo()																				noexcept;
			StructClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup, EType&& entityType)	noexcept;
			StructClassInfo(StructClassInfo const&)															= default;
			StructClassInfo(StructClassInfo&&)																= default;
			~StructClassInfo()																				= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, kodgen::StructClassInfo const&) noexcept;
}