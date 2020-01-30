#pragma once

#include "CodeGen/GeneratedCodeTemplate.h"

namespace refureku
{
	class TestGeneratedCodeTemplate : public GeneratedCodeTemplate
	{
		virtual void generateCode(GeneratedFile& generatedFile, EntityInfo const& entityInfo) noexcept override
		{
			switch (entityInfo.entityType)
			{
				case EntityInfo::EType::Class:
					generatedFile.writeLines(	"#define _RFRK_GENERATED_CLASS_" + entityInfo.name + " //TODO something",
												"",
												"/**");

					generatedFile.writeLine("Fields");
					for (std::pair<EAccessSpecifier, std::vector<FieldInfo>> const fieldIt : static_cast<StructClassInfo const&>(entityInfo).fields)
					{
						generatedFile.writeLine(toString(fieldIt.first));

						for (FieldInfo const& field : fieldIt.second)
						{
							generatedFile.writeLine(field.type.pureName + " -> " + field.type.canonicalPureName + " " + field.name);
						}
					}

					generatedFile.writeLine("Methods");
					for (std::pair<EAccessSpecifier, std::vector<MethodInfo>> const methodIt : static_cast<StructClassInfo const&>(entityInfo).methods)
					{
						generatedFile.writeLine(toString(methodIt.first));

						for (MethodInfo const& method : methodIt.second)
						{
							std::string methodAsString;

							methodAsString += method.name + "(";

							for (TypeInfo typeInfo : method.parameters)
							{
								methodAsString += typeInfo.pureName + " -> " + typeInfo.canonicalPureName + ", ";
							}

							methodAsString += ")";

							generatedFile.writeLine(methodAsString);
						}
					}

					generatedFile.writeLine("*/");

					break;

				case EntityInfo::EType::Struct:
					generatedFile.writeLine(	"#define _RFRK_GENERATED_STRUCT_" + entityInfo.name + " //TODO something");
					break;

				case EntityInfo::EType::Enum:
					
					EnumInfo const& enumInfo = static_cast<EnumInfo const&>(entityInfo);

					generatedFile.writeLines(	"#define _RFRK_GENERATED_ENUM_" + entityInfo.name + " //TODO something",
												"/*",
												"enum class " + enumInfo.name + "Reflect : " + enumInfo.underlyingType,
												"{");

					for (EnumValueInfo const& evi : enumInfo.enumValues)
					{
						generatedFile.writeLine("\t" + evi.name + " = " + std::to_string(evi.defaultValue) + ",");
					}

					generatedFile.writeLines(	"};",
												"this",
												"is",
												"a",
												"test",
												"*/");

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