#pragma once

#define SomeMacro(Args)	//Args

class TestClass
{
	private:
		#pragma region Variables

		SomeMacro(Hey)
		int		var1 = 42;
		float	var2;

		#pragma endregion

	protected:

	public:
		#pragma region Methods

		[[noreturn]]
		int someMethod(int);

		#pragma endregion
};