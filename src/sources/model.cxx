#include "model.hxx"

#include "person.hxx"
#include "utils.hxx"
#include <cmath>
#include <iostream>
#include <vector>

unsigned int Model::m_population = 0;

Model& Model::instance()
{
	static Model instance;
	return instance;
}

void Model::evaluate(unsigned int population, unsigned int iterations, double maxBiasFactor) const
{
	Model::m_population = population;

	std::vector<Person> persons; 
	persons.reserve(population);

	for (unsigned int i = 0; i < population; ++i)
	{
		persons.emplace_back(Person(getRandomReal(0., maxBiasFactor)));
	}

	std::deque<unsigned int> firstBarAttendanceHistory;
	std::deque<unsigned int> secondBarAttendanceHistory;

	// generate initial attendance
	for (int i = 0; i < 8; ++i)
	{
		firstBarAttendanceHistory.emplace_back(getRandomInteger(1, std::round(population)));
		secondBarAttendanceHistory.emplace_back(getRandomInteger(1, std::round(population)));
	}

	for (unsigned int i = 0; i < iterations; ++i)
	{
		unsigned int firstBarAttendance = 0;
		unsigned int secondBarAttendance = 0;
		for (const auto& p : persons)
		{
			p.attend(firstBarAttendanceHistory, secondBarAttendanceHistory, firstBarAttendance, secondBarAttendance);
		}
		firstBarAttendanceHistory.push_back(firstBarAttendance);
		secondBarAttendanceHistory.push_back(secondBarAttendance);
	}

	for (int i = 0; i < firstBarAttendanceHistory.size(); ++i) 
	{
		std::cout << firstBarAttendanceHistory[i] << " " << secondBarAttendanceHistory[i] << std::endl;
	}
}

