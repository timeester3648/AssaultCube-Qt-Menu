#pragma once

#pragma comment(lib, "Psapi")

#include <Windows.h>
#include <Psapi.h>

namespace EOUL::Util {

	namespace Private {

		inline static MEMORYSTATUSEX memInfo;
		inline static bool initialized = false;

	}

	enum MemoryType {

		Physical,
		Virtual

	};

	/* returns how much ram is free */
	inline size_t ramFree(MemoryType type = MemoryType::Physical) {

		if (!Private::initialized) {

			Private::memInfo.dwLength = sizeof(MEMORYSTATUSEX);

			Private::initialized = true;

		}

		GlobalMemoryStatusEx(&Private::memInfo);

		if (type == MemoryType::Physical) {

			return Private::memInfo.ullAvailPhys;

		} else {

			return Private::memInfo.ullAvailVirtual;

		}

	}
	/*  returns how much ram is used */
	inline size_t ramUsed(MemoryType type = MemoryType::Physical) {

		if (!Private::initialized) {

			Private::memInfo.dwLength = sizeof(MEMORYSTATUSEX);

			Private::initialized = true;

		}

		GlobalMemoryStatusEx(&Private::memInfo);

		if (type == MemoryType::Physical) {

			return Private::memInfo.ullTotalPhys - Private::memInfo.ullAvailPhys;

		} else {

			return Private::memInfo.ullTotalVirtual - Private::memInfo.ullAvailVirtual;

		}

	}
	/* returns how much ram is used by current process */
	inline size_t ramUsedByCurrent() {

		PROCESS_MEMORY_COUNTERS pmc;

		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

		return pmc.WorkingSetSize;

	}

}