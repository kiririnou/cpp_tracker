#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <codecvt>
#include <locale>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <exception>
#include <vector>
#include <fstream>

#include <thread>

#include <curl/curl.h>

#include "rapidjson/prettywriter.h"

namespace utils {

	std::string ConvertUTF16ToUTF8(LPWSTR lpwstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(std::wstring(lpwstr));
	}

	std::string ConvertUTF16ToUTF8(std::wstring wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(wstr);
	}

	template <class T>
	std::string Jsonify(T res)
	{
		using namespace rapidjson;
		
		StringBuffer strbuf;
		PrettyWriter<StringBuffer> writer(strbuf);
	
		writer.StartObject();
		res.Serialize(writer);
		writer.EndObject();
	
		return strbuf.GetString();
	}
	
	static size_t Callback(const char* contents, size_t size, size_t nmemb, std::string* out)
	{
		const std::size_t realsize(size * nmemb);
		out->append(contents, realsize);
		return realsize;
	}

	template <class T>
	int Post(const std::string& url, T res)
	{
		CURL* curl;
		CURLcode resCode;
		curl_slist* slist = nullptr;
		std::string callbackBuffer = "";

		curl = curl_easy_init();
		slist = curl_slist_append(slist, "Content-Type: application/json");
	
		std::string json = Jsonify(res);

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callbackBuffer);

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(json.length()));
	
		// std::cerr << json << std::endl;

		resCode = curl_easy_perform(curl);
		if (resCode != CURLE_OK)
			throw std::runtime_error("Curl POST failed. Code: " + resCode);
	
		int response_code = -1;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		// std::cerr << "callbackBuffer size: " << callbackBuffer.length() << std::endl;
		// std::cerr << "Response:\n" << callbackBuffer << std::endl;
		
		curl_easy_cleanup(curl);
		return response_code;
	}

	inline std::string ReadConfig(const std::string& path)
	{
		std::ifstream fs;
		std::string data;
		fs.open(path, std::ios::in | std::ios::app);

		if (fs.is_open())
			fs >> data;
		else
			throw std::runtime_error("No config file found.\nCreate config.txt with content <addr=http://your.address>");
		
		fs.close();	
		return data.substr(5, data.length() - 4);
	}

	inline std::string GetThreadId()
	{
		int th_id = 0;
		std::stringstream ss;
		ss << std::this_thread::get_id();
		return ss.str();
	}

}