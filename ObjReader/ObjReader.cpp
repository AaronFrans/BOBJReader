#include <iostream>
#include "Converter.h"

int main()
{
	try
	{
		Converter::ConvertToBOBJ("Resources/low poly stanford bunny.obj");
		Converter::ConvertToOBJ();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}