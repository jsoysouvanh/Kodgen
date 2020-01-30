#pragma once

#include "CodeGen/GeneratedCodeTemplate.h"

namespace refureku
{
	class TestGeneratedCodeTemplate : public GeneratedCodeTemplate
	{
		virtual void generateCode(GeneratedFile& generatedFile, EntityInfo const& entityInfo) noexcept override
		{
			switch (entityInfo.type)
			{
				case EntityInfo::EType::Class:
					generatedFile.writeLine(	"#define _RFRK_GENERATED_CLASS_" + entityInfo.name + " //TODO something");

					generatedFile.writeLine("/**");

					for (std::pair<EAccessSpecifier, std::vector<MethodInfo>> const methodIt : static_cast<StructClassInfo const&>(entityInfo).methods)
					{
						generatedFile.writeLine(toString(methodIt.first));

						for (MethodInfo const& method : methodIt.second)
						{
							generatedFile.writeLine(method.returnType.fullName + " " + method.name);
						}
					}

					generatedFile.writeLine("*/");

					break;

				case EntityInfo::EType::Struct:
					generatedFile.writeLine(	"#define _RFRK_GENERATED_STRUCT_" + entityInfo.name + " //TODO something");
					break;

				case EntityInfo::EType::Enum:
					
					EnumInfo const& enumInfo = static_cast<EnumInfo const&>(entityInfo);

					generatedFile.writeLine(	"#define _RFRK_GENERATED_ENUM_" + entityInfo.name + " //TODO something");

					generatedFile.writeLine("/*");

					generatedFile.writeLine("enum class " + enumInfo.name + "Reflect : " + enumInfo.underlyingType);
					generatedFile.writeLine("{");

					for (EnumValueInfo const& evi : enumInfo.enumValues)
					{
						generatedFile.writeLine("\t" + evi.name + " = " + std::to_string(evi.defaultValue) + ",");
					}

					generatedFile.writeLine("};");

					generatedFile.writeLines("this", "is", "a", "test");

					generatedFile.writeLine("*/");

					generatedFile.writeMacro(	"SOME_MACRO(...)",
												"here",
												"is",
												"a",
												"test");

					break;
			}
		}
	};
}