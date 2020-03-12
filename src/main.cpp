#include <iostream>
#include <string>
#include <memory>
#include <chrono>

#include <thread>

#include "utils.h"
#include "process_activity.h"
#include "user.h"

// const inline std::string url = Utils::ReadConfig("config.txt") + "/api/process_activity";

int main()
{
	using namespace Utils;
	using namespace std::chrono_literals;

	std::cout << "Addr: " << ReadConfig("config.txt") << std::endl;
	// const std::string URL = ReadConfig("config.txt") + "/api/process_activity";
	User user(GetHWID(), GetUsername());

	const std::string URL = ReadConfig("config.txt");
	const std::string URL_Resource_Usage = URL + "/resource-usages/" + user.GetHWID();
	const std::string URL_Active_Windows = URL + "/active-windows/" + user.GetHWID();
	const std::string URL_Users = URL + "users";

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
			int res = Post(URL, old);

			if (res == 404)
			{
				res = Post(URL_Users, user);
				
			}

			old = current;
		}
		std::this_thread::sleep_for(1m);
	}

	system("pause");
    return 0;
}