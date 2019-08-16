#pragma once

#pragma comment(lib, "urlmon")
#pragma comment(lib, "Ws2_32")
#pragma comment(lib, "WinInet")

#define NOMINMAX
#include <WinSock2.h>
#include <urlmon.h>
#include <Windows.h>
#include <WinInet.h>
#include <Ws2tcpip.h>

#include <string>
#include <vector>
#include <thread>
#include <functional>

#include <EOUL/String.hpp>

using namespace std::string_literals;

namespace EOUL::Net {

	/*

		downloads a file from the given url to the given path. path needs to contain the file name.
		it will use the cache if useCache is true.

	*/
	inline HRESULT download(const std::string& url, const std::string& path, bool useCache = false) {

		if (!useCache) {

			DeleteUrlCacheEntryA(url.c_str());

		}

		return URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);

	}
	/*

		downloads a file from the given url to the given path. path needs to contain the file name.
		it will use the cache if useCache is true.

	*/
	inline HRESULT download(const std::wstring& url, const std::wstring& path, bool useCache = false) {

		if (!useCache) {

			DeleteUrlCacheEntryW(url.c_str());

		}

		return URLDownloadToFileW(NULL, url.c_str(), path.c_str(), 0, NULL);

	}

	/* returns if the current pc has a internet connection */
	inline bool hasInternet() {

		DWORD check;
		BOOL access = InternetGetConnectedState(&check, 0);

		return !(check == INTERNET_CONNECTION_OFFLINE || access == 0);

	}

}