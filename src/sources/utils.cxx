#include "utils.hxx"

#include <random>

int getRandomInteger(int min, int max)
{
	static std::random_device randomDevice;
	static std::mt19937 engine(randomDevice());
	std::uniform_int_distribution<std::mt19937::result_type> intDistribution(min, max);
	
	return intDistribution(engine);
}

double getRandomReal(double min, double max)
{
	static std::random_device randomDevice;
	static std::mt19937 engine(randomDevice());
	std::uniform_real_distribution<double> realDistribution(min, max);

	return realDistribution(engine);
}