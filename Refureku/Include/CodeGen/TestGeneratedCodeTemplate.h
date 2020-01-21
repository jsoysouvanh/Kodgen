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
					write(	"#define _RFRK_GENERATED_ENUM_" + entityInfo.name + " //TODO something");
					break;
			}
		}
	};
}