// only header include
#include "../nPrkl.h"

#include <stdio.h>

#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

void nprkl_sleep(int s)
{
#ifdef WINDOWS
	Sleep(s);
#else
	usleep(s * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
}

int main()
{
	np_init();

	// Testing out "normal" kind of rendering, where the whole scene is rendered from scracth every frame
	// nPrkl is going to handle the caching and making sure only icons that have changed are rendered
	// this way we can prevent annoying flashing
	int pos_x = 5;
	int pos_y = 5;
	for (;;)
	{
		// Fill almost the whole scene with #
		np_fg_color(NP_GREEN);
		np_bg_color(NP_BLACK);

		for (unsigned x = 2; x < 20; ++x)
		for (unsigned y = 2; y < 20; ++y)
			np_draw(x, y, '#');

		++pos_x;

		np_fg_color(NP_RED);
		np_bg_color(NP_BLACK);

		np_draw(pos_x, pos_y, '@');

		np_fg_color(NP_WHITE);
		np_bg_color(NP_BLACK);

		np_draw_string(1, 1, "Hello world from nPrkl!");

		// Call once per "frame"
		np_update();

		nprkl_sleep(1);
	}

	np_uninit();

	return 0;
}
