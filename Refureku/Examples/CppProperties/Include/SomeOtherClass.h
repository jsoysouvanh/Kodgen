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
			RfrkField(Get[const, &], Get[&])
			float						_someFloat;

			RfrkField(Get[const, &])
			std::vector<SomeClass*>		_someVectorOfSomeClasses;

			RfrkField(Get[const, &])
			std::vector<std::string>	_someVectorOfStrings;

			RfrkField(Get[const])
			SomeClass*					_someClass;

			RfrkField(Get[const, &])
			std::vector<int>			_someVectorOfInt;

		public:
			SomeOtherClass() = default;

		SomeOtherClass_GENERATED
	};
}