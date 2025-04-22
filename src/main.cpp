#include <iostream>

#include "Number.hpp"

int main(int, char **argv)
{
	Number a = Number::fromString(argv[1]);
	Number b = Number::fromString(argv[2]);

    auto result = a - b;
    std::cout << argv[1] << " - " << argv[2] << " = " << result.toString() << "\n";

	return 0;
}
