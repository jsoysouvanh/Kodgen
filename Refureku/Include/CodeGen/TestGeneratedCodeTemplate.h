#pragma once

#include "GeneratedCodeTemplate.h"

namespace refureku
{
	class TestGeneratedCodeTemplate : public GeneratedCodeTemplate
	{
		virtual fs::path getGeneratedFilename(fs::path const& parsedFile, EntityInfo const& entityInfo)	noexcept override
		{
			fs::path filepath = parsedFile;

			filepath.replace_filename(entityInfo.name);

			return filepath.replace_extension(".generated");
		}

		virtual void generateCode(fs::path const& parsedFile, EntityInfo const& entityInfo) noexcept override
		{
			GeneratedCodeTemplate::generateCode(parsedFile, entityInfo);

			write("#pragma once");

			write(	"class " + entityInfo.name + 
					"{"
						"/*TODO*/"
					"};");
		}
	};
}