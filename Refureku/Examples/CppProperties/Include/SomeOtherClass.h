#pragma once

#include <string>
#include <vector>

#include "RefurekuConfig.h"
#include "SomeClass.h"

#include "Generated/SomeOtherClass.myCustomExtension.h"

template <typename T>
class SomeTemplateClass
{
	T* someT = nullptr;
};

namespace SomeNamespace
{
	class RfrkClass() SomeOtherClass
	{
		private:
			RfrkField(Get[const, &], Get[&])
			float							_someFloat	= 42.42f;

			RfrkField(Get[const, &])
			SomeClass						_someClass;

			RfrkField(Get[const, &])
			SomeTemplateClass<SomeClass>	_someTemplateClass;

			RfrkField(Get[const, &])
			std::vector<int>				_someVectorOfInt;
		
			RfrkField(Get[const, &])
			std::vector<SomeClass*>			_someVectorOfSomeClass;

			RfrkField(Get[const, &])
			std::pmr::vector<SomeClass*>	_someVectorOfSomeClass2; 

		public:
			SomeOtherClass() = default;

		SomeOtherClass_GENERATED
	};
}