#pragma once

#include <memory>

namespace hooks
{
	typedef void** vftable, *pvftable;

	class VMThook
	{
	public:
		VMThook();
		~VMThook();
		template <typename T> T getvfunc(void **vtable, unsigned int const &index);

	private:
		vftable *original;
		vftable *introduced;
	};
}

