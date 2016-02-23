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
	}

	for(;;)
		;

	return 0;
}
