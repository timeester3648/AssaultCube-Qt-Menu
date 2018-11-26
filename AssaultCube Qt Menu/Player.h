#pragma once

#include "AimBot.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <tlhelp32.h>
#include <tchar.h>
#include <math.h>
#include <algorithm>
#include <vector>

class Player {

	public:

		AimBot::Vec3Pos vLocation;
		AimBot::Vec3Angle vCurrentAngle;
		AimBot::Vec3Angle vAimbotAngles;
		UINT_PTR PlayerAddress;
		char Name[16];
		BYTE Team;
		int Health;
		BYTE State;
		float Distance;
		float fCrosshairToTargetAngle;
		bool bWeaponSemiAuto = 0;

	public:

		Player();
		Player(DWORD playerAddr);

};