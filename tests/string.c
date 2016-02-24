#include "../nPrkl.h"

int main()
{
	np_init();
  np_bg_color(NP_BLACK);
  
  int cx = 0, cy = 0;
  int mx = 50, my = 50;
  int dir = 0;
  np_draw_string(2, 2, "Pick starting point");
  np_fg_color(NP_GREEN);
  np_draw_string(4, 3, "<+lordkryss> haxe game engine");
  np_draw_string(4, 4, "<@lordkryss> you're a dick telling me to learn lisp"); 
  np_draw_string(4, 5, "<+lordkryss> k00pa: wanna make a start up together?");

  np_fg_color(NP_WHITE);
  np_draw_string(2, 7, "Pick mission"); 
  np_fg_color(NP_RED);
  np_draw_string(4, 8, "<+lordkryss> also fuck word and it's alignments");
  np_draw_string(4, 9, "<+lordkryss> yeah but then why not italian in this instructions? there's fucking greek"); 

  for(;;) ; 
	return 0;
}
