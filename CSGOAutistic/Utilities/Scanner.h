#pragma once

namespace utils
{
	namespace patterns
	{
		constexpr static char const *entity_list = "\xBB\x11\x11\x11\x11\x90\x83\xFF\x01\x7C\x11\xA1";
		constexpr static char const *view_matrix = "\x0F\x10\x05\x11\x11\x11\x11\x8D\x85\x11\x11\x11\x11\xB9";
	}

	namespace masks
	{
		constexpr static char const *entity_list = "x????xxxxx?x";
		constexpr static char const *view_matrix = "xxx????xx????x";
	}

	namespace scanner
	{
		DWORD GetBinaryTextSection32(BYTE* pbRawModuleBytes, BYTE **pbTextSection);
		DWORD FindPattern(char const *szModule, char const *szMask, char const *szPattern, int iOffset);
	}
}