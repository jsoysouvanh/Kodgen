#pragma once

#include "PropertyMacros.h"

namespace TestNamespace
{
	class Class1{};

	class RfrkClass(ClassProp1, ClassProp2[ClassSubProp21, ClassSubProp22], ClassProp3) TestClass final : public Class1
	{
		private:
		#pragma region Variables

		RfrkField(FieldProp1, FieldProp2[FieldSubProp21, FieldSubProp22], FieldProp3)
		int				var1 = 42;
		float			var2;
		//MyAwesomeType*	var3 = nullptr;
		class Hey*		var4 = nullptr;

		#pragma endregion

		protected:

		public:
		#pragma region Methods

		RfrkMethod(MethodProp1, MethodProp2[MethodSubProp21, MethodSubProp22], MethodProp3)
		int someMethod(int* param1);
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