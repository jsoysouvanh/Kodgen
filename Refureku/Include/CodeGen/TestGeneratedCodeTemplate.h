#pragma once

#include "GeneratedCodeTemplate.h"

namespace refureku
{
	class TestGeneratedCodeTemplate : public GeneratedCodeTemplate
	{
		virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo) noexcept override
		{
			write(	"#define _RFRK_GENERATED_" + entityInfo.name + " //TODO something");
		}
	};
}