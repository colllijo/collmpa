#pragma once

#include <compare>
#include <cstdint>
#include <string>
#include <vector>

class Number
{
public:
	Number();
	Number(const std::string& value);

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	std::string toString() const;
	static Number fromString(const std::string& value);

private:
	std::vector<uint32_t> digits;
	bool negative;
};
