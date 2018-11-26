#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <tlhelp32.h>
#include <tchar.h>
#include <math.h>
#include <algorithm>
#include <vector>

class AimBot {

	public:

		struct Vec3Pos {

			float x, y, z;

		};

		struct Vec3Angle {

			float yaw, pitch, roll;

		};

	public:

		static bool aimbotActive, noRecoilActive, endLoop;

		static HWND gameWindow;
		static int KeyPressTimer;
		static int UpdateAddressesTimer;
		static bool bUpdatedOnce;
		static bool bGetProcessDataCurrent;
		static UINT_PTR ac_clientModule;
		static UINT_PTR botClassAddr;
		static UINT_PTR humanClassAddr;
		static UINT_PTR localPlayerAddr;
		static UINT_PTR playerArrayPointer;
		static UINT_PTR playerArrayAddress;

		static int GameMode;
		static int NumOfPlayers;
		static bool bTeamGame;

		static DWORD dwProcId;
		static HANDLE hProcHandle;

		static bool bAimbotStatus;
		static DWORD playerAddr[31];
		static bool bSortByAngle;

	private:

		static bool updateFirst;

	public:

		static bool findProcess();
		static DWORD_PTR dwGetModuleBaseAddress(DWORD dwProcID, TCHAR* szModuleName);
		static DWORD calculatePointer(HANDLE hProcHandle, int PointerLevel, DWORD Offsets[], UINT_PTR BaseAddress);

		static void run();
		static void aim();
		static void cleanUp();
		static void getProcess();
		static void readPlayerData();
		static void updateAdresses();
		static void updateCurrentGameData();
		static void sortPlayerVector();

		static float distxy(Vec3Pos src, Vec3Pos dst);
		static float get3dDistance(Vec3Pos to, Vec3Pos from);
		static float differenceOfAngles(Vec3Angle vAngleToEnemy, Vec3Angle vLocalPlayerViewAngle);

		static Vec3Angle calcAngle(Vec3Pos src, Vec3Pos dst);

};