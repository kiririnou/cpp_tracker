// #define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>
#include <regex>

#include "utils.h"

#include <curl/curl.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include "resource_usage.h"

int main()
{
	std::cout << "Username: " << GetUsername() << std::endl;
	std::cout << "HWID: " << GetHWID() << std::endl;
	std::cout << "Memory in use: " << GetUsedMemory() << std::endl;

	// std::cout << "CPU: " << PrintCPUData(5) << std::endl;
	std::cout << "Current time: " << GetNow() << std::endl;

    //SetConsoleOutputCP(CP_UTF8);

	#pragma region CURL test
	// CURL* curl;
	// CURLcode res;
	// curl_global_init(CURL_GLOBAL_DEFAULT);

	// curl = curl_easy_init();
	// if (curl) 
	// {
	// 	curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/");
	// 	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	// 	curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	// 	curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

	// 	res = curl_easy_perform(curl);
	// 	if (res != CURLE_OK)
	// 		std::cerr << "Curl failed. Code: " << res << std::endl;
	// 	else 
	// 		std::cout << "Curl succeeded\n";

	// 	curl_easy_cleanup(curl);
	// }
	// curl_global_cleanup();
	#pragma endregion

	#pragma region Serialization
	using namespace rapidjson;

	Resource_Usage res("id", "2020-02-02 10:10:10", "2020-02-02 20:20:20", "Process", 40);

	StringBuffer buf;
	PrettyWriter<StringBuffer> writer(buf);

	writer.StartObject();
	res.Serialize(writer);
	writer.EndObject();

	std::puts(buf.GetString());
	#pragma endregion

	#pragma region CURL POST
	// CURL* curl;
	// CURLcode result;

	// curl = curl_easy_init();
	// curl_slist* slist = NULL;
	// slist = curl_slist_append(slist, "Content-Type: application/json");
	// curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.116:5000/api/process_activity");
	// curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf.GetString());
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(buf.GetLength()));

	// result = curl_easy_perform(curl);
	// if (result != CURLE_OK)
	// 	std::cerr << "Curl failed. Code: " << result << std::endl;
	// else
	// 	std::cout << "Curl succeedded. Code: " << result << std::endl;
	// curl_easy_cleanup(curl);
	#pragma endregion

	system("pause");
    return 0;
}
/*

{
	"hw_id": "$1",
	"start": "$2",
	"end": "$3",
	"process_title": "$4",
	"mem": $5
}

*/