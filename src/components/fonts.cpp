#include "std_include.hpp"

// Included fonts: OpenSans Regular, OpenSans Bold
// Compressed using ImGui's binary_to_compressed_c
// https://www.apache.org/licenses/LICENSE-2.0.html

#define FONT_SMALL_DEV  "fonts/smallDevFont"
#define FONT_BIG_DEV    "fonts/bigDevFont"
#define FONT_CONSOLE    "fonts/consoleFont"
#define FONT_BIG        "fonts/bigFont"
#define FONT_SMALL      "fonts/smallFont"
#define FONT_BOLD       "fonts/boldFont"
#define FONT_NORMAL     "fonts/normalFont"
#define FONT_EXTRA_BIG  "fonts/extraBigFont"
#define FONT_OBJECTIVE  "fonts/objectiveFont"

namespace components
{
	namespace fonts
    {
        // *
		// return font string for style
		const char* get_font_for_style(int font_style)
		{
			return	font_style == 0 ? FONT_SMALL_DEV : font_style == 1 ? FONT_BIG_DEV : font_style == 2 ? FONT_CONSOLE :
				font_style == 3 ? FONT_BIG : font_style == 4 ? FONT_SMALL : font_style == 5 ? FONT_BOLD :
				font_style == 6 ? FONT_NORMAL : font_style == 7 ? FONT_EXTRA_BIG : FONT_OBJECTIVE;
		}
    }
}
