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

class StrategyBase 
{   
    protected:
        int m_strategyScore;
        unsigned int m_lastPrediction;

        StrategyBase();

    public:
        virtual ~StrategyBase() = default;

        virtual int GetStrategyScore() const;
        virtual unsigned int GetPrediction() const;
        // Predict attendance based on the historical attendances in the series.
        // The historical series is of the form [last_week, two_weeks_ago, ...]
        virtual void predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population) = 0;
};

class ArthurStrategyA : public StrategyBase
{
    private:
        unsigned int m_k;

    public:
        ArthurStrategyA();
        void predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population) override;
};

class ArthurStrategyB : public StrategyBase
{
    public:
        ArthurStrategyB();
        void predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population) override;
};

class ArthurStrategyC : public StrategyBase
{   
    private:
        unsigned int m_fixedPredictor;

    public:
        ArthurStrategyC() = delete;
        explicit ArthurStrategyC(unsigned int population);
        void predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population) override;
};

class ArthurStrategyD : public StrategyBase
{
    public:
        ArthurStrategyD();
        void predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population) override;
};

class ArthurStrategyE : public StrategyBase
{
    public:
        ArthurStrategyE();
        void predict(const std::deque<unsigned int>& attendanceHistory, unsigned int population) override;
};

