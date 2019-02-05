#pragma once

namespace utils
{
	namespace scanner
	{
		DWORD GetBinaryTextSection32(BYTE* pbRawModuleBytes, BYTE **pbTextSection);
		DWORD FindPattern(char const *szModule, char const *szMask, char const *szPattern, int iOffset);
	}
}