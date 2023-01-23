#include "SADXModLoader.h"

// todo config
constexpr auto button = Buttons::Buttons_B;
constexpr unsigned speed{ 15 };

bool try_boost{ false };

/// the tornado progress variable
DataPointer(unsigned, tornado, 0x3C82300);
/// the tornado's current position
DataPointer(NJS_VECTOR, tornado_translation, 0x3C822B4);
/// array of positions tornado should be in during the level
DataPointer(NJS_VECTOR*, tornado_translation_array, 0x3C822A0);

extern "C" {
	__declspec(dllexport) void __cdecl OnInit(
		const char* const /*path*/, const HelperFunctions& /*helper_functions*/
	) {}

	__declspec(dllexport) void __cdecl OnControl() {
		// if in Sky Chase and Boost button is being held
		if (
			(
				CurrentLevel == LevelIDs::LevelIDs_SkyChase1
				|| CurrentLevel == LevelIDs::LevelIDs_SkyChase2
			)
			&& (Controllers[0].HeldButtons & button || Controllers[0].PressedButtons & button)
		) {
			// try to boost
			try_boost = true;

			// disable the boost buttons
			Controllers[0].HeldButtons &= ~button;
			Controllers[0].PressedButtons &= ~button;
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
