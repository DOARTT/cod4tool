#pragma once

namespace components
{
    enum FONTS
	{
		BOLD_LARGER		= 0,
		BOLD_LARGE		= 1,
		BOLD			= 2,
		REGULAR_LARGE	= 3,
		REGULAR			= 4,
	};

	namespace fonts
    {
        extern const char* get_font_for_style(int font_style);
    }
}
