#include "strategy.hxx"

#include "utils.hxx"
#include <cmath>

StrategyBase::StrategyBase() : m_strategyScore(0), m_lastPrediction(0)
{

}

inline int StrategyBase::GetStrategyScore() const
{
    return m_strategyScore;
}

inline unsigned int StrategyBase::GetPrediction() const
{
    return m_lastPrediction;
}
/**************************************************************************************************
Strategy A: k-period cycle detectors, where k = 1,2,5
**************************************************************************************************/
ArthurStrategyA::ArthurStrategyA() : StrategyBase()
{
    static constexpr int kValues[3] = { 1, 2, 5 };
    m_k = kValues[getRandomInteger(0, 2)];
}

void ArthurStrategyA::predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population)
{
    m_strategyScore += heavisideStep((m_lastPrediction - (unsigned int)std::round(population/3))*(attendanceHistory[0] - (unsigned int)std::round(population/3)));
    m_lastPrediction = *(attendanceHistory.end() - m_k);
}

/**************************************************************************************************
Strategy B: Mirror image around 50% of last week's attendance
**************************************************************************************************/
ArthurStrategyB::ArthurStrategyB() : StrategyBase()
{

}

void ArthurStrategyB::predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population)
{
    m_strategyScore += heavisideStep((m_lastPrediction - (unsigned int)std::round(population/3))*(attendanceHistory[0] - (unsigned int)std::round(population/3)));
    m_lastPrediction = (unsigned int)std::round(std::abs(0.5*population - attendanceHistory[0]));
}

/**************************************************************************************************
Strategy C: A fixed predictor
**************************************************************************************************/
ArthurStrategyC::ArthurStrategyC(unsigned int population) : StrategyBase()
{
    m_fixedPredictor = (unsigned int)std::round(population * getRandomInteger(1, 100) / 100); // x% of the population attended
}

void ArthurStrategyC::predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population)
{
    m_strategyScore += heavisideStep((m_fixedPredictor - (unsigned int)std::round(population/3))*(attendanceHistory[0] - (unsigned int)std::round(population/3)));
    m_lastPrediction = m_fixedPredictor;
}

/**************************************************************************************************
Strategy D: Rounded average of the last four weeks
**************************************************************************************************/
ArthurStrategyD::ArthurStrategyD() : StrategyBase()
{

}

void ArthurStrategyD::predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population)
{
    m_strategyScore += heavisideStep((m_lastPrediction - (unsigned int)std::round(population/3))*(attendanceHistory[0] - (unsigned int)std::round(population/3)));
    m_lastPrediction = (unsigned int)std::round((attendanceHistory[0] + attendanceHistory[1] + attendanceHistory[2] + attendanceHistory[3]) / 4);
}

/**************************************************************************************************
Strategy E: Rounded and bound by 0 and N, last 8 weeks trend, computed by least squares method
**************************************************************************************************/
ArthurStrategyE::ArthurStrategyE() : StrategyBase()
{

}

void ArthurStrategyE::predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population)
{
     // 8 weeks trend computation
    double a = 0.0;
    for (int i = 0; i < 8; ++i) {
        a + attendanceHistory[i];
    }

    a = a/8;
    double b = (1.*attendanceHistory[0]) + (3.*attendanceHistory[1]) + (5.*attendanceHistory[2]) + (7.*attendanceHistory[3]) +
               (-1.*attendanceHistory[4]) + (-3.*attendanceHistory[5]) + (-5.*attendanceHistory[6]) + (-7.*attendanceHistory[7]);
    constexpr double s = (1. + 9. + 25. + 49. + 1. + 9. + 25. + 49.);
    b = b / s;
    
    int prediction = std::round(a + (-9*b));
    if (prediction < 0) prediction = std::abs(prediction);
    if (prediction > population) prediction = population;

    m_strategyScore += heavisideStep((m_lastPrediction - (unsigned int)std::round(population/3))*(attendanceHistory[0] - (unsigned int)std::round(population/3)));
    m_lastPrediction = prediction;
}