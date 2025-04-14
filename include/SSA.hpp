#pragma once

#include <cstdint>
#include <vector>

struct ModRing
{
	using Limb = uint64_t;

	Limb k;
	Limb mod;
	Limb mask;

	ModRing(Limb k);

	Limb reduce(Limb x) const;
	Limb add(Limb a, Limb b) const;
	Limb sub(Limb a, Limb b) const;
	Limb mul(Limb a, Limb b) const;
};

std::vector<uint64_t> computePowers(uint64_t base, size_t n, const ModRing& ring);

void bitReverse(std::vector<uint64_t>& a);

void ntt(std::vector<uint64_t>& a, const ModRing& ring, uint64_t w);

void intt(std::vector<uint64_t>& a, const ModRing& ring, uint64_t w);

uint64_t modInverse(uint64_t a, uint64_t m);

std::vector<uint64_t> convolution(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b, const ModRing& ring, uint64_t root, uint64_t rootInv);
