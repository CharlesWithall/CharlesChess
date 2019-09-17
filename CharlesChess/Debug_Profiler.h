#pragma once

#include <map>
#include <string>

class Debug_Profiler
{
public:
	static void StartProfiling(const std::string& aProfilingName);
	static void EndProfiling();
	static void QueryAverageTimes();
	static void QueryTotalTimes();
private:
	static std::string myProfilingName;
	static __int64 myCounterStart;
	static double myPCFrequency;

	static std::map<std::string, double> myAverageTimes;
	static std::map<std::string, int> myAverageInstances;
};

