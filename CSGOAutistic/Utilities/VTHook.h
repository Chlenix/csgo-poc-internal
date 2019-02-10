#pragma once

#include <memory>

namespace utils {
	namespace vt
	{
		typedef void* vfunc;
		typedef vfunc* vftable;
		typedef vftable* pvftable;

		class VTMananger
		{
		public:
			VTMananger();
			VTMananger(pvftable vt, std::size_t const &size = 0);
			VTMananger(std::uintptr_t vt, std::size_t const &size = 0) : VTMananger(reinterpret_cast<pvftable>(vt), size) {};
			~VTMananger();

			pvftable get_class();

			void commit_hook();
			void release_hook();

			void prepare_function(vfunc vf, unsigned int const & index);

			template <typename T>
			inline T get_original_vfunc(unsigned int const &index)
			{
				return reinterpret_cast<T>(this->vt_original[index]);
			}

		private:
			void init();
			void restore_original_table();

		private:
			pvftable vt_object = nullptr;
			vftable vt_original = nullptr;
			vftable vt_clone = nullptr;

			std::size_t size = 0;
		};
	}
}

