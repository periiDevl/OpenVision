#ifndef UUID_HEADER
#define UUID_HEADER

#include <cstdint>
#include <random>

using UUID = uint64_t;

struct UUIDManager
{
	static UUID randomUUID()
	{
		// https://stackoverflow.com/questions/22883840/c-get-random-number-from-0-to-max-long-long-integer
		std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
		std::mt19937_64 eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator

		// Define the distribution, by default it goes from 0 to MAX(uint64_t )
		std::uniform_int_distribution<uint64_t> distr;

		// Generate random number
		return distr(eng);
	}

	template<typename T>
	static UUID getUUID()
	{
		static UUID result = randomUUID();

		return result;
	}
};

#endif