#pragma once

#include "GeneratedCodeTemplate.h"

namespace refureku
{
	class TestGeneratedCodeTemplate : public GeneratedCodeTemplate
	{
		virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo) noexcept override
		{
			switch (entityInfo.type)
			{
				case EntityInfo::EType::Class:
					write(	"#define _RFRK_GENERATED_CLASS_" + entityInfo.name + " //TODO something");
					break;

				case EntityInfo::EType::Struct:
					write(	"#define _RFRK_GENERATED_STRUCT_" + entityInfo.name + " //TODO something");
					break;

				case EntityInfo::EType::Enum:
					
					EnumInfo const& enumInfo = static_cast<EnumInfo const&>(entityInfo);

					write(	"#define _RFRK_GENERATED_ENUM_" + entityInfo.name + " //TODO something");

					write("/*");

					write("enum class " + enumInfo.name + "Reflect : " + enumInfo.underlyingType);
					write("{");

					for (EnumValueInfo const& evi : enumInfo.enumValues)
					{
						write("\t" + evi.name + " = " + std::to_string(evi.defaultValue) + ",");
					}

					write("};");

					write("*/");

					writeMacro(	"SOME_MACRO(...)",
								"here",
								"is",
								"a",
								"test");

					break;
			}
		}
	};
}