#pragma once

#include <QtWidgets/QMainWindow>
#include <QCheckBox>

#include <EOUL\Window.h>
#include <thread>

#include "ui_Window.h"
#include "AimBot.h"

class Window : public QMainWindow {

	Q_OBJECT

	private:

		Ui::WindowClass ui;

		QFont check_box_font = QFont("Arial", 12, QFont::Weight::Bold);
		QCursor hand_pointer = QCursor(Qt::CursorShape::PointingHandCursor);

		QCheckBox* aimbot;
		QCheckBox* no_recoil;
		QCheckBox* infinite_ammo;
		QCheckBox* infinite_armor;
		QCheckBox* infinite_health;
		QCheckBox* infinite_smg_ammo;
		QCheckBox* infinite_rifle_ammo;
		QCheckBox* infinite_pistol_ammo;
		QCheckBox* infinite_shotgun_ammo;
		QCheckBox* infinite_grenades_ammo;

		static constexpr int ofs_BaseModule = 0x109B74;
		static constexpr int ofs_iRifleAmmo = 0x150;
		static constexpr int ofs_iSMGAmmo = 0x148;
		static constexpr int ofs_iShotgunAmmo = 0x144;
		static constexpr int ofs_iPistolAmmo = 0x13C;
		static constexpr int ofs_iGrenades = 0x158;
		static constexpr int ofs_iHealth = 0xF8;
		static constexpr int ofs_iArmor = 0xFC;

		std::thread menu_thread, update_thread, aimbot_thread;
		EOUL::Windows::Window* window = nullptr;

		int base_address = 0, base_module_address = 0;
		int ptr_iRifleAmmo, ptr_iSMGAmmo, ptr_iShotgunAmmo, ptr_iPistolAmmo, ptr_iGrenades, ptr_iHealth, ptr_iArmor;

		bool aimbotActive = false, noRecoilActive = false, endLoop = false, closed = false;

	public:

		Window(QWidget* parent = Q_NULLPTR);
		~Window();

	protected:

		void closeEvent(QCloseEvent* event) override;

	private:

		void enable_all();
		void disable_all();
		void enable_all_infinite_ammo();
		void disable_all_infinite_ammo();

	private slots:

		void state_changed(int state);

};
