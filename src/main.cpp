#include <iostream>
#include <string>
#include <memory>
#include <chrono>

#include <thread>

#include "utils.h"
#include "user.h"
#include "active_window.h"

int main()
{
	using namespace Utils;
	using namespace std::chrono_literals;
	using p_ActiveWindow = std::unique_ptr<ActiveWindow>;

	std::cout << "Addr: " << ReadConfig("config.txt") << std::endl;
	static const User user(GetHWID(), GetUsername());

	const std::string URL = ReadConfig("config.txt");
	const std::string URL_Resource_Usage = URL + "/api/v1/resource-usages/" + user.GetHWID();
	const std::string URL_Active_Windows = URL + "/api/v1/active-windows/" + user.GetHWID();
	const std::string URL_Users = URL + "/api/v1/users";

	p_ActiveWindow p_current = std::make_unique<ActiveWindow>(GetWindowName(), GetNow());
	p_ActiveWindow p_last = p_current->Copy();

	try
	{
		while (true)
		{
			p_current = std::make_unique<ActiveWindow>(GetWindowName(), GetNow());

			if (p_current->GetTitle().empty())
			{
				p_current.release();
				p_current = p_last->Copy();
				continue;
			}

			if (p_current->GetTitle() != p_last->GetTitle())
			{
				p_last->SetEndTime(p_current->GetStartTime());
				switch (Post<ActiveWindow>(URL_Active_Windows, *p_last))
				{
				case 201:
					std::cout << "201 Window's activity period succesfully added\n";
					break;
				case 400:
					std::cout << "400 Malformed request parameters\n";
					break;
				case 404:
				{
					std::cout << "404 A user with the specified hardware ID was not found";
					Post<User>(URL_Users, user);
					Post<ActiveWindow>(URL_Active_Windows, *p_last);
				}
					break;
				default:
					std::cout << "Unknown case. Contact the developer";
					break;
				}

				p_last = p_current->Copy();
			}
			std::this_thread::sleep_for(1s);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	system("pause");
    return 0;
}