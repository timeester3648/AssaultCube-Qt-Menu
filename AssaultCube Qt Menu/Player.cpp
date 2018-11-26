#include "Player.h"

#define WeaponSemiAuto (t_WeaponNum == 1 || t_WeaponNum == 2 || t_WeaponNum == 3 || t_WeaponNum == 5 || t_WeaponNum == 7 || t_WeaponNum == 0 || t_WeaponNum == 8)

Player::Player() {

	PlayerAddress = AimBot::localPlayerAddr;

	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x225), &Name, 16, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x32c), &Team, 1, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0xF8), &Health, 4, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x34), &vLocation, 12, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x338), &State, 1, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x40), &vCurrentAngle, 12, NULL);

	int t_WeaponNum;
	DWORD tmpOffsets[2] = { 0x374, 0x4 };

	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID) (AimBot::calculatePointer(AimBot::hProcHandle, 2, tmpOffsets, 0x509B74)), &t_WeaponNum, 4, NULL);

	if (WeaponSemiAuto) {

		bWeaponSemiAuto = true;

	} else {

		bWeaponSemiAuto = false;

	}

}

Player::Player(DWORD playerAddr) {

	PlayerAddress = playerAddr;

	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x225), &Name, 16, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x32c), &Team, 1, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0xF8), &Health, 4, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x34), &vLocation, 12, NULL);
	ReadProcessMemory(AimBot::hProcHandle, (LPCVOID)(PlayerAddress + 0x338), &State, 1, NULL);

}