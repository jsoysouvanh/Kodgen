#pragma once

#ifdef PARSER
#define TURBO_PROP(...)	__attribute__((annotate(#__VA_ARGS__)))
#else
#define TURBO_PROP(...)	
#endif

template <typename MyAwesomeType>
class TestClass final
{
	private:
		#pragma region Variables

		int				var1 = 42;
		float			var2;
		MyAwesomeType*	var3 = nullptr;
		class Hey*		var4 = nullptr;

		#pragma endregion

	protected:

	public:
		#pragma region Methods

		TURBO_PROP(Coucou=ablblaa, azdpjad, azdiohzad)
		int someMethod(int);
		int dsomeMethod3(int) noexcept;
		//int someMethosd3(int) override;
		virtual int someMethodd3(int) const;

		#pragma endregion
};