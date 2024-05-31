
#include <deque>

class Model
{
	friend class Person;

	public:
		static Model& instance();

		Model(const Model&) = delete;
		Model& operator = (const Model&) = delete;
	
		static std::deque<unsigned int> m_firstBarAttendanceHistory;
		static std::deque<unsigned int> m_secondBarAttendanceHistory;
	private:
		Model() = default;
		~Model() = default;

		static unsigned int m_population;
		// maximum proportion of the population that can go to a bar and still have fun

	public:
		void evaluate(unsigned int population, unsigned int iterations, double maxBiasFactor=2.) const;
};