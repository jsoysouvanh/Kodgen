#pragma once

#include "PropertyMacros.h"

namespace TestNamespace
{
	class RfrkClass(Prop1, Prop2[SubProp21, SubProp22], Prop3) TestClass final
	{
		private:
		#pragma region Variables

		RfrkField(var1 is following)
		int				var1 = 42;
		float			var2;
		//MyAwesomeType*	var3 = nullptr;
		class Hey*		var4 = nullptr;

		#pragma endregion

		protected:

		public:
		#pragma region Methods

		RfrkMethod(int someMethod(int) is following)
		int someMethod(int);
		int dsomeMethod3(int) noexcept;
		//int someMethosd3(int) override;
		virtual int someMethodd3(int) const;

		#pragma endregion
	};

	class TestClass2
	{
		private:
			#pragma region Variables

			RfrkMethod(var1 is following)
			int				var1 = 42;
			float			var2;

			#pragma endregion

			protected:

			public:
			#pragma region Methods

			RfrkMethod(int someMethod(int) is following)
			int someMethod(int);
			int dsomeMethod3(int) noexcept;
			virtual int someMethodd3(int) const;

			#pragma endregion
	};

	enum class RfrkEnum(Some enum) MyEnum
	{
		EVal1 RfrkEnumVal(heyhey) = 1 << 0,
		EVal2 = 1 << 1,
		Count
	};
}