#include <strategy.hxx>
#include <random>

namespace 
{
    inline unsigned int heavisideStep(int x)
    {
        return x >= 0 ? 1 : 0;
    }

    int getRandom()
    {
        static std::random_device randomDevice;
        static std::mt19937 engine(randomDevice());
        static std::uniform_int_distribution<std::mt19937::result_type> distribution(1,100);

        return distribution(engine);
    }
}

/**************************************************************************************************
Strategy A: k-period cycle detectors, where k = 1,2,5
**************************************************************************************************/
ArthurStrategyA::ArthurStrategyA(unsigned int population, unsigned int k) : m_strategyScore(0), m_lastPrediction(0)
{
    m_population = population;
    m_k = k;
}

unsigned int ArthurStrategyA::predict(const std::deque<unsigned int>& attendanceHistory)
{
    m_strategyScore += heavisideStep((m_lastPrediction - std::round(0.6*m_population))*(attendanceHistory[0] - std::round(0.6*m_population)));
    m_lastPrediction = attendanceHistory[m_k];
    return m_lastPrediction;
}

/**************************************************************************************************
Strategy B: Mirror image around 50% of last week's attendance
**************************************************************************************************/
ArthurStrategyB::ArthurStrategyB(unsigned int population) : m_strategyScore(0), m_lastPrediction(0)
{
    m_population = population;
}

unsigned int ArthurStrategyB::predict(const std::deque<unsigned int>& attendanceHistory)
{
    m_strategyScore += heavisideStep((m_lastPrediction - std::round(0.6*m_population))*(attendanceHistory[0] - std::round(0.6*m_population)));
    m_lastPrediction = std::abs(0.5*m_population - attendanceHistory[0]);
    return m_lastPrediction;
}

/**************************************************************************************************
Strategy C: A fixed predictor
**************************************************************************************************/
ArthurStrategyC::ArthurStrategyC(unsigned int population) : m_strategyScore(0), m_lastPrediction(0)
{
    m_population = population;
    m_lastPrediction = std::round(population * getRandom()/100);
}

unsigned int ArthurStrategyC::predict(const std::deque<unsigned int>& attendanceHistory)
{
    m_strategyScore += heavisideStep((m_lastPrediction - std::round(0.6*m_population))*(attendanceHistory[0] - std::round(0.6*m_population)));
    return m_lastPrediction;
}

/**************************************************************************************************
Strategy D: Rounded average of the last four weeks
**************************************************************************************************/
ArthurStrategyD::ArthurStrategyD(unsigned int population) : m_strategyScore(0), m_lastPrediction(0)
{
    m_population = population;
}

unsigned int ArthurStrategyD::predict(const std::deque<unsigned int>& attendanceHistory)
{
    m_strategyScore += heavisideStep((m_lastPrediction - std::round(0.6*m_population))*(attendanceHistory[0] - std::round(0.6*m_population)));
    m_lastPrediction = std::round((attendanceHistory[0] + attendanceHistory[1] + attendanceHistory[2] + attendanceHistory[3]) / 4);
    return m_lastPrediction;
}

/**************************************************************************************************
Strategy E: Rounded and bound by 0 and N, last 8 weeks trend, computed by least squares method
**************************************************************************************************/
ArthurStrategyE::ArthurStrategyE(unsigned int population) : m_strategyScore(0), m_lastPrediction(0)
{
    m_population = population;
}

unsigned int ArthurStrategyE::predict(const std::deque<unsigned int>& attendanceHistory)
{
     // 8 weeks trend computation
    double a = 0.0;
    std::for_each(attendanceHistory.cbegin(), attendanceHistory.cbegin() + 8, [&] (const int& attendance) { a+= attendance; });
    a = a/8;
    double b = (1*attendanceHistory[0]) + (3*attendanceHistory[1]) + (5*attendanceHistory[2]) + (7*attendanceHistory[3]) +
               (-1*attendanceHistory[4]) + (-3*attendanceHistory[5]) + (-5*attendanceHistory[6]) + (-7*attendanceHistory[7]);
    b = b / (1 + 9 + 25 + 49 + 1 + 9 + 25 + 49);
    
    int prediction = std::round(a + (-9*b));
    if (prediction < 0) prediction = std::abs(prediction);
    if (prediction > m_population) prediction = m_population;

    m_strategyScore += heavisideStep((m_lastPrediction - std::round(0.6*m_population))*(attendanceHistory[0] - std::round(0.6*m_population)));
    m_lastPrediction = prediction;
    return m_lastPrediction;
}