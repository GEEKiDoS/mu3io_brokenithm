#include <Windows.h>
#include <cstdint>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "mu3io/mu3io.hpp"
#include "brokenithm/brokenithm.hpp"

namespace mu3::io
{
	static uint8_t opbtn;
	static uint8_t left_btn;
	static uint8_t right_btn;
	static int16_t lever_xpos;

	namespace
	{
		static LPDIRECTINPUT8A dinput_instance;
		static IDirectInputDevice8* dinput_device;

		HRESULT dinput_init()
		{
			return DirectInput8Create(
				GetModuleHandle(NULL),
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(LPVOID*)&dinput_instance,
				NULL
			);
		}

		HRESULT dinput_get_device()
		{
			return dinput_instance->EnumDevices(DI8DEVCLASS_GAMECTRL, [](auto instance, auto)
			{
				dinput_instance->CreateDevice(instance->guidInstance, &dinput_device, NULL);
				dinput_device->SetCooperativeLevel(GetActiveWindow(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
				dinput_device->SetDataFormat(&c_dfDIJoystick);
				dinput_device->Acquire();
				return DIENUM_STOP;
			}, NULL, DIEDFL_ALLDEVICES);
		}

		HRESULT dinput_poll()
		{
			if (!dinput_device)
			{
				dinput_get_device();
			}
			else
			{
				DIJOYSTATE state;
				auto result = dinput_device->GetDeviceState(sizeof(DIJOYSTATE), &state);

				if (result == DI_OK)
				{
#if USE_SLIDER
					lever_xpos = (state.rglSlider[0] - 32767) / 2;
#else
					lever_xpos = (state.lX - 32767) / 2;
#endif

					if (state.rgbButtons[0])
						right_btn |= game_buttons::side;

					if (state.rgbButtons[1])
						left_btn |= game_buttons::side;
				}
				else if (result == DIERR_INPUTLOST)
				{
					dinput_device = nullptr;
					dinput_get_device();
				}
				else return result;
			}

			return S_OK;
		}
	}
}

uint16_t mu3_io_get_api_version(void)
{
	return 0x0101;
}

HRESULT mu3_io_init(void)
{
	brokenithm::init();
	return mu3::io::dinput_init();
}

HRESULT mu3_io_poll(void)
{
	mu3::io::left_btn = brokenithm::get_key(false);
	mu3::io::right_btn = brokenithm::get_key(true);

	auto result = mu3::io::dinput_poll();

	if (GetAsyncKeyState(VK_LCONTROL) < 0)
		mu3::io::left_btn |= mu3::io::game_buttons::menu;
	if (GetAsyncKeyState(VK_RCONTROL) < 0)
		mu3::io::right_btn |= mu3::io::game_buttons::menu;

	return result;
}

void mu3_io_get_opbtns(uint8_t* opbtn)
{
    if (opbtn) *opbtn = mu3::io::opbtn;
}

void mu3_io_get_gamebtns(uint8_t* left, uint8_t* right)
{
    if (left) *left = mu3::io::left_btn;
    if (right) *right = mu3::io::right_btn;
}

void mu3_io_get_lever(int16_t* pos)
{
    if (pos) *pos = mu3::io::lever_xpos;
}
