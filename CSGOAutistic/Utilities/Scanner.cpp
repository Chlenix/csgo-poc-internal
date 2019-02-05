#include "..\stdafx.h"

#include "Scanner.h"

#include <iostream>
#include <string>
#include <string_view>
#include <deque>
#include <Windows.h>
#include <vector>
#include <algorithm>

#define WILDCARD_CHAR '?'

namespace utils
{
	namespace scanner
	{
		constexpr char const wildcard_char = '?';

		DWORD GetBinaryTextSection32(BYTE* pbRawModuleBytes, BYTE **pbTextSection)
		{
			PIMAGE_DOS_HEADER pDosHeader = nullptr;
			PIMAGE_NT_HEADERS pNtHeaders = nullptr;
			PIMAGE_SECTION_HEADER pSectionTable = nullptr;

			DWORD dwSize = 0;

			pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pbRawModuleBytes);

			if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			{
				std::cerr << "Invalid module format." << std::endl;
				return dwSize;
			}

			pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(pbRawModuleBytes + pDosHeader->e_lfanew);
			pSectionTable = IMAGE_FIRST_SECTION(pNtHeaders);

			for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; i++)
			{
				PIMAGE_SECTION_HEADER pSectionHeader = pSectionTable + i;

				if (std::strcmp(reinterpret_cast<const char*>(pSectionHeader->Name), ".text") == 0)
				{
					dwSize = pSectionTable->Misc.VirtualSize;
					*pbTextSection = reinterpret_cast<PBYTE>(pbRawModuleBytes + pSectionTable->VirtualAddress);
					break;
				}
			}

			return dwSize;
		}

		DWORD FindPattern(char const *szModule, char const *szMask, char const *szPattern, int iOffset)
		{
			BYTE* pbTextSection = nullptr;
			DWORD dwSectionSize = 0;
			DWORD dwOffset = 0;

			auto lpClientBase = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(szModule));

			// if no text section (size == 0)
			if (!(dwSectionSize = GetBinaryTextSection32(reinterpret_cast<BYTE *>(lpClientBase), &pbTextSection)))
			{
				return dwOffset;
			}

			auto svMask = std::string_view(szMask);
			auto svPattern = std::string_view(szPattern, svMask.length());
			auto svHaystack = std::string_view{ reinterpret_cast<char *>(pbTextSection), dwSectionSize };

			if (svMask.length() != svPattern.length())
			{
				return dwOffset;
			}

			std::deque<std::pair <std::size_t, std::size_t>> wildcards;

			for (auto it = svMask.begin(); it != svMask.end(); it++)
			{
				if (*it == wildcard_char)
				{
					std::size_t uiWildcardLength = 0;
					std::size_t uiWildcardOffset = std::distance(svMask.begin(), it);

					do
					{
						uiWildcardLength++;
					} while (++it != svMask.end() && *it == wildcard_char);

					wildcards.push_front(std::make_pair(uiWildcardOffset, uiWildcardLength));
				}
			}

			std::vector<std::pair<BYTE, bool>> pattern_data;

			std::transform(svPattern.begin(), svPattern.end(), svMask.begin(), std::back_inserter(pattern_data), [](BYTE const& chPattern, BYTE const &chMask) -> std::pair<BYTE, bool>
			{
				bool bIgnoreByte = (chMask == wildcard_char);
				return std::make_pair(chPattern, bIgnoreByte);
			});

			auto result = std::search(svHaystack.begin(), svHaystack.end(), pattern_data.begin(), pattern_data.end(), [&](BYTE const &i, std::pair<BYTE, bool> const &p) -> bool
			{
				return p.second || p.first == i;
			});

			if (result != svHaystack.end())
			{
				auto pointer_ = reinterpret_cast<std::uintptr_t>(result.operator->());
				auto& lpdwVirtualAddress = *reinterpret_cast<PDWORD>(pointer_ + wildcards[iOffset].first);
				dwOffset = lpdwVirtualAddress - lpClientBase;
			}

			return dwOffset;
		}
	}
}