#pragma once

#include "CodeGen/GeneratedCodeTemplate.h"

namespace refureku
{
	class TestGeneratedCodeTemplate : public GeneratedCodeTemplate
	{
		virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo) noexcept override
		{
			switch (entityInfo.type)
			{
				case EntityInfo::EType::Class:
					writeLine(	"#define _RFRK_GENERATED_CLASS_" + entityInfo.name + " //TODO something");
					break;

				case EntityInfo::EType::Struct:
					writeLine(	"#define _RFRK_GENERATED_STRUCT_" + entityInfo.name + " //TODO something");
					break;

				case EntityInfo::EType::Enum:
					
					EnumInfo const& enumInfo = static_cast<EnumInfo const&>(entityInfo);

					writeLine(	"#define _RFRK_GENERATED_ENUM_" + entityInfo.name + " //TODO something");

					writeLine("/*");

					writeLine("enum class " + enumInfo.name + "Reflect : " + enumInfo.underlyingType);
					writeLine("{");

					for (EnumValueInfo const& evi : enumInfo.enumValues)
					{
						writeLine("\t" + evi.name + " = " + std::to_string(evi.defaultValue) + ",");
					}

					writeLine("};");

					writeLines("this", "is", "a", "test");

					writeLine("*/");

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