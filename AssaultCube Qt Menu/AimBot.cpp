#include "AimBot.h"
#include "Player.h"

#include <EOUL\Maths.h>
#include <EOUL\Constants.h>

#define QDegToRad(A) (A * 180.0f / EOUL::Math::PI)
#define QACUBE(A) (A / EOUL::Math::PI * 180.0f + 180.0f)

bool AimBot::aimbotActive, AimBot::noRecoilActive, AimBot::endLoop, AimBot::updateFirst = true;

HWND AimBot::gameWindow = NULL;
int AimBot::KeyPressTimer = 0;
int AimBot::UpdateAddressesTimer = 0;
bool AimBot::bUpdatedOnce = false;
bool AimBot::bGetProcessDataCurrent = false;
UINT_PTR AimBot::ac_clientModule = 0;
UINT_PTR AimBot::botClassAddr = 0;
UINT_PTR AimBot::humanClassAddr = 0;
UINT_PTR AimBot::localPlayerAddr = 0;
UINT_PTR AimBot::playerArrayPointer = 0x50F4F8; //multiplayer
UINT_PTR AimBot::playerArrayAddress = 0;

int AimBot::GameMode;
int AimBot::NumOfPlayers = 0;
bool AimBot::bTeamGame = false;

DWORD AimBot::dwProcId = NULL;
HANDLE AimBot::hProcHandle = NULL;

bool AimBot::bAimbotStatus = false;
DWORD AimBot::playerAddr[31];
bool AimBot::bSortByAngle = true;

std::vector<Player> playerVector;
bool playerSorter(Player& lhs, Player& rhs);

Player localPlayer;

void AimBot::run() {

	bSortByAngle = false;

	WriteProcessMemory(hProcHandle, (LPVOID) 0x463786, "\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, NULL);

	while (true) {

		bAimbotStatus = aimbotActive;

		if (noRecoilActive) {

			WriteProcessMemory(hProcHandle, (LPVOID) 0x463786, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);

		} else {

			WriteProcessMemory(hProcHandle, (LPVOID) 0x463786, "\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, NULL);

		}

		if (endLoop) {

			break;

		}

		if (!aimbotActive && !noRecoilActive || !GetAsyncKeyState(VK_LBUTTON)) {

			Sleep(16);

			continue;

		}

		getProcess();
		updateAdresses();
		updateCurrentGameData();
		
		while (GetAsyncKeyState(VK_LBUTTON) && bAimbotStatus) {

			aim();
			readPlayerData();
			sortPlayerVector();

			Sleep(1);

		}

		if (bAimbotStatus && NumOfPlayers != 0) {

			readPlayerData();
			sortPlayerVector();

		}

	}

}

void AimBot::aim() {

	if (localPlayer.bWeaponSemiAuto == false) {

		WriteProcessMemory(hProcHandle, (PBYTE*) (localPlayer.PlayerAddress + 0x40), &playerVector[0].vAimbotAngles, 8, NULL);

	} else {

		WriteProcessMemory(hProcHandle, (PBYTE*) (localPlayer.PlayerAddress + 0x40), &playerVector[0].vAimbotAngles, 8, NULL);
		WriteProcessMemory(hProcHandle, (PBYTE*) (localPlayer.PlayerAddress + 0x224), "x01", 1, NULL);

	}

}

void AimBot::readPlayerData() {

	localPlayer = Player();
	playerVector.clear();

	ReadProcessMemory(hProcHandle, (LPCVOID) playerArrayAddress, &playerAddr, 124, NULL);

	for (auto& player : playerAddr) {

		DWORD tmp;

		if (ReadProcessMemory(hProcHandle, (LPCVOID) player, &tmp, 4, NULL) == TRUE) {

			if (tmp == humanClassAddr || tmp == botClassAddr) {

				playerVector.push_back(Player(player));

			}

		}

	}

}

void AimBot::cleanUp() {

	endLoop = true;

	if (hProcHandle) {

		CloseHandle(hProcHandle);

	}

}

void AimBot::sortPlayerVector() {

	for (auto&& player : playerVector) {

		player.vAimbotAngles = calcAngle(localPlayer.vLocation, player.vLocation);

		if (bSortByAngle) {

			player.fCrosshairToTargetAngle = differenceOfAngles(player.vAimbotAngles, localPlayer.vCurrentAngle);

		} else {

			player.Distance = get3dDistance(player.vLocation, localPlayer.vLocation);

		}

	}

	sort(playerVector.begin(), playerVector.end(), playerSorter);

}

void AimBot::updateCurrentGameData() {

	ReadProcessMemory(hProcHandle, (LPCVOID) (ac_clientModule + 0x10F500), &NumOfPlayers, 4, NULL);

	if (NumOfPlayers != 0) {

		bTeamGame = false;

		ReadProcessMemory(hProcHandle, (LPCVOID) (0x50F49C), &GameMode, 1, NULL);

		static constexpr int m_teamMode[11] = { 0, 4, 5, 7, 13, 11, 14, 17, 16, 20, 21 };

		for (auto& i : m_teamMode) {

			if (i == GameMode) {

				bTeamGame = true;

				break;

			}

		}

	}

}

bool AimBot::findProcess() {

	gameWindow = FindWindow(NULL, L"AssaultCube");
	bGetProcessDataCurrent = false;

	return true;

}

DWORD_PTR AimBot::dwGetModuleBaseAddress(DWORD dwProcID, TCHAR* szModuleName) {

	DWORD_PTR dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);

	if (hSnapshot != INVALID_HANDLE_VALUE) {

		MODULEENTRY32 ModuleEntry32;
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &ModuleEntry32)) {

			do {

				if (_tcsicmp(ModuleEntry32.szModule, szModuleName) == 0) {

					dwModuleBaseAddress = (DWORD_PTR) ModuleEntry32.modBaseAddr;

					break;

				}

			} while (Module32Next(hSnapshot, &ModuleEntry32));

		}

		CloseHandle(hSnapshot);

	}

	return dwModuleBaseAddress;

}

