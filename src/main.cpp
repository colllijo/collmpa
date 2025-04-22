#include <iostream>

#include "Number.hpp"

int main(int, char **argv)
{
	Number a = Number::fromString(argv[1]);
	Number b = Number::fromString(argv[2]);

    auto result = a <=> b;
    std::cout << argv[1] << " <=> " << argv[2] << " : "
              << (result < 0 ? "less" : (result == 0 ? "equal" : "greater"))
              << "\n";

	std::cout << a.toString() << " <=> " << b.toString() << "\n";

	return 0;
}
