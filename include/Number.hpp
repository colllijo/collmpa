#pragma once

#include <compare>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class Number
{
public:
	Number();
	Number(const std::string& value);

	Number operator+(const Number& other) const;
	Number operator-(const Number& other) const;

	Number& operator+=(const Number& other);
	Number& operator-=(const Number& other);

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	std::strong_ordering compareAbs(const Number& other) const;

	std::string toString() const;
	static Number fromString(const std::string& value);

	friend std::ostream& operator<<(std::ostream& os, const Number& number);

private:
	std::vector<uint32_t> digits;
	bool negative;

	std::vector<uint32_t> add(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> sub(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;

	void trimLeadingZeros();
};
