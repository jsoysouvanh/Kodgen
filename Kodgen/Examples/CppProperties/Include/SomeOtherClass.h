#pragma once

#include <vector>
#include <unordered_map>

#include "SomeClass.h"

#include "Generated/SomeOtherClass.h.h"	//Last include

namespace SomeNamespace KGNamespace()
{
	template <typename T>
	class SomeTemplateClass
	{
		T* someT = nullptr;
	};

	class KGClass() SomeOtherClass
	{
		private:
			KGField(Get[const, &])
			float										_someFloat	= 42.42f; 
			
			KGField(Get[const, &])
			np1::SomeClass								_someClass;

			KGField(Get[const, &])
			SomeTemplateClass<np1::SomeClass>			_someTemplateClass;

			KGField(Get[const, &])
			std::vector<int>							_someVectorOfInt = { 1, 2, 3 };
			
			KGField(Get[const, &])
			std::vector<np1::SomeClass*>				_someVectorOfSomeClass { nullptr, nullptr };

			KGField(Get[const, &])
			std::unordered_map<int, np1::SomeClass*>	_someUmapOfSomeClass2{ { 1, nullptr } };

		public:
			SomeOtherClass() = default;

		SomeNamespace_SomeOtherClass_GENERATED
	};
}

File_SomeOtherClass_GENERATED