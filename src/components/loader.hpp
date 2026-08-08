#pragma once

namespace components
{
	// add new modules here
	struct activeModules_s
	{
		bool command;
		bool d3d9ex;
		//bool flags;
		//bool gscr_methods;
		bool scheduler;
		bool doart;

		//bool draw_collision;
	};

	extern activeModules_s active;

#define REGISTER_MODULE(name)			\
	if(active.##name)					\
		loader::register_(new name())

	class component
	{
	public:
		component() = default;
		virtual ~component() = default;
		virtual const char* get_name() { return "unknown"; }
	};

	class loader
	{
	public:
		static void initialize_();
		static void uninitialize_();
		static void register_(component* component);
		static bool is_registered(const char *componentName);

		static utils::memory::allocator* get_alloctor();
		
	private:
		static std::vector<component*> components_;
		static utils::memory::allocator component_allocator_;
	};
}

// General modules that have to be included
#include "modules/command.hpp"
#include "modules/flags.hpp"
#include "modules/d3d9ex.hpp"
#include "modules/doart.hpp"
//#include "modules/gscr_methods.hpp"

//#include "modules/draw_collision.hpp"

// General Modules with dynamic hooks depending on addon component loading
#include "modules/scheduler.hpp"