DWORD AimBot::calculatePointer(HANDLE hProcHandle, int PointerLevel, DWORD Offsets[], UINT_PTR BaseAddress) {

	DWORD Pointer = BaseAddress;
	DWORD TempBuffer;

	DWORD PointerAddress;

	for (int i = 0; i < PointerLevel; i++) {

		if (i == 0) {

			ReadProcessMemory(hProcHandle, (LPCVOID) Pointer, &TempBuffer, 4, NULL);

		}

		PointerAddress = TempBuffer + Offsets[i];

		ReadProcessMemory(hProcHandle, (LPCVOID) PointerAddress, &TempBuffer, 4, NULL);

	}

	return PointerAddress;

}

void AimBot::getProcess() {

	if (findProcess()) {

		if (bGetProcessDataCurrent == false) {

			if (hProcHandle) {

				CloseHandle(hProcHandle);

			}

			GetWindowThreadProcessId(gameWindow, &dwProcId);

			if (dwProcId) {

				hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);

				if (!(hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)) {

					bGetProcessDataCurrent = true;

				}

			}

		}

	}


}

void AimBot::updateAdresses() {

	if (!bUpdatedOnce || UpdateAddressesTimer > 5000) {

		ac_clientModule = dwGetModuleBaseAddress(dwProcId, (TCHAR*) _T("ac_client.exe"));

		ReadProcessMemory(hProcHandle, (LPCVOID) 0x509B74, &localPlayerAddr, 4, NULL);
		ReadProcessMemory(hProcHandle, (LPCVOID) playerArrayPointer, &playerArrayAddress, 4, NULL);

		botClassAddr = ac_clientModule + 0xe4ac0;
		humanClassAddr = ac_clientModule + 0xE4A98;

		UpdateAddressesTimer = clock();
		bUpdatedOnce = true;

	}

}

float AimBot::distxy(AimBot::Vec3Pos src, AimBot::Vec3Pos dst) {

	float dx = dst.x - src.x;
	float dy = dst.y - src.y;

	return sqrtf(dx * dx + dy * dy);

}

float AimBot::get3dDistance(AimBot::Vec3Pos to, AimBot::Vec3Pos from) {

	return (float) (sqrt(((to.x - from.x) * (to.x - from.x)) + ((to.y - from.y) * (to.y - from.y)) + ((to.z - from.z) * (to.z - from.z))));

}

float AimBot::differenceOfAngles(AimBot::Vec3Angle vAngleToEnemy, AimBot::Vec3Angle vLocalPlayerViewAngle) {

	Vec3Angle vdifference;

	vdifference.pitch = vLocalPlayerViewAngle.pitch - vAngleToEnemy.pitch;
	vdifference.yaw = vLocalPlayerViewAngle.yaw - vAngleToEnemy.yaw;

	if (vdifference.pitch < 0) {

		vdifference.pitch *= -1;

	} 
	
	if (vdifference.yaw < 0) {

		vdifference.yaw *= -1;

	}

	float fDifference = (vdifference.pitch + vdifference.yaw) * .5f;

	return fDifference;

}

AimBot::Vec3Angle AimBot::calcAngle(Vec3Pos src, Vec3Pos dst) {

	Vec3Angle angles;

	angles.yaw = QACUBE((-(float) atan2(dst.x - src.x, dst.y - src.y)));
	angles.pitch = QDegToRad((atan2(dst.z - src.z, distxy(src, dst))));
	angles.roll = 0.0f;

	return angles;

}

bool playerSorter(Player& lhs, Player& rhs) {

	if (lhs.State != 0) {

		return false;

	} else {

		if (AimBot::bTeamGame) {

			if (lhs.Team != localPlayer.Team && rhs.Team == localPlayer.Team) {

				return true;

			}

			if (lhs.Team == localPlayer.Team && rhs.Team != localPlayer.Team) {

				return false;

			} else {

				if (AimBot::bSortByAngle) {

					return lhs.fCrosshairToTargetAngle < rhs.fCrosshairToTargetAngle;

				} else {

					return lhs.Distance < rhs.Distance;

				}

			}

		} else {

			if (AimBot::bSortByAngle) {

				return lhs.fCrosshairToTargetAngle < rhs.fCrosshairToTargetAngle;

			} else {

				return lhs.Distance < rhs.Distance;

			}

		}

	}

}