#include "..\stdafx.h"
#include "VTHook.h"
#include "..\index.hpp"
#include <iostream>

namespace utils 
{
	namespace vt
	{
		VTMananger::VTMananger() : vt_object(nullptr), vt_original(nullptr), vt_clone(nullptr) {};
		VTMananger::VTMananger(pvftable vt, std::size_t const &size) :
			vt_object(vt),
			vt_original(*vt),
			vt_clone(nullptr),
			size(size)
		{
			init();
		};

		VTMananger::~VTMananger() {}
		pvftable VTMananger::get_class()
		{
			return this->vt_object;
		};

		void VTMananger::init()
		{
			void *tmp = VirtualAlloc(nullptr, this->size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			std::memcpy(tmp, this->vt_original, this->size);

			this->vt_clone = reinterpret_cast<vftable>(tmp);

			std::cout << "Allocated: 0x" << std::hex << tmp << std::endl;
			std::cout << "Original table: 0x" << std::hex << this->vt_original << std::endl;
			std::cout << "Table Object: 0x" << std::hex << this->vt_object << std::endl;
			std::cout << "VT CLONE: 0x" << std::hex << this->vt_clone << std::endl;
		}

		void VTMananger::commit_hook()
		{
			*this->vt_object = this->vt_clone;
		}

		void VTMananger::restore_original_table()
		{
			*this->vt_object = this->vt_original;
		}

		void VTMananger::release_hook()
		{
			restore_original_table();

			if (this->vt_clone != nullptr)
			{
				VirtualFree(this->vt_clone, 0, MEM_RELEASE);
			}
		}

		void VTMananger::prepare_function(vfunc vf, unsigned int const & index)
		{
			this->vt_clone[index] = vf;
		}
	}
}

