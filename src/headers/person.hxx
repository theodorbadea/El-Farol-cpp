
#include <deque>
#include <vector>

class StrategyBase;

class Person
{
	friend class Model;

	public:
		Person() = delete;
		Person(Person&& p) noexcept;
		~Person();
		explicit Person(double biasFactor);

		void attend(const std::deque<unsigned int>& firstBarAttendanceHistory, const std::deque<unsigned int>& secondBarAttendanceHistory,
			unsigned int& firstBarAttendance, unsigned int& secondBarAttendance) const;

	private:
		// Bias my choice towards a bar by modifying my prediction 
		// with the bias value. The sign of m_bias gives my choice of bar:
		//		----->  (+) positive: I favour the first bar
		//		----->  (-) negative: I favour the second bar 
		double m_bias;

		std::vector<StrategyBase*> m_firstBarStrategies;
		std::vector<StrategyBase*> m_secondBarStrategies;
};