#ifndef _COMMUN_H_
#define _COMMUN_H_

#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
//#include <sfil.h>	//may be used in future updates

//#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <sstream>
#include <stdarg.h>
#include <cstdio>

#include "score_font_ttf.h"
#include "main_font_ttf.h"
#include "title_font_ttf.h"

#define C_BLACK RGBA8(0, 0, 0, 255)
#define C_WHITE RGBA8(255, 255, 255, 255)
#define C_GREY RGBA8(127, 127, 127, 255)
#define C_RED RGBA8(255, 0, 0, 255)
#define C_BLUE RGBA8(0, 0, 255, 255)

#define TOP_W 400
#define BOT_W 320
#define HEIGHT 240

#define VERSION "1.0"

extern sftd_font *scoreFont, *mainFont, *titleFont;

void DrawTextCentered(sftd_font *font, float h, u32 color, int size, const char* str);
void DrawTextCenteredF(sftd_font *font, float h, u32 color, int size, const char* str, ...);

#endif