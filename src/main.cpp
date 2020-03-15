#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <chrono>

#include <thread>

#include "utils.h"
#include "system_info.h"

#include "user.h"
#include "active_window.h"
#include "resource_usage.h"

#include "logger.h"

static const User user(system_info::GetHWID(), system_info::GetUsername());

void CheckUser(const std::string& url);
void MonitorWindows(const std::string& url);
void MonitorResources(const std::string& url);

int main()
{

	const std::string URL = utils::ReadConfig("config.txt");
	const std::string URL_Resource_Usage = URL + "/api/v1/resource-usages/" + user.GetHWID();
	const std::string URL_Active_Windows = URL + "/api/v1/active-windows/" + user.GetHWID();
	const std::string URL_Users = URL + "/api/v1/users";
	
	CheckUser(URL_Users);

	std::thread th_mw(MonitorWindows, URL_Active_Windows), th_mr(MonitorResources, URL_Resource_Usage);

	th_mw.join();
	th_mr.join();

	system("pause");
    return 0;
}

void CheckUser(const std::string& url)
{
	logger log("CheckUser");
	log.info("Thread id: " + utils::GetThreadId());

	try
	{
		switch (utils::Post<User>(url, user))
		{
		case 200:
			log.info("200 Successfully created new user");
			break;
		case 400:
			log.error("400 Malformed request parameters");
			break;
		case 409:
			log.info("409 User already exists");
		default:
			break;
		}
	}
	catch(const std::exception& e)
	{
		log.critical(e.what());
	}
	
}

void MonitorWindows(const std::string& url)
{
	using namespace std::chrono_literals;
	using p_ActiveWindow = std::unique_ptr<ActiveWindow>;

	logger log("MonitorWindows");
	log.info("Thread id: " + utils::GetThreadId());

	try
	{
		p_ActiveWindow current = std::make_unique<ActiveWindow>(system_info::GetWindowName(), system_info::GetNow());
		p_ActiveWindow last = current->Copy();

		while (true)
		{
			current = std::make_unique<ActiveWindow>(system_info::GetWindowName(), system_info::GetNow());

			if (current->GetTitle().empty())
			{
				current.release();
				current = last->Copy();
				continue;
			}

			if (current->GetTitle() != last->GetTitle())
			{
				last->SetEndTime(current->GetStartTime());

				switch (utils::Post<ActiveWindow>(url, *last))
				{
				case 201:
					log.info("201 Window's activity period succesfully added");
					break;
				case 400:
					log.error("400 Malformed request parameters");
					break;
				case 404:
					log.error("404 A user with the specified hardware ID was not found");
					break;
				default:
					log.error("Unknown code");
					break;
				}

				last.release();
				last = current->Copy();
			}
			std::this_thread::sleep_for(1s);
		}
	}
	catch(const std::exception& e)
	{
		log.critical(e.what());
	}
	
}

void MonitorResources(const std::string& url)
{
	using namespace std::chrono_literals;
	using p_ResourceUsage = std::unique_ptr<ResourceUsage>;

	logger log("MonitorResources");
	log.info("Thread id: " + utils::GetThreadId());

	try
	{
		p_ResourceUsage resUsage = nullptr;

		while (true)
		{
			resUsage = std::make_unique<ResourceUsage>(system_info::GetNow(), system_info::GetCPUData(3), system_info::GetUsedMemory());

			switch (utils::Post<ResourceUsage>(url, *resUsage))
			{
			case 201:
				log.info("201 Resource usage created successfully");
				break;
			case 400:
				log.error("400 Malformed request parameters");
				break;
			case 404:
				log.error("404 A user with the specified hardware ID was not found");
				break;
			default:
				log.error("Unknown code");
				break;
			}
			resUsage.release();
			std::this_thread::sleep_for(1min);
		}
	}
	catch(const std::exception& e)
	{
		log.critical(e.what());
	}
}