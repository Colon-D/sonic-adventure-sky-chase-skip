#include "SADXModLoader.h"

/// whether "whatever B is" is being held.
/// ideally this would be X as X does not fire, but X seems to be Y and Y seems
/// to be L and idk what L is. moral of the story, B is the button
bool holding_b{ false };

/// the tornado progress variable
DataPointer(unsigned, tornado, 0x3C82300);

/// if "whatever B is" is being held, move the tornado an 16x faster
void progress_sky_chase() {
	if (holding_b) {
		tornado += 15;
	}
}

/// location to jump to after progress_sky_chase_asm()
const unsigned* loc_626E78{ reinterpret_cast<unsigned*>(0x00626E78) };
/// tornado movement for sky chase act 1
void __declspec(naked) progress_sky_chase_asm() {
	__asm {
		; execute original instructions
		mov     eax, tornado          ; get the tornado pointer
		inc     DWORD PTR [eax]       ; move tornado forward
		pushad                        ; push current registers, so that
		                              ; my function does not mess with them
		call    progress_sky_chase    ; call my function
		popad                         ; pop registers
		mov     eax, [eax]            ; set eax to tornado value
		add     edx, 0FFFFFF9Ch       ; subtract 100 from something
		cmp     eax, edx              ; compare tornado to that something
		; and finally, return back to the original code
		jmp     [loc_626E78]          ; go back!
	}
}

/// location to jump to after progress_sky_chase_2_asm()
const unsigned* loc_627187{ reinterpret_cast<unsigned*>(0x00627187) };
/// tornado movement for sky chase act 2, before transformation
void __declspec(naked) progress_sky_chase_2_asm() {
	__asm {
		; execute original instructions
		mov     eax, tornado
		inc     DWORD PTR [eax]
		; call my function
		pushad
		call    progress_sky_chase
		popad
		; go back!
		mov     eax, [eax]
		cmp     eax, 157Ch
		jmp     [loc_627187]                  
	}
}

/// location to jump to after progress_sky_chase_2_2_asm()
const unsigned* loc_6272CE{ reinterpret_cast<unsigned*>(0x6272CE) };
DataPointer(unsigned, dword_29E345C, 0x29E345C);
/// tornado movement for sky chase act 2, after transformation
void __declspec(naked) progress_sky_chase_2_2_asm() {
	__asm {
		; execute original instructions
		mov     eax, tornado
		inc     DWORD PTR[eax]
		; call my function
		pushad
		call    progress_sky_chase
		popad
		; go back!
		mov     eax, [eax]
		mov     ecx, [dword_29E345C]
		add     [ecx], 0FFFFFFFBh
		cmp     eax, [ecx]
		jmp     [loc_6272CE]
	}
}

extern "C" {
	__declspec(dllexport) void __cdecl Init(
		const char* path,
		const HelperFunctions& helper_functions
	) {
		// tornado movement for sky chase act 1
		WriteJump(
			reinterpret_cast<void*>(0x626E6D),
			progress_sky_chase_asm
		);
		// tornado movement for sky chase act 2, before transformation
		WriteJump(
			reinterpret_cast<void*>(0x627177),
			progress_sky_chase_2_asm
		);
		// tornado movement for sky chase act 2, after transformation
		WriteJump(
			reinterpret_cast<void*>(0x6272B8),
			progress_sky_chase_2_2_asm
		);
	}

	__declspec(dllexport) void __cdecl OnInput() {
		// check if "whatever B is" is being held
		holding_b = ControllerPointers[0]->HeldButtons & Buttons_B;
	}

	__declspec(dllexport) ModInfo SADXModInfo{ ModLoaderVer };
}
