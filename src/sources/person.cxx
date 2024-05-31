#include "person.hxx"

#include "model.hxx"
#include "strategy.hxx"
#include "utils.hxx"

#include <cassert>
#include <random>

namespace
{
	double computeRandomBias(double biasFactor)
	{
		// All these values are expressed on a scale from -biasFactor...biasFactor,
		// where we use biasFactor to denote the % of the total population
		// that we use as bias, with sign meaning:
		//		----->  (+) positive: I favour the first bar
		//		----->  (-) negative: I favour the second bar 
		// Example: 
		//	For a biasFactor = 5, suppose we obtain a totalBias = -13.5.
		//  This translates to adding 13.5%*population
		//  to the prediction of the first bar (trying to invalidate it)
		//  and subtracting 13.5%*population to the prediction of the
		//	second bar.
		double myMusicBias = getRandomReal(-biasFactor, biasFactor);
		double myLocationBias = getRandomReal(-biasFactor, biasFactor);
		double myFoodBias = getRandomReal(-biasFactor, biasFactor);
		double myDrinksBias = getRandomReal(-biasFactor, biasFactor);
		double myPersonalBias = getRandomReal(-biasFactor, biasFactor);

		double totalBias = myMusicBias + myLocationBias + myFoodBias + myDrinksBias + myPersonalBias;
		return totalBias;
	}
}

Person::Person(double biasFactor)
{
	m_bias = computeRandomBias(biasFactor) * Model::m_population / 100;

	m_firstBarStrategies.push_back(new ArthurStrategyA());
	m_firstBarStrategies.push_back(new ArthurStrategyB());
	m_firstBarStrategies.push_back(new ArthurStrategyC(Model::m_population));
	m_firstBarStrategies.push_back(new ArthurStrategyD());
	m_firstBarStrategies.push_back(new ArthurStrategyE());

	m_secondBarStrategies.push_back(new ArthurStrategyA());
	m_secondBarStrategies.push_back(new ArthurStrategyB());
	m_secondBarStrategies.push_back(new ArthurStrategyC(Model::m_population));
	m_secondBarStrategies.push_back(new ArthurStrategyD());
	m_secondBarStrategies.push_back(new ArthurStrategyE());
}

Person::Person(Person&& p) noexcept
{
	m_bias = p.m_bias;
	m_firstBarStrategies = std::move(p.m_firstBarStrategies);
	m_secondBarStrategies = std::move(p.m_secondBarStrategies);
}

Person::~Person()
{
	for (auto& strategy : m_firstBarStrategies) delete strategy;
	for (auto& strategy : m_secondBarStrategies) delete strategy;

	m_firstBarStrategies.clear();
	m_secondBarStrategies.clear();
}

void Person::attend(const std::deque<unsigned int>& firstBarAttendanceHistory, const std::deque<unsigned int>& secondBarAttendanceHistory,
	unsigned int& firstBarAttendance, unsigned int& secondBarAttendance) const
{
	const auto& population = Model::m_population;

	auto predictBar = [&](const std::vector<StrategyBase*>& strategies, const std::deque<unsigned int>& attendanceHistory, double bias)
	{
		StrategyBase* bestStrategy = strategies[0];
		int bestStrategyScore = 0;
		for (auto& strategy : strategies) 
		{
			strategy->predict(attendanceHistory, population);
			int score = strategy->GetStrategyScore();
			if (score > bestStrategyScore && strategy->GetPrediction() < bestStrategy->GetPrediction())
			{
				bestStrategyScore = score;
				bestStrategy = strategy;
			}
		}
		
		// Decide upon going or not to the bar
		unsigned int prediction = (unsigned int)std::round(bestStrategy->GetPrediction() + bias);
		return heavisideStep((unsigned int)std::round(population/3) - prediction);
	};

	if (m_bias >= 0) // positive bias means I favour the first bar, so I try to predict its attendance first
	{
		if (predictBar(m_firstBarStrategies, firstBarAttendanceHistory, -1.*m_bias))
		{
			// I go to my favorite bar!
			++firstBarAttendance;
		}
		else if (predictBar(m_secondBarStrategies, secondBarAttendanceHistory, m_bias))
		{
			// My favorite bar (first one) was too crowded, I go to the second one...
			++secondBarAttendance;
		}
		// else 		
		// Both bars were too crowded, I stay home.
	}
	else // negative bias means I favor the second bar, so I try to predict its attendance first
	{
		if (predictBar(m_secondBarStrategies, secondBarAttendanceHistory, m_bias)) 
		{
			// I go to my favorite bar!
			++secondBarAttendance;
		}
		else if (predictBar(m_firstBarStrategies, firstBarAttendanceHistory, -1.*m_bias))
		{
			// My favorite bar (second one) was too crowded, I go to the first one...
			++firstBarAttendance;
		}
		// else 		
		// Both bars were too crowded, I stay home.
	}
}