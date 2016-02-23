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

struct s_np_color {
	unsigned r;
	unsigned g;
	unsigned b;
};

struct s_np_state {
	unsigned width;
	unsigned height;
	unsigned cursor_pos_x;
	unsigned cursor_pos_y;

	struct s_np_color fg_color;
	struct s_np_color bg_color;

#ifdef _WIN32
	HANDLE consoleHandle;
#endif
};

struct s_np_state np_state;

// TODO
// * Callback for events (screenresize)
// * Writing strings
// * Colors
// * Buffering for more performance
// * Linux support
// * title change support
// * error handling
// * Check for the VIRTUAL_TERMINAL_PROCESSING support on the platform (win32)
// * !! INPUT !!

// Initialize the nPrkl library, should always be called first
void np_init();

// Write a single char into a certain position
void np_draw(unsigned x, unsigned y, char c);

void np_fg_color(unsigned r, unsigned g, unsigned b);
void np_bg_color(unsigned r, unsigned g, unsigned b);

// Set cursor position, used internally
void np_set_cursor_pos(unsigned x, unsigned y);

// Get console width in characters
unsigned np_width();

// Get console height in rows
unsigned np_height();

#ifdef _WIN32
void np_init()
{
	// reset everything
	memset(&np_state, 0, sizeof(np_state));

	// default foreground should be white
	np_state.fg_color.r = 1;
	np_state.fg_color.g = 1;
	np_state.fg_color.b = 1;

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

	if (np_state.fg_color.r > 127)
		value += FOREGROUND_RED;

	if (np_state.fg_color.g > 127)
		value += FOREGROUND_GREEN;

	if (np_state.fg_color.b > 127)
		value += FOREGROUND_BLUE;

	if (np_state.bg_color.r > 127)
		value += BACKGROUND_RED;

	if (np_state.bg_color.g > 127)
		value += BACKGROUND_GREEN;

	if (np_state.bg_color.b > 127)
		value += BACKGROUND_BLUE;

	SetConsoleTextAttribute(np_state.consoleHandle, value);
}

void np_fg_color(unsigned r, unsigned g, unsigned b)
{
	np_state.fg_color.r = min(r, 255);
	np_state.fg_color.g = min(g, 255);
	np_state.fg_color.b = min(b, 255);

	np_internal_set_color();
}

void np_bg_color(unsigned r, unsigned g, unsigned b)
{
	np_state.bg_color.r = min(r, 255);
	np_state.bg_color.g = min(g, 255);
	np_state.bg_color.b = min(b, 255);

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

	// default foreground should be white
	np_state.fg_color.r = 1;
	np_state.fg_color.g = 1;
	np_state.fg_color.b = 1;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	// Get the relevant information out
	np_state.width = w.ws_row;
	np_state.height = w.ws_col;
}

void np_draw(unsigned x, unsigned y, char c)
{

}

void np_set_cursor_pos(unsigned x, unsigned y)
{

}

#endif // __linux__

unsigned np_width() { return np_state.width; }

unsigned np_height() { return np_state.height; }

#endif
