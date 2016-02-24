#ifndef NPRKL_H
#define NPRKL_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <Wincon.h>
#endif

#ifdef __linux__
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#endif

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

enum np_color
{
	NP_BLACK = 0,
	NP_RED,
	NP_GREEN,
	NP_YELLOW,
	NP_BLUE,
	NP_MAGENTA,
	NP_CYAN,
  NP_WHITE
};

struct s_np_state {
	unsigned width;
	unsigned height;
	unsigned cursor_pos_x;
	unsigned cursor_pos_y;

	enum np_color fg_color;
	enum np_color bg_color;

#ifdef _WIN32
	HANDLE consoleHandle;
#endif
};

struct s_np_state np_state;

// TODO
// * Support for the HILIGHTED colors
// * Callback for events (screenresize)
// * Writing strings
// * Buffering stuff for performance (?)
// * title change support
// * better error handling
// * Input support
// * Hide cursor functionality
// * static / extern include

// Initialize the nPrkl library, should always be called first
void np_init();

// Write a single char into a certain position
void np_draw(unsigned x, unsigned y, char c);

// Ok
void np_draws(unsigned x, unsigned y, char * s);

// Set the foreground color
void np_fg_color(enum np_color color);

// Set the background color
void np_bg_color(enum np_color color);

// Set cursor position, used internally
void np_set_cursor_pos(unsigned x, unsigned y);

// Get console width in characters
unsigned np_width();

// Get console height in rows
unsigned np_height();

// #-----------------#
// # implementations #
// #-----------------#

#ifdef _WIN32
void np_init()
{
	// reset everything
	memset(&np_state, 0, sizeof(np_state));

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	np_state.consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(np_state.consoleHandle, &bufferInfo);

	// Get the relevant information out
	np_state.width = bufferInfo.dwSize.X;
	np_state.height = bufferInfo.dwSize.Y;
}

void np_draw(unsigned x, unsigned y, char c)
{
	np_set_cursor_pos(x, y);


	unsigned written;
	WriteConsole(np_state.consoleHandle, &c, 1, &written, NULL);
}

void np_internal_set_color()
{
	WORD value = 0;

	//TODO: add intensity for extra effect

	switch (np_state.fg_color)
	{
	default:
		break;
	case NP_BLACK:
		// not set
		break;
	case NP_RED:
		value += FOREGROUND_RED;
		break;
	case NP_GREEN:
		value += FOREGROUND_GREEN;
		break;
	case NP_YELLOW:
		value += FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case NP_BLUE:
		value += FOREGROUND_BLUE;
		break;
	case NP_MAGENTA:
		value += FOREGROUND_RED | FOREGROUND_BLUE;
		break;
	case NP_CYAN:
		value += FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
  case NP_WHITE:
    value += FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
	}

	switch (np_state.bg_color)
	{
	default:
		break;
	case NP_BLACK:
		// not set
		break;
	case NP_RED:
		value += BACKGROUND_RED;
		break;
	case NP_GREEN:
		value += BACKGROUND_GREEN;
		break;
	case NP_YELLOW:
		value += BACKGROUND_RED | BACKGROUND_GREEN;
		break;
	case NP_BLUE:
		value += BACKGROUND_BLUE;
		break;
	case NP_MAGENTA:
		value += BACKGROUND_RED | BACKGROUND_BLUE;
		break;
	case NP_CYAN:
		value += BACKGROUND_GREEN | BACKGROUND_BLUE;
		break;
  case NP_WHITE:
    value += BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
    break;
	}

	SetConsoleTextAttribute(np_state.consoleHandle, value);
}

void np_fg_color(enum np_color color)
{
	np_state.fg_color = color;

	np_internal_set_color();
}

void np_bg_color(enum np_color color)
{
	np_state.bg_color = color;

	np_internal_set_color();
}

void np_set_cursor_pos(unsigned x, unsigned y)
{
	if (np_state.cursor_pos_x == x && np_state.cursor_pos_y == y)
		return; // Already in position

	COORD pos;

	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(np_state.consoleHandle, pos);
}
#endif // _WIN32

#ifdef __linux__
void np_init()
{
	// reset everything
	memset(&np_state, 0, sizeof(np_state));

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	// Get the relevant information out
	np_state.width = w.ws_row;
	np_state.height = w.ws_col;

	// Clear the whole terminal
	printf("%c[2J", 0x1B);
}

void np_draw(unsigned x, unsigned y, char c)
{
	np_set_cursor_pos(x, y);
	printf("%c", c);
	printf("\n"); // force flush TODO: fix this
}

void np_draw_string(unsigned x, unsigned y, char * s) {
  np_set_cursor_pos(x, y);
  printf("%s", s);
  printf("\n");
}

void np_set_cursor_pos(unsigned x, unsigned y)
{
	printf("%c[%d;%df", 0x1B , y, x);
}

void np_fg_color(enum np_color color)
{
	//switch..
	//   ESC[ … 38;2;<r>;<g>;<b> … m Select RGB foreground color
	printf("%c[%dm", 0x1B, 30 + color);
}

void np_bg_color(enum np_color color)
{
	// 48 is bg
	printf("%c[%dm", 0x1B, 40 + color);
}

#endif // __linux__

unsigned np_width() { return np_state.width; }

unsigned np_height() { return np_state.height; }

#ifdef __cplusplus
}
#endif

#endif // NPRKL_H


