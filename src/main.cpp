#include <iostream>
#include <string>
#include <memory>
#include <chrono>

#include <thread>

#include "utils.h"
#include "process_activity.h"

// const inline std::string url = Utils::ReadConfig("config.txt") + "/api/process_activity";

int main()
{
	using namespace Utils;
	using namespace std::chrono_literals;

	std::cout << "Addr: " << ReadConfig("config.txt") << std::endl;
	const std::string URL = ReadConfig("config.txt") + "/api/process_activity";

	ProcessActivity old;
	ProcessActivity current(GetHWID(), GetNow(), GetWindowName(), GetUsedMemory());

	old = current;

	while (true)
	{
		current.SetHWID(GetHWID());
		current.SetStartTime(GetNow());
		current.SetProcessTitle(GetWindowName());
		current.SetMemory(GetUsedMemory());

		if (current.GetProcessTitle().empty())
		{
			current = old;
			continue;
		}

		if (current.GetProcessTitle() != old.GetProcessTitle())
		{
			old.SetEndTime(current.GetStartTime());
			Post(URL, old);

			old = current;
		}
		std::this_thread::sleep_for(1s);
	}

	system("pause");
    return 0;
}