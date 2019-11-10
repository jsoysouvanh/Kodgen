#pragma once

#include <clang-c/Index.h>

#include "SafeFilesystem.h"

namespace refureku
{
	class Parser
	{
		private:
		
		protected:
			CXIndex _clangIndex;

			std::string	getString(CXString& clangString);

		public:
			Parser() noexcept;
			~Parser() noexcept;

			virtual bool Parse(fs::path const& parseFile) const noexcept;
	};
}