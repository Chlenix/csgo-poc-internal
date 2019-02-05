#include "..\stdafx.h"
#include "VTHook.h"
#include "..\index.hpp"
#include <iostream>

namespace utils 
{
	namespace vtmanager
	{
		VTMananger::VTMananger() : vt_object(nullptr), vt_original(nullptr), vt_clone(nullptr) {};
		VTMananger::VTMananger(pvftable vt, std::size_t const &size) :
			vt_object(vt),
			vt_original(reinterpret_cast<vftable>(reinterpret_cast<std::uintptr_t>(*vt))),
			vt_clone(nullptr),
			size(size)
		{
			init();
		};
		VTMananger::~VTMananger() {};

		void VTMananger::init()
		{
			void *tmp = VirtualAlloc(nullptr, this->size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			std::cout << "Allocated: 0x" << std::hex << tmp << std::endl;

			std::cout << "Original table: 0x" << std::hex << this->vt_original << std::endl;
			std::cout << "Table Object: 0x" << std::hex << this->vt_object << std::endl;

			std::memcpy(tmp, this->vt_original, this->size);

			this->vt_clone = reinterpret_cast<vftable>(tmp);
			std::cout << "VT CLONE: 0x" << std::hex << this->vt_clone << std::endl;
		}

		void VTMananger::commit_hook()
		{
			*this->vt_object = this->vt_clone;
		}

		void VTMananger::restore_original()
		{
			*this->vt_object = this->vt_original;
		}

		void VTMananger::release_hook()
		{
			restore_original();
			if (this->vt_clone != nullptr)
			{
				VirtualFree(this->vt_clone, 0, MEM_RELEASE);
			}
		}

		void VTMananger::prepare_function(vfunc vf, unsigned int const & index)
		{
			this->vt_clone[index] = vf;
		}

		template <typename Func> Func VTMananger::get_original_vfunc(unsigned int const &index)
		{
			return reinterpret_cast<Func>(this->vt_original[index]);
		}
	}
}

