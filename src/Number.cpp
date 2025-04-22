#include "Number.hpp"

#include <algorithm>
#include <cmath>
#include <compare>
#include <cstdint>
#include <stdexcept>
#include <tuple>

// Internal functions
namespace
{
	std::tuple<std::string, uint64_t> divideByBase(const std::string& value, uint64_t base);
};

Number::Number() : digits(0), negative(false) {}
Number::Number(const std::string& value)
{
	*this = fromString(value);
}

bool Number::operator==(const Number& other) const
{
	if (negative != other.negative) return false;
	if (digits.size() != other.digits.size()) return false;

	for (size_t i = 0; i < digits.size(); ++i)
	{
		if (digits.at(i) != other.digits.at(i)) return false;
	}

	return true;
}

std::strong_ordering Number::operator<=>(const Number& other) const
{
	if (*this == other) return std::strong_ordering::equal;
	if (negative != other.negative) return negative ? std::strong_ordering::less : std::strong_ordering::greater;

	bool greater = false;
	if (digits.size() != other.digits.size())
	{
		greater = digits.size() > other.digits.size();
	}
	else
	{
		for (size_t i = digits.size(); i-- > 0;)
		{
			if (digits.at(i) != other.digits.at(i))
			{
				greater = digits.at(i) > other.digits.at(i);
				break;
			}
		}
	}

	if (negative) greater = !greater;

	if (greater) return std::strong_ordering::greater;
	else return std::strong_ordering::less;
}

std::string Number::toString() const
{
	if (digits.empty()) return "0";

	std::string result;
	Number tmp = *this;

	do
	{
		uint64_t carry = 0;

		for (size_t i = tmp.digits.size(); i-- > 0;)
		{
			carry = (carry << 32) | tmp.digits.at(i);
			tmp.digits[i] = static_cast<uint32_t>((carry / 10) % 0xFFFFFFFF);
			carry %= 10;
		}

		result += static_cast<char>(carry + '0');

		while (!tmp.digits.empty() && tmp.digits.back() == 0) tmp.digits.pop_back();
	} while (!tmp.digits.empty());

	if (negative) result += '-';
	std::reverse(result.begin(), result.end());

	return result;
}

Number Number::fromString(const std::string& value)
{
	const uint64_t base = (1ULL << 32);

	if (value.empty()) throw std::invalid_argument("Value is required to create a number.");

	std::string represenation = value;
	Number result;

	if (represenation.at(0) == '+' || represenation.at(0) == '-')
	{
		result.negative = represenation.at(0) == '-';
		represenation = represenation.substr(1);

		// Check that there actually is a number not just a sign.
		if (represenation.empty()) throw std::invalid_argument("Value is required to create a number.");
	}

	do
	{
		auto [quotient, remainder] = divideByBase(represenation, base);

		represenation = quotient;
		result.digits.push_back(static_cast<uint32_t>(remainder));
	} while (!represenation.empty());

	return result;
}

namespace
{
	std::tuple<std::string, uint64_t> divideByBase(const std::string& value, uint64_t base)
	{
		std::string quotient = "";
		uint64_t carry = 0;

		for (size_t i = 0; i < value.size(); ++i)
		{
			if (value.at(i) < '0' || value.at(i) > '9') throw std::invalid_argument("Number may only use digits 0-9.");

			carry = carry * 10 + value.at(i) - '0';

			quotient += static_cast<char>((carry / base) + '0');
			carry %= base;
		}

		while (!quotient.empty() && quotient.at(0) == '0') quotient = quotient.substr(1);

		return {quotient, carry};
	}
}  // namespace
