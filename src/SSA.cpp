#include "SSA.hpp"

#include <stdexcept>
#include <tuple>
#include <utility>

ModRing::ModRing(Limb k) : k(k), mod((Limb(1) << k) + 1), mask((Limb(1) << k) - 1) {}

ModRing::Limb ModRing::reduce(Limb x) const
{
	Limb low = x & mask;
	Limb high = x >> k;
	Limb res = low - high;

	if (int64_t(res) < 0) res += mod;
	return res;
}

ModRing::Limb ModRing::add(Limb a, Limb b) const
{
	Limb res = a + b;
	if (res >= mod) res -= mod;
	return res;
}

ModRing::Limb ModRing::sub(Limb a, Limb b) const
{
	Limb res = a - b;
	if (int64_t(res) < 0) res += mod;
	return res;
}

ModRing::Limb ModRing::mul(Limb a, Limb b) const
{
	uint64_t res = a * b;
	return reduce(res);
}

std::vector<uint64_t> computePowers(uint64_t w, size_t n, const ModRing& ring)
{
	std::vector<uint64_t> powers(n);
	powers[0] = 1;

	for (size_t i = 1; i < n; ++i) powers[i] = ring.mul(powers[i - 1], w);

	return powers;
}

void bitReverse(std::vector<uint64_t>& a)
{
	size_t n = a.size();
	size_t logn = __builtin_ctzll(n);

	for (size_t i = 0; i < n; ++i)
	{
		size_t rev = __builtin_bitreverse64(i) >> (64 - logn);
		if (i < rev) std::swap(a[i], a[rev]);
	}
}

void ntt(std::vector<uint64_t>& a, const ModRing& ring, uint64_t w)
{
	size_t n = a.size();
	auto roots = computePowers(w, n, ring);
	bitReverse(a);

	for (size_t len = 2; len <= n; len <<= 1)
	{
		size_t half = len >> 1;
		size_t step = n / len;

		for (size_t i = 0; i < n; i += len)
		{
			for (size_t j = 0; j < half; ++j)
			{
				uint64_t u = a[i + j];
				uint64_t v = ring.mul(a[i + j + half], roots[j * step]);

				a[i + j] = ring.add(u, v);
				a[i + j + half] = ring.sub(u, v);
			}
		}
	}
}

void intt(std::vector<uint64_t>& a, const ModRing& ring, uint64_t w)
{
	size_t n = a.size();
	ntt(a, ring, w);

	uint64_t n_inv = modInverse(n, ring.mod);
	for (auto& x : a) x = ring.mul(x, n_inv);
}

uint64_t modInverse(uint64_t a, uint64_t m)
{
	uint64_t t = 0, newT = 1;
	uint64_t r = m, newR = a;

	while (newR != 0)
	{
		int64_t q = r / newR;
		std::tie(t, newT) = std::make_pair(newT, t - q * newT);
		std::tie(r, newR) = std::make_pair(newR, r - q * newR);
	}

	if (r > 1) throw std::runtime_error("a is not invertible");
	if (t < 0) t += m;

	return t;
}
