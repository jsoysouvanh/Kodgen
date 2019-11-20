#pragma once

#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "AccessSpecifier.h"
#include "InfoStructures/EntityInfo.h"

namespace refureku
{
	class ClassInfo : public EntityInfo
	{
		public:
			bool isFinal;
			//...

			ClassInfo() = default;
			~ClassInfo() = default;

			void AddField(AccessSpecifier accessSpecifier, CXCursor fieldCursor)	noexcept;
			void AddMethod(AccessSpecifier accessSpecifier, CXCursor methodCursor)	noexcept;
	};
}