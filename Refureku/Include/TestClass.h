#pragma once

#ifdef PARSER
#define TURBO_PROP(...)	__attribute__((annotate(#__VA_ARGS__)))
#else
#define TURBO_PROP(...)	
#endif

//#include <string>



namespace TestNamespace
{
	//TURBO_PROP(template <typename MyAwesomeType> is following)
	//template <typename MyAwesomeType>
	class TURBO_PROP(Heyoooooooooooooooooo) TestClass final
	{
		private:
		#pragma region Variables

		TURBO_PROP(var1 is following)
		int				var1 = 42;
		float			var2;
		//MyAwesomeType*	var3 = nullptr;
		class Hey*		var4 = nullptr;

		#pragma endregion

		protected:

		public:
		#pragma region Methods

		TURBO_PROP(int someMethod(int) is following)
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

			TURBO_PROP(var1 is following)
			int				var1 = 42;
			float			var2;

			#pragma endregion

			protected:

			public:
			#pragma region Methods

			TURBO_PROP(int someMethod(int) is following)
			int someMethod(int);
			int dsomeMethod3(int) noexcept;
			virtual int someMethodd3(int) const;

			#pragma endregion
	};

	enum class TURBO_PROP(Some enum) MyEnum
	{
		EVal1 TURBO_PROP(heyhey) = 1 << 0,
		EVal2 = 1 << 1,
		Count
	};
}