#pragma once

#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "InfoStructures/EntityInfo.h"
#include "AccessSpecifier.h"

namespace refureku
{
	class ClassInfo : public EntityInfo
	{
		private:
			

		public:
			ClassInfo() = default;
			~ClassInfo() = default;

			void AddField(AccessSpecifier accessSpecifier, CXCursor fieldCursor)	noexcept;
			void AddMethod(AccessSpecifier accessSpecifier, CXCursor methodCursor)	noexcept;
	};
}