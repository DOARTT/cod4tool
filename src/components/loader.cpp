#include "std_include.hpp"

namespace components
{
	// global active modules struct
	activeModules_s active = activeModules_s();

	std::vector<component*> loader::components_;
	utils::memory::allocator loader::component_allocator_;

	void loader::initialize_()
	{
		loader::component_allocator_.clear();

		// global bools for more dynamic modules (choose which modules to load)
		active.command = true;
		active.d3d9ex = true;
		//active.gscr_methods = true;
		active.scheduler = true;
		active.doart = true;

		//active.draw_collision = true;
		
		// General Modules that need to be loaded
		REGISTER_MODULE(command);
		REGISTER_MODULE(d3d9ex);
		REGISTER_MODULE(scheduler);
		REGISTER_MODULE(doart);

		// Addons
		//REGISTER_MODULE(draw_collision);

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void loader::uninitialize_()
	{
		std::ranges::reverse(loader::components_.begin(), loader::components_.end());
		for (const auto component : loader::components_)
		{
			delete component;
		}

		loader::components_.clear();
		loader::component_allocator_.clear();
	}

	void loader::register_(component* component)
	{
		if (component)
		{
			game::glob::loaded_modules.append(utils::va("Component registered: %s\n", component->get_name()));
			loader::components_.push_back(component);
		}
	}

	bool loader::is_registered(const char *componentName)
	{
		std::ranges::reverse(loader::components_.begin(), loader::components_.end());
		for (const auto component : loader::components_)
		{
			if (!strcmp(componentName, component->get_name()))
			{
				return true;
			}
		}

		return false;
	}

	utils::memory::allocator* loader::get_alloctor()
	{
		return &loader::component_allocator_;
	}
}
