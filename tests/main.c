#include "../nPrkl.h"

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

	for(unsigned i = 0; i < 100; ++i)
	{
		np_fg_color(NP_BLACK);

		np_bg_color(NP_RED);
		np_draw(i, 15, '#');

		np_bg_color(NP_GREEN);
		np_draw(i, 16, '#');

		np_draw_string(0, 20, "Hello world, testing strings!");
	}

	for (;;)
	{
		// Fill whole scene with #
		np_fg_color(NP_GREEN);
		np_bg_color(NP_BLACK);

		for (unsigned x = 1; x < np_width() - 1; ++x)
		for (unsigned y = 1; y < np_height() - 1; ++y)
			np_draw(x, y, '#');
	}

	return 0;
}
