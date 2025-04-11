#include <iostream>
#include <sstream>
#include <string>

#include "Number.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::string calculation(argv[1]);
		std::istringstream stream(calculation);
		std::string token;
		std::vector<std::string> substrings;

		while (std::getline(stream, token, ' '))
		{
			substrings.push_back(token);
		}

		if ((substrings.size() & 0b1) == 0) return 1;

		Number result(substrings[0]);

		for (size_t i = 1; i < substrings.size(); i += 2)
		{
			std::string op = substrings[i];
			Number next(substrings[i + 1]);

			if (op == "+")
			{
				result += next;
			}
			else if (op == "-")
			{
				result -= next;
			}
			else
			{
				std::cerr << "Invalid operator: " << op << std::endl;
				return 1;
			}
		}

		std::cout << result << std::endl;
	}

	return 0;
}
