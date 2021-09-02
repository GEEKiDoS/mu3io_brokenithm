#include <Windows.h>
#include <cstdint>
#include <iostream>

#include "mu3io/mu3io.hpp"
#include "brokenithm/brokenithm.hpp"

using namespace mu3::io;

namespace brokenithm
{
	static void* file_mapping_handle = nullptr;
	static ipc_memory_info* info = nullptr;

	
	const game_buttons slider_mapping[] = 
	{
		// right
		c, c, c, c,
		b, b, b, b,
		a, a, a, a, a,
		// left
		// right hand is for joy stick so left need more space
		c, c, c, c, c, c,
		b, b, b, b, b, b,
		a, a, a, a, a, a, a,
	};

	void init()
	{
		if (info) return;
		printf("--- brokenithm::init ---\n");

		file_mapping_handle = CreateFileMapping(
			INVALID_HANDLE_VALUE, 0, PAGE_READWRITE,
			0, sizeof(ipc_memory_info), "Local\\BROKENITHM_SHARED_BUFFER"
		);

		if (!file_mapping_handle) return;

		printf("--- file mapping done ---\n");

		info = reinterpret_cast<ipc_memory_info*>(
			MapViewOfFile(
				file_mapping_handle, FILE_MAP_ALL_ACCESS,
				0, 0, sizeof(ipc_memory_info)
			)
		);

		if (!info) return;

		printf("--- map view done ---\n");

		std::memset(info, 0, sizeof(ipc_memory_info));

		// init colors;
		for (int i = 0; i < 4; i++)
			info->colors[i] = color_t(0x0a95ff);
		for (int i = 4; i < 8; i++)
			info->colors[i] = color_t(0x7fba00);
		for (int i = 8; i < 13; i++)
			info->colors[i] = color_t(0xf74e1e);

		for (int i = 13; i < 19; i++)
			info->colors[i] = color_t(0x0a95ff);
		for (int i = 19; i < 25; i++)
			info->colors[i] = color_t(0x7fba00);
		for (int i = 25; i < 31; i++)
			info->colors[i] = color_t(0xf74e1e);

		printf("---brokenithm::init done ---\n");
	}

	game_buttons get_key(bool right)
	{
		uint8_t button = 0;

		if (info)
		{
			int start = right ? 12 : 31;
			int end = right ? 0 : 13;
			for (int i = start; i >= end; --i)
			{
				if (info->slider_io_status[i])
				{
					button |= slider_mapping[i];
				}
			}
		}

		return static_cast<game_buttons>(button);
	}

	void free()
	{
		if (!info) return;

		UnmapViewOfFile(info);
		CloseHandle(file_mapping_handle);
	}
}
