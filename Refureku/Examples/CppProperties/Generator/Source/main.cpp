#include <iostream>

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		std::cout << "WORKING DIRECTORY IS: " << argv[1] << std::endl;
	}

	return EXIT_SUCCESS;
}