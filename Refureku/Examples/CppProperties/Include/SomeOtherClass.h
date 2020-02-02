#pragma once

#include <string>

#include "RefurekuConfig.h"
#include "SomeClass.h"

#include "Generated/SomeOtherClass.myCustomExtension.h"

namespace SomeNamespace
{
	class RfrkClass() SomeOtherClass
	{
		private:
			RfrkField(Get[const, &], Get[&])
			float		_someFloat	= 42.42f;

			RfrkField(Get[const])
			SomeClass*	_someClass	= nullptr;

		public:
			SomeOtherClass() = default;

		SomeOtherClass_GENERATED
	};
}