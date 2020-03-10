#include <iostream>
#include <string>
#include <memory>
#include <chrono>

#include "mingw.thread.h"

#include "utils.h"
#include "process_activity.h"

int main()
{
	using namespace Utils;

	std::string url = "http://192.168.1.116:5000/api/process_activity";
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
			Post(url, old);

			old = current;
		}
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(500ms);
	}

	system("pause");
    return 0;
}