#include "Number.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "SSA.hpp"

Number::Number() = default;
Number::Number(const std::string& str)
{
	*this = fromString(str);
}

Number Number::operator+(const Number& other) const
{
	Number result = *this;
	result += other;

	return result;
}

Number& Number::operator+=(const Number& other)
{
	add(other);

	return *this;
}

void Number::add(const Number& other)
{
	if (negative == other.negative)
	{
		limbs = addLimbs(limbs, other.limbs);
	}
	else
	{
		int cmp = compareAbs(*this, other);

		if (cmp == 0)
		{
			limbs.clear();
			negative = false;
		}
		else if (cmp > 0)
		{
			limbs = subLimbs(limbs, other.limbs);
		}
		else
		{
			limbs = subLimbs(other.limbs, limbs);
			negative = other.negative;
		}
	}
}

std::vector<uint32_t> Number::addLimbs(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b)
{
	std::vector<uint32_t> result;
	result.reserve(std::max(a.size(), b.size()) + 1);

	uint64_t carry = 0;
	for (size_t i = 0; i < a.size() || i < b.size() || carry; ++i)
	{
		uint64_t sum = carry;
		if (i < a.size()) sum += a[i];
		if (i < b.size()) sum += b[i];

		result.push_back(uint32_t(sum & 0xFFFFFFFF));
		carry = sum >> 32;
	}

	while (!result.empty() && result.back() == 0) result.pop_back();

	return result;
}

Number Number::operator-(const Number& other) const
{
	Number result = *this;
	result -= other;

	return result;
}

Number& Number::operator-=(const Number& other)
{
	sub(other);

	return *this;
}

void Number::sub(const Number& other)
{
	if (negative != other.negative)
	{
		limbs = addLimbs(limbs, other.limbs);
	}
	else
	{
		int cmp = compareAbs(*this, other);
		if (cmp == 0)
		{
			limbs.clear();
			negative = false;
		}
		else if (cmp > 0)
		{
			limbs = subLimbs(limbs, other.limbs);
		}
		else
		{
			limbs = subLimbs(other.limbs, limbs);
			negative = !other.negative;
		}
	}
}

std::vector<uint32_t> Number::subLimbs(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b)
{
	std::vector<uint32_t> result;
	result.reserve(a.size());

	uint64_t borrow = 0;
	for (size_t i = 0; i < a.size() || i < b.size(); ++i)
	{
		uint64_t diff = borrow;
		if (i < a.size()) diff += a[i];
		if (i < b.size()) diff -= b[i];

		if (diff > 0xFFFFFFFF)
		{
			borrow = 1;
			diff += 0x100000000;
		}
		else
		{
			borrow = 0;
		}

		result.push_back(uint32_t(diff & 0xFFFFFFFF));
	}

	while (!result.empty() && result.back() == 0) result.pop_back();

	return result;
}

Number Number::operator*(const Number& other) const
{
	Number result = *this;
	result *= other;

	return result;
}

Number& Number::operator*=(const Number& other)
{
	mul(other);

	return *this;
}

void Number::mul(const Number& other)
{
	std::cout << "Multipling" << std::endl;

	static const int k = 32;
	static const ModRing ring(k);
	static const uint64_t root = 3;
	static const uint64_t rootInv = modInverse(root, ring.mod);

	std::vector<uint64_t> product = convolution(limbs, other.limbs, ring, root, rootInv);

	limbs.clear();
	uint64_t carry = 0;
	for (uint64_t value: product)
	{
		value += carry;
		limbs.push_back(uint32_t(value & 0xFFFFFFFF));
		carry = value >> 32;
	}
	if (carry) limbs.push_back(uint32_t(carry));

	while (!limbs.empty() && limbs.back() == 0) limbs.pop_back();
	negative = negative != other.negative;
}

bool Number::operator==(const Number& other) const
{
	if (negative != other.negative) return false;
	if (limbs.size() != other.limbs.size()) return false;

	for (size_t i = 0; i < limbs.size(); ++i)
	{
		if (limbs[i] != other.limbs[i]) return false;
	}

	return true;
}

std::strong_ordering Number::operator<=>(const Number& other) const
{
	if (this == &other) return std::strong_ordering::equal;

	int cmp = compare(*this, other);
	if (cmp < 0) return std::strong_ordering::less;
	if (cmp > 0) return std::strong_ordering::greater;

	return std::strong_ordering::equal;
}

int Number::compare(const Number& a, const Number& b)
{
	if (a.negative != b.negative) return a.negative ? -1 : 1;

	int cmp = compareAbs(a, b);
	return a.negative ? -cmp : cmp;
}

int Number::compareAbs(const Number& a, const Number& b)
{
	if (a.limbs.size() != b.limbs.size()) return a.limbs.size() < b.limbs.size() ? -1 : 1;

	for (size_t i = a.limbs.size(); i-- > 0;)
	{
		if (a.limbs[i] != b.limbs[i]) return a.limbs[i] < b.limbs[i] ? -1 : 1;
	}

	return 0;
}

Number Number::fromString(const std::string& str)
{
	Number number;

	number.limbs.clear();
	number.negative = false;

	size_t i = 0;
	if (str[i] == '-')
	{
		number.negative = true;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}

	number.limbs.push_back(0);

	for (; i < str.size(); ++i)
	{
		char c = str[i];
		if (c < '0' || c > '9') throw std::invalid_argument("Invalid character in number string");

		int digit = c - '0';

		uint64_t carry = digit;
		for (size_t j = 0; j < number.limbs.size(); ++j)
		{
			uint64_t tmp = uint64_t(number.limbs[j]) * 10 + carry;
			number.limbs[j] = uint32_t(tmp & 0xFFFFFFFF);
			carry = tmp >> 32;
		}

		if (carry) number.limbs.push_back(uint32_t(carry));
	}

	return number;
}

std::string Number::toString() const
{
	if (limbs.empty()) return "0";

	Number tmp = *this;
	tmp.negative = false;

	std::string result;
	while (!tmp.limbs.empty())
	{
		uint64_t remainder = 0;
		for (size_t i = tmp.limbs.size(); i-- > 0;)
		{
			uint64_t value = (remainder << 32) | tmp.limbs[i];
			tmp.limbs[i] = uint32_t(value / 10);
			remainder = value % 10;
		}

		result.insert(result.begin(), '0' + char(remainder));

		while (!tmp.limbs.empty() && tmp.limbs.back() == 0) tmp.limbs.pop_back();
	}

	if (negative) result.insert(result.begin(), '-');

	return result;
}

std::ostream& operator<<(std::ostream& os, const Number& number)
{
	os << number.toString();
	return os;
}
