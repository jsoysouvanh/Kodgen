#pragma once

#include <vector>

#include "RefurekuConfig.h"
#include "SomeClass.h"

#include "Generated/SomeOtherClass.myCustomExtension.h"

namespace SomeNamespace
{
	class RfrkClass() SomeOtherClass
	{
		private:
			RfrkField(Get[const, &])
			std::vector<SomeClass*>		_someVectorOfClasses;

			RfrkField(Get[const, &], Get[&])
			int							_int;

		public:
			SomeOtherClass() = default;

		SomeOtherClass_GENERATED
	};
}