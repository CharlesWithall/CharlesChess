#include "Debug_Profiler.h"

#include <iostream>
#include <Windows.h>

std::string Debug_Profiler::myProfilingName = "";
__int64 Debug_Profiler::myCounterStart = 0;
double Debug_Profiler::myPCFrequency = 0.0;

std::map<std::string, double> Debug_Profiler::myAverageTimes = std::map<std::string, double>();
std::map<std::string, int> Debug_Profiler::myAverageInstances = std::map<std::string, int>();

void Debug_Profiler::StartProfiling(const std::string& aProfilingName)
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!" << std::endl;

	myPCFrequency = double(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	myCounterStart = li.QuadPart;

	myProfilingName = aProfilingName;
}

void Debug_Profiler::EndProfiling()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	const double profilingTime = double(li.QuadPart - myCounterStart) / myPCFrequency;
	if (profilingTime > 1000)
	{
		//std::cout << myProfilingName << " took " << profilingTime/1000 << "s" << std::endl;
	}
	else
	{
		//std::cout << myProfilingName << " took " << profilingTime << "ns" << std::endl;
	}
	
	if (myAverageInstances.find(myProfilingName) == myAverageInstances.end())
	{
		myAverageInstances.insert({ myProfilingName, 0 });
		myAverageTimes.insert({ myProfilingName, 0.0 });
	}

	myAverageInstances.at(myProfilingName)++;
	const int instanceCount = myAverageInstances.at(myProfilingName);
	myAverageTimes.at(myProfilingName) = (myAverageTimes.at(myProfilingName) * (instanceCount - 1) + profilingTime) / instanceCount;

	myProfilingName = "";
}

void Debug_Profiler::QueryAverageTimes()
{
	std::cout << "///////////////////////////////////////////" << std::endl;
	std::cout << "--- Query Average Times ---" << std::endl;
	std::cout << "///////////////////////////////////////////" << std::endl;
	for (std::map<std::string, double>::iterator itr = myAverageTimes.begin(); itr != myAverageTimes.end(); ++itr)
	{
		std::cout << itr->first << " = " << itr->second << "ns" << std::endl;
	}
	std::cout << "///////////////////////////////////////////" << std::endl;
}

void Debug_Profiler::QueryTotalTimes()
{
	std::cout << "///////////////////////////////////////////" << std::endl;
	std::cout << "--- Query Total Times ---" << std::endl;
	std::cout << "///////////////////////////////////////////" << std::endl;
	for (std::map<std::string, double>::iterator itr = myAverageTimes.begin(); itr != myAverageTimes.end(); ++itr)
	{
		std::cout << itr->first << " = " << itr->second * (double)myAverageInstances.at(itr->first) * 0.001 << "s" << std::endl;
	}
	std::cout << "///////////////////////////////////////////" << std::endl;
}