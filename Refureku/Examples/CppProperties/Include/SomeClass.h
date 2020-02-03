#pragma once

#include <string>

#include "RefurekuConfig.h"

#include "Generated/SomeClass.myCustomExtension.h"

namespace SomeNamespace
{
	class RfrkClass() SomeClass
	{
		public:
			class SomeNestedClass{};

		private:
			RfrkField(Get[explicit], Set[])
			int													_someInt				= 2;

			RfrkField(Get[const, *], Set[])
			float												_someFloat				= 3.14f;

			RfrkField(Get[])
			char const											_someChar				= '*';

			RfrkField(Get[const, &], Set[])
			unsigned long long									_someUnsignedLongLong	= 42;

			RfrkField(Get[], Set[])
			SomeNestedClass*									_someNestedClass		= nullptr;

			RfrkField(Get[const, &], Set[])
			std::string											_someString				= "This is a test";
				
		public:
			SomeClass() = default;

		SomeClass_GENERATED
	};
}