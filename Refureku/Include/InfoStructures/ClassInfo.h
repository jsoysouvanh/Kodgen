#pragma once

#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "EAccessSpecifier.h"
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

			void AddField(EAccessSpecifier accessSpecifier, CXCursor fieldCursor)	noexcept;
			void AddMethod(EAccessSpecifier accessSpecifier, CXCursor methodCursor)	noexcept;
	};
}