#ifndef NPRKL_H
#define NPRKL_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <Wincon.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#endif

#include <stdlib.h>
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

// State for a single tile
struct s_np_tile_state
{
	char c;
	enum np_color fg_color;
	enum np_color bg_color;
};

// State for the whole nPrkl
struct s_np_state 
{
	unsigned width;
	unsigned height;
	unsigned cursor_pos_x;
	unsigned cursor_pos_y;

	enum np_color fg_color;
	enum np_color bg_color;

#ifdef _WIN32
	HANDLE consoleHandle;
#endif

	struct s_np_tile_state *buffer;
};

static struct s_np_state np_state;

// TODO
// * Support for the HILIGHTED colors
// * Callback for events (screenresize)
// * title change support
// * better error handling
// * Input support
// * Hide cursor functionality
// * static / extern include
// * If cpp is enabled, use namespaces to hide stuff that should not be used

// Currently only one state is supported per application.
// _internal or _shared functions should not be called directly

// Initialize the nPrkl library, should always be called first
void np_init();

// Release memory allocated by nPrkl
void np_uninit();

// Call once per frame
void np_update();

// Write a single char into a certain position
void np_draw(unsigned x, unsigned y, char c);

// Draw string to a certain position
void np_draw_string(unsigned x, unsigned y, char *s);

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
// Shared implementations are used by all different platforms

void np_shared_init()
{
	// Allocate a buffer
	np_state.buffer = (struct s_np_tile_state*)malloc(sizeof(struct s_np_tile_state) * np_state.width * np_state.height);

	assert(np_state.buffer != NULL);

	for (unsigned x = 0; x < np_state.width; ++x)
	{
		for (unsigned y = 0 ; y < np_state.height; ++y)
		{
			struct s_np_tile_state *tile = &np_state.buffer[np_state.width * y + x];

			// set the default colors
			tile->c = ' ';
			tile->fg_color = NP_WHITE;
			tile->bg_color = NP_BLACK;
		}
	}

}

void np_shared_uninit()
{
	free(np_state.buffer);
}

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
	np_state.height = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;

	np_shared_init();
}

void np_uninit()
{

	np_shared_uninit();
}

void np_draw(unsigned x, unsigned y, char c)
{
	assert(x <= np_width());
	assert(y <= np_height());

	// check cache
	struct s_np_tile_state *tile = &np_state.buffer[np_state.width * y + x];

	if (c == tile->c && np_state.bg_color == tile->bg_color && np_state.fg_color == tile->fg_color)
		return; // Nothing changed!

	np_set_cursor_pos(x, y);

	unsigned written;
	WriteConsole(np_state.consoleHandle, &c, 1, &written, NULL);

	assert(written == 1);

	// Update cache
	tile->c = c;
	tile->fg_color = np_state.fg_color;
	tile->bg_color = np_state.bg_color;
}

void np_update()
{
	/* stub */
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

#if defined(__linux__) || defined(__APPLE__)
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

	np_shared_init();
}

void np_uninit()
{
	np_shared_uninit();
}

void np_draw(unsigned x, unsigned y, char c)
{
	assert(x <= np_width());
	assert(y <= np_height());

	// check cache
	struct s_np_tile_state *tile = &np_state.buffer[np_state.width * y + x];

	if (c == tile->c && np_state.bg_color == tile->bg_color && np_state.fg_color == tile->fg_color)
		return; // Nothing changed!

	np_set_cursor_pos(x, y);
	printf("%c", c);

	// Update cache
	tile->c = c;
	tile->fg_color = np_state.fg_color;
	tile->bg_color = np_state.bg_color;
}

void np_update()
{
	np_set_cursor_pos(np_width(), np_height());
	printf("\n");
}

void np_set_cursor_pos(unsigned x, unsigned y)
{
	printf("%c[%d;%df", 0x1B , y, x);
}

void np_fg_color(enum np_color color)
{
	printf("%c[%dm", 0x1B, 30 + color);
}

void np_bg_color(enum np_color color)
{
	printf("%c[%dm", 0x1B, 40 + color);
}

#endif // __linux__

// Implementations that are the same for both platforms

unsigned np_width() { return np_state.width; }

unsigned np_height() { return np_state.height; }

void np_draw_string(unsigned x, unsigned y, char *s)
{
	unsigned len = strlen(s);
	unsigned i = 0;

	while (i < len)
	{
		np_set_cursor_pos(x, y);

		// draw one by one to make sure caching is working
		np_draw(x, y, s[i]);

		++x;
		++i;
	}
}

#ifdef __cplusplus
}
#endif

#endif // NPRKL_H


