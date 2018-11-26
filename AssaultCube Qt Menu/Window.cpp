#include "Window.h"

#include <QDebug>

Window::Window(QWidget* parent) : QMainWindow(parent) {

	this->ui.setupUi(this);

	this->setFixedSize(300, 350);
	this->setWindowTitle("AssautCube Mod Menu");
	this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

	this->aimbot = new QCheckBox("Aimbot", this);
	this->no_recoil = new QCheckBox("No Recoil", this);
	this->infinite_ammo = new QCheckBox("Infinite Ammo", this);
	this->infinite_armor = new QCheckBox("Infinite Armor", this);
	this->infinite_health = new QCheckBox("Infinite Health", this);
	this->infinite_smg_ammo = new QCheckBox("Infinite SMG Ammo", this);
	this->infinite_rifle_ammo = new QCheckBox("Infinite Rifle Ammo", this);
	this->infinite_pistol_ammo = new QCheckBox("Infinite Pistol Ammo", this);
	this->infinite_shotgun_ammo = new QCheckBox("Infinite Shotgun Ammo", this);
	this->infinite_grenades_ammo = new QCheckBox("Infinite Grenades Ammo", this);

	this->aimbot->setDisabled(true);
	this->aimbot->setGeometry(50, 250, 250, 20);
	this->aimbot->setCursor(this->hand_pointer);
	this->aimbot->setFont(this->check_box_font);

	this->no_recoil->setDisabled(true);
	this->no_recoil->setGeometry(50, 270, 250, 20);
	this->no_recoil->setCursor(this->hand_pointer);
	this->no_recoil->setFont(this->check_box_font);

	this->infinite_ammo->setDisabled(true);
	this->infinite_ammo->setGeometry(50, 50, 250, 20);
	this->infinite_ammo->setCursor(this->hand_pointer);
	this->infinite_ammo->setFont(this->check_box_font);

	this->infinite_smg_ammo->setDisabled(true);
	this->infinite_smg_ammo->setGeometry(50, 70, 250, 20);
	this->infinite_smg_ammo->setCursor(this->hand_pointer);
	this->infinite_smg_ammo->setFont(this->check_box_font);

	this->infinite_rifle_ammo->setDisabled(true);
	this->infinite_rifle_ammo->setGeometry(50, 90, 250, 20);
	this->infinite_rifle_ammo->setCursor(this->hand_pointer);
	this->infinite_rifle_ammo->setFont(this->check_box_font);

	this->infinite_pistol_ammo->setDisabled(true);
	this->infinite_pistol_ammo->setGeometry(50, 110, 250, 20);
	this->infinite_pistol_ammo->setCursor(this->hand_pointer);
	this->infinite_pistol_ammo->setFont(this->check_box_font);

	this->infinite_shotgun_ammo->setDisabled(true);
	this->infinite_shotgun_ammo->setGeometry(50, 130, 250, 20);
	this->infinite_shotgun_ammo->setCursor(this->hand_pointer);
	this->infinite_shotgun_ammo->setFont(this->check_box_font);

	this->infinite_grenades_ammo->setDisabled(true);
	this->infinite_grenades_ammo->setGeometry(50, 150, 250, 20);
	this->infinite_grenades_ammo->setCursor(this->hand_pointer);
	this->infinite_grenades_ammo->setFont(this->check_box_font);

	this->infinite_armor->setDisabled(true);
	this->infinite_armor->setGeometry(50, 190, 250, 20);
	this->infinite_armor->setCursor(this->hand_pointer);
	this->infinite_armor->setFont(this->check_box_font);

	this->infinite_health->setDisabled(true);
	this->infinite_health->setGeometry(50, 210, 250, 20);
	this->infinite_health->setCursor(this->hand_pointer);
	this->infinite_health->setFont(this->check_box_font);

	QObject::connect(this->aimbot, SIGNAL(stateChanged(int)), SLOT(state_changed(int)));
	QObject::connect(this->no_recoil, SIGNAL(stateChanged(int)), SLOT(state_changed(int)));
	QObject::connect(this->infinite_ammo, SIGNAL(stateChanged(int)), SLOT(state_changed(int)));

	this->menu_thread = std::thread([&]() {

		do {

			if (this->window != nullptr) {

				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			}

			delete this->window;

			this->window = new EOUL::Windows::Window("AssaultCube");

		} while (this->window->handle == nullptr && !this->closed);

		if (this->closed) {

			return;

		}

		this->base_address = this->window->getBaseAdress("ac_client.exe");

		this->window->readMemory((this->base_address + ofs_BaseModule), &this->base_module_address, sizeof(this->base_module_address));

		this->ptr_iRifleAmmo = this->base_module_address + ofs_iRifleAmmo;
		this->ptr_iSMGAmmo = this->base_module_address + ofs_iSMGAmmo;
		this->ptr_iPistolAmmo = this->base_module_address + ofs_iPistolAmmo;
		this->ptr_iShotgunAmmo = this->base_module_address + ofs_iShotgunAmmo;
		this->ptr_iGrenades = this->base_module_address + ofs_iGrenades;
		this->ptr_iHealth = this->base_module_address + ofs_iHealth;
		this->ptr_iArmor = this->base_module_address + ofs_iArmor;

		this->aimbot_thread = std::thread(AimBot::run);

		this->enable_all();

		this->update_thread = std::thread([&]() {

			int value = 1337;

			while (!this->closed && this->window->handle != nullptr) {

				if (FindWindow(NULL, L"AssaultCube") == nullptr) {

					this->disable_all();

					AimBot::cleanUp();

					this->close();

					return;

				}

				if (this->infinite_ammo->isChecked() || this->infinite_smg_ammo->isChecked()) {

					this->window->writeMemory(this->ptr_iSMGAmmo, &value, sizeof(value));

				}

				if (this->infinite_ammo->isChecked() || this->infinite_rifle_ammo->isChecked()) {

					this->window->writeMemory(this->ptr_iRifleAmmo, &value, sizeof(value));

				}

				if (this->infinite_ammo->isChecked() || this->infinite_pistol_ammo->isChecked()) {

					this->window->writeMemory(this->ptr_iPistolAmmo, &value, sizeof(value));

				}

				if (this->infinite_ammo->isChecked() || this->infinite_shotgun_ammo->isChecked()) {

					this->window->writeMemory(this->ptr_iShotgunAmmo, &value, sizeof(value));

				}

				if (this->infinite_ammo->isChecked() || this->infinite_grenades_ammo->isChecked()) {

					this->window->writeMemory(this->ptr_iGrenades, &value, sizeof(value));

				}

				if (this->infinite_armor->isChecked()) {

					this->window->writeMemory(this->ptr_iArmor, &value, sizeof(value));

				}

				if (this->infinite_health->isChecked()) {

					this->window->writeMemory(this->ptr_iHealth, &value, sizeof(value));

				}

				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			}

		});

	});

}

Window::~Window() {

	delete this->aimbot;
	delete this->no_recoil;
	delete this->infinite_ammo;
	delete this->infinite_smg_ammo;
	delete this->infinite_rifle_ammo;
	delete this->infinite_pistol_ammo;
	delete this->infinite_shotgun_ammo;
	delete this->infinite_grenades_ammo;

	if (this->menu_thread.joinable()) {

		this->menu_thread.join();

	}

	if (this->update_thread.joinable()) {

		this->update_thread.join();

	}

	if (this->aimbot_thread.joinable()) {

		this->aimbot_thread.join();

	}

	delete this->window;

}

void Window::closeEvent(QCloseEvent* event) {

	AimBot::cleanUp();

	this->closed = true;

}

void Window::enable_all() {

	this->enable_all_infinite_ammo();

	this->aimbot->setDisabled(false);
	this->no_recoil->setDisabled(false);
	this->infinite_armor->setDisabled(false);
	this->infinite_health->setDisabled(false);

}

void Window::disable_all() {

	this->disable_all_infinite_ammo();

	this->aimbot->setDisabled(true);
	this->no_recoil->setDisabled(true);
	this->infinite_armor->setDisabled(true);
	this->infinite_health->setDisabled(true);

}

void Window::enable_all_infinite_ammo() {

	this->infinite_ammo->setDisabled(false);
	this->infinite_smg_ammo->setDisabled(false);
	this->infinite_rifle_ammo->setDisabled(false);
	this->infinite_pistol_ammo->setDisabled(false);
	this->infinite_shotgun_ammo->setDisabled(false);
	this->infinite_grenades_ammo->setDisabled(false);

}

void Window::disable_all_infinite_ammo() {

	this->infinite_ammo->setDisabled(true);
	this->infinite_smg_ammo->setDisabled(true);
	this->infinite_rifle_ammo->setDisabled(true);
	this->infinite_pistol_ammo->setDisabled(true);
	this->infinite_shotgun_ammo->setDisabled(true);
	this->infinite_grenades_ammo->setDisabled(true);

}

void Window::state_changed(int state) {

	if (QObject::sender() == this->infinite_ammo) {

		if (state) {

			this->disable_all_infinite_ammo();

			this->infinite_ammo->setDisabled(false);

		} else {

			this->enable_all_infinite_ammo();

		}

	} else if (QObject::sender() == this->aimbot) {

		AimBot::aimbotActive = this->aimbot->isChecked();

	} else if (QObject::sender() == this->no_recoil) {

		AimBot::noRecoilActive = this->no_recoil->isChecked();

	}

}