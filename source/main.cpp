#include "commun.h"
#include "Game.hpp"

sftd_font *scoreFont, *mainFont, *titleFont;

void DrawTextCentered(sftd_font *font, float h, u32 color, int size, const char* str)
{
	int width = 400;
	if(sf2d_get_current_screen() == GFX_BOTTOM)
		width = 320;

	sftd_draw_text(font, width/2-sftd_get_text_width(font, size, str)/2, h, color, size, str);
}

void DrawTextCenteredF(sftd_font *font, float h, u32 color, int size, const char* str, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, str);
	vsnprintf(buffer, 256, str, args);
	DrawTextCentered(font, h, color, size, buffer);
	va_end(args);
}

void initFonts()
{
	scoreFont = sftd_load_font_mem(score_font_ttf, score_font_ttf_size);
	mainFont = sftd_load_font_mem(main_font_ttf, main_font_ttf_size);
	titleFont = sftd_load_font_mem(title_font_ttf, title_font_ttf_size);
}

int main()
{
	srand(time(0));

	sf2d_init();
	sftd_init();

	initFonts();

	Game *game = new Game;
	game->Opening();

	while (aptMainLoop()) {
		game->CheckInputs();
		game->Update();
		game->Draw();

		int state = game->getState();
		if(state == ST_EXIT)
			break;
		if(state == ST_MENU_RESET)
		{
			delete game;
			game = new Game;
		}
	}

	sftd_free_font(scoreFont);
	sftd_free_font(mainFont);
	sftd_free_font(titleFont);

	sftd_fini();
	sf2d_fini();

	return 0;
}