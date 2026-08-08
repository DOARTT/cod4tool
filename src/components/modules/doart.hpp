#pragma once

namespace components
{
	class doart: public component
	{
	public:
		doart();
		~doart() = default;
		const char* get_name() override { return "doart_module_mp"; }

	private:
	};
}