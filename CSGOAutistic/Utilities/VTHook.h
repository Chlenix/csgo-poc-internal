#pragma once

#include <memory>

namespace utils {
	namespace vtmanager
	{
		typedef void* vfunc;
		typedef void** vftable;
		typedef vftable* pvftable;

		class VTMananger
		{
		public:
			VTMananger();
			VTMananger(pvftable vt, std::size_t const &size);
			~VTMananger();
		public:
			void commit_hook();
			void release_hook();

			void prepare_function(vfunc vf, unsigned int const & index);
			template <typename Func> Func get_original_vfunc(unsigned int const &index);

		private:
			void init();
			void restore_original();
		private:
			pvftable vt_object;

			vftable vt_original;
			vftable vt_clone;

			std::size_t size;
		};
	}
}

