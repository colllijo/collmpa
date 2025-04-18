#pragma once

#include <compare>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class Number
{
public:
	explicit Number();
	explicit Number(const std::string& str);

	static Number fromString(const std::string& str);
	std::string toString() const;

	Number operator+(const Number& other) const;
	Number& operator+=(const Number& other);

	Number operator-(const Number& other) const;
	Number& operator-=(const Number& other);

	Number operator*(const Number& other) const;
	Number& operator*=(const Number& other);

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Number& number);

private:
	std::vector<uint32_t> limbs;
	bool negative;

	void add(const Number& other);
	static std::vector<uint32_t> addLimbs(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b);

	void sub(const Number& other);
	static std::vector<uint32_t> subLimbs(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b);

	void mul(const Number& other);

	static int compare(const Number& a, const Number& b);
	static int compareAbs(const Number& a, const Number& b);
};
