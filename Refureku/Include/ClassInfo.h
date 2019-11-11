#pragma once

#include <string>
#include <clang-c/Index.h>

#include "AccessSpecifier.h"

namespace refureku
{
	class ClassInfo
	{
		public:
			ClassInfo() = default;
			~ClassInfo() = default;

			void AddField(AccessSpecifier accessSpecifier, CXCursor fieldCursor);
			void AddMethod(AccessSpecifier accessSpecifier, CXCursor methodCursor);
	};
}