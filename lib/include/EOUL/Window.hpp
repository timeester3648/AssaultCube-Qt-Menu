#pragma once

#pragma comment(lib, "Psapi")

#include <string>
#include <Psapi.h>
#include <Windows.h>
#include <TlHelp32.h>

#include <EOUL/String.hpp>

namespace EOUL::Windows {

	using namespace std::string_literals;

	class Window final {

		public:

			std::string name;

			HANDLE handle = 0;
			HWND window = 0;
			DWORD process = 0;

		public:

			inline Window(std::string name) : name(std::move(name)) {

				this->window = FindWindowA(NULL, (LPCSTR) this->name.c_str());

				GetWindowThreadProcessId(this->window, &this->process);

				this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->process);

			}

			inline Window(Window&& other) noexcept {

				this->swap(other);

			}

			inline ~Window() {

				CloseHandle(this->handle);

			}

		public:

			/* swaps 2 Windows */
			inline void swap(Window& other) noexcept {

				std::swap(this->name, other.name);
				std::swap(this->handle, other.handle);
				std::swap(this->window, other.window);
				std::swap(this->process, other.process);

			}

			/* Keep arguments empty if the exe has the same name as the window, returns the base adress of the application */
			inline size_t getBaseAdress(std::wstring customExeName) const {

				HMODULE hMods[1024];
				DWORD cbNeeded;
				unsigned int i;

				if (!Util::endsWith(customExeName, L".exe"s)) {

					customExeName.append(L".exe");

				}

				if (EnumProcessModules(this->handle, hMods, sizeof(hMods), &cbNeeded)) {

					for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {

						WCHAR szModName[MAX_PATH];

						if (GetModuleFileNameExW(this->handle, hMods[i], szModName, sizeof(szModName) / sizeof(WCHAR))) {

							std::wstring wstrModName = szModName;

							if (wstrModName.find(customExeName) != std::wstring::npos) {

								return (size_t) hMods[i];

							}

						}

					}

				}

				return -1;

			}

			/* writes data to the given menory adress */
			template<typename T, typename enabled = std::enable_if_t<!std::is_array_v<T>>>
			inline size_t writeMemory(size_t adress, T* data, size_t count) const {

				size_t written = 0;

				WriteProcessMemory(this->handle, (LPVOID) adress, (LPCVOID) data, count * sizeof(T), &written);

				return written;

			}
			/* reads data from the given menory adress */
			template<typename T, typename enabled = std::enable_if_t<!std::is_array_v<T>>>
			inline size_t readMemory(size_t adress, T* buffer, size_t count) const {

				size_t read = 0;

				ReadProcessMemory(this->handle, (LPCVOID) adress, (LPVOID) buffer, count * sizeof(T), &read);

				return read;

			}

	};

}