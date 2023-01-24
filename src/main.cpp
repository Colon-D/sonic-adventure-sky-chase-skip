#include "IniFile.hpp"
#include "SADXModLoader.h"
#include <filesystem>
#include <string_view>

unsigned speed{ 15 };
auto button = Buttons::Buttons_B;
bool disable_button{ true };

const std::unordered_map<std::string_view, Buttons> string_to_button{
	{ "A", Buttons::Buttons_A },
	{ "B", Buttons::Buttons_B },
	{ "X", Buttons::Buttons_X },
	{ "Y", Buttons::Buttons_Y },
	{ "D-Pad Up", Buttons::Buttons_Up },
	{ "D-Pad Down", Buttons::Buttons_Down },
	{ "D-Pad Left", Buttons::Buttons_Left },
	{ "D-Pad Right", Buttons::Buttons_Right },
	{ "L", Buttons::Buttons_L },
	{ "R", Buttons::Buttons_R },
	{ "Start", Buttons::Buttons_Start },

	{ "C", Buttons::Buttons_C },
	{ "D", Buttons::Buttons_D },
	{ "Z", Buttons::Buttons_Z },
};

bool try_boost{ false };

/// the tornado progress variable
DataPointer(unsigned, tornado, 0x3C82300);
/// the tornado's current position
DataPointer(NJS_VECTOR, tornado_translation, 0x3C822B4);
/// array of positions tornado should be in during the level
DataPointer(NJS_VECTOR*, tornado_translation_array, 0x3C822A0);

extern "C" {
	__declspec(dllexport) void __cdecl Init(
		const char* const path, const HelperFunctions& /*helper_functions*/
	) {
		const IniFile ini_file{ std::filesystem::path{ path } / "config.ini" };
		if (const auto* const settings = ini_file.getGroup("Settings")) {
			speed = settings->getInt("Boost Speed", speed);
			
			const auto button_string =
				settings->getString("Boost Button", "B");
			button = string_to_button.at(button_string);
			
			disable_button = settings->getBool(
				"Disable Boost Button from performing"
				"other actions in Sky Chase",
				disable_button
			);
		}
	}

	__declspec(dllexport) void __cdecl OnControl() {
		// if in Sky Chase and Boost button is being held and game isn't paused
		if (
			(
				CurrentLevel == LevelIDs::LevelIDs_SkyChase1
				|| CurrentLevel == LevelIDs::LevelIDs_SkyChase2
			)
			&& Controllers[0].HeldButtons & button
			&& GameState == 0x0F // unpaused?
		) {
			// try to boost
			try_boost = true;

			if (disable_button) {
				// disable the boost buttons
				Controllers[0].HeldButtons &= ~button;
				Controllers[0].PressedButtons &= ~button;
			}
		}
		else {
			try_boost = false;
		}
	}

	__declspec(dllexport) void __cdecl OnFrame() {
		switch (CurrentLevel) {
		case LevelIDs_SkyChase1: {
			if (!try_boost) {
				break;
			}
			// if boosting in sky chase zone 1:
			// stop at end cutscene
			constexpr unsigned end{ 8440 };
			if (tornado < end) {
				tornado += speed;
				if (tornado > end) {
					tornado = end;
				}
			}
			break; }
		case LevelIDs_SkyChase2: {
			if (!try_boost) {
				break;
			}
			// if boosting in sky chase zone 2:
			// when you have reached boss you are teleported to 15,
			// this check prevents you from moving whilst fighting the boss
			constexpr unsigned boss{ 15 };
			constexpr unsigned transform_begin{ 5000 };
			constexpr unsigned transform_end{ 5500 };
			if (tornado > boss) {
				// the transformation dialogue begins here
				if (tornado < transform_begin) {
					tornado += speed;
					if (tornado > transform_begin) {
						tornado = transform_begin;
					}
				}
				// the transformation has ended here
				if (tornado > transform_end) {
					tornado += speed;
				}
			}
			break; }
		default:
			// return if not in Sky Chase Zone
			return;
		}
		// if in either Sky Chase Zone, display the tornado at right position
		if (PlayerPtrs[0]) {
			tornado_translation = tornado_translation_array[tornado];
		}
	}

	__declspec(dllexport) ModInfo SADXModInfo{ ModLoaderVer };
}
