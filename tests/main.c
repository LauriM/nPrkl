#include "../nPrkl.h"

#include <stdio.h>

int main()
{
	np_init();

	for(unsigned i = 0; i < 8; ++i)
	{
		np_bg_color(i);
		np_draw(i, i, 'X');
	}

	np_bg_color(NP_BLACK);

	for(unsigned i = 0; i < 8; ++i)
	{
		np_fg_color(i);
		np_draw(10 - i, i, 'X');
	}

	for(unsigned i = 0; i < np_width(); ++i)
	{
		np_fg_color(NP_BLACK);

		np_bg_color(NP_RED);
		np_draw(i, 15, '#');

		np_bg_color(NP_GREEN);
		np_draw(i, 16, '#');

		np_draw_string(0, 20, "Hello world, testing strings!");
	}

	// Testing out "normal" kind of rendering, where the whole scene is rendered from scracth every frame
	// nPrkl is going to handle the caching and making sure only icons that have changed are rendered
	// this way we can prevent annoying flashing
	int pos_x = 5;
	int pos_y = 5;
	for (;;)
	{
		// Fill whole scene with #
		np_fg_color(NP_GREEN);
		np_bg_color(NP_BLACK);

		for (unsigned x = 1; x < np_width() - 1; ++x)
		for (unsigned y = 1; y < np_height() - 1; ++y)
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

		int a = getch();
	}

	np_uninit();

	return 0;
}
