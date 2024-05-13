#include <deque>

/**************************************************************************************************
Possible approaches:
    1. Arthur's original model, which involves 5 predictors as follows:
        A. k-period cycle detectors, where k = 1,2,5 - we chose random in this paper;
        B. Mirror image around 50% of last week's attendance;
        C. A fixed predictor;
        D. Rounded average of the last four weeks;
        E. Rounded and bound by 0 and N, last 8 weeks trend, computed by least squares method.
**************************************************************************************************/

class IStrategy 
{   
    public:
        virtual ~IStrategy() = 0;

        // Predict attendance based on the historical attendances in the series.
        // The historical series is of the form [last_week, two_weeks_ago, ...]
        virtual unsigned int predict(const std::deque<unsigned int>& attendanceHistory) = 0;
};

class ArthurStrategyA: public IStrategy
{
    int m_strategyScore;
    unsigned int m_lastPrediction;
    unsigned int m_population;
    unsigned int m_k;

    public:
    ~ArthurStrategyA() = default;
    ArthurStrategyA() = delete;
    explicit ArthurStrategyA(unsigned int population, unsigned int k);
    unsigned int predict(const std::deque<unsigned int>& attendanceHistory) override;
};

class ArthurStrategyB: public IStrategy
{
    int m_strategyScore;
    unsigned int m_lastPrediction;
    unsigned int m_population;

    public:
    ~ArthurStrategyB() = default;
    ArthurStrategyB() = delete;
    explicit ArthurStrategyB(unsigned int population);
    unsigned int predict(const std::deque<unsigned int>& attendanceHistory) override;
};

class ArthurStrategyC: public IStrategy
{
    int m_strategyScore;
    unsigned int m_lastPrediction;
    unsigned int m_population;

    public:
    ~ArthurStrategyC() = default;
    ArthurStrategyC() = delete;
    explicit ArthurStrategyC(unsigned int population);
    unsigned int predict(const std::deque<unsigned int>& attendanceHistory) override;
};

class ArthurStrategyD: public IStrategy
{
    int m_strategyScore;
    unsigned int m_lastPrediction;
    unsigned int m_population;

    public:
    ~ArthurStrategyD() = default;
    ArthurStrategyD() = delete;
    explicit ArthurStrategyD(unsigned int population);
    unsigned int predict(const std::deque<unsigned int>& attendanceHistory) override;
};

class ArthurStrategyE: public IStrategy
{
    int m_strategyScore;
    unsigned int m_lastPrediction;
    unsigned int m_population;

    public:
    ~ArthurStrategyE() = default;
    ArthurStrategyE() = delete;
    explicit ArthurStrategyE(unsigned int population);
    unsigned int predict(const std::deque<unsigned int>& attendanceHistory) override;
};

