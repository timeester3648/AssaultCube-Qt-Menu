#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <string>

namespace EOUL {

	namespace Windows {

		class Window {

			public:

				std::string name;
				HANDLE handle;
				HWND window;
				DWORD process;

			public:

				Window(std::string name);

			public:

				/* Keep arguments empty if the exe has the same name as the window, returns the base adress of the application */
				unsigned long long getBaseAdress(std::string customExeName = "") const;

				/* writes data to the given menory adress */
				void writeMemory(unsigned long long adress, void* data, unsigned long long size) const;
				/* reads data from the given menory adress */
				void readMemory(unsigned long long adress, void* buffer, unsigned long long size) const;

				/* writes data to the given menory adress */
				void writeMemory(void* adress, void* data, unsigned long long size) const;
				/* reads data from the given menory adress */
				void readMemory(void* adress, void* buffer, unsigned long long size) const;

		};

	}

}