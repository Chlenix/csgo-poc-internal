#include "..\stdafx.h"
#include "VTHook.h"

namespace hooks
{
	VMThook::VMThook() : original(nullptr), introduced(nullptr) {};
	VMThook::~VMThook() {};

	template <typename T> T VMThook::getvfunc(void **vtable, unsigned int const &index)
	{
		return nullptr;
	}
}

