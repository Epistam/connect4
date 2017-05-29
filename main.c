#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgraphique/libgraphique.h"
#include "include/game_ui.h"
#include "include/game_manager.h"
#include "include/ai_engine_easy.h"
#include "include/ai_engine_minmax.h"



int main(void) {

	COORDS center;
	center.x = WINDOW_WIDTH/2;
	center.y = WINDOW_HEIGHT/2;

	BOARD board;
	ui_init();

	int game_continues=1;
	int menu_choice;

	// Chargement du  menu principal
	UI_MENU main_menu = ui_setmenu_main();

	// Chargement du menu en jeu
	UI_MENU ig_menu = ui_setmenu_ig();

	// Chargement du menu de fin de jeu
	UI_MENU endgame_menu = ui_setmenu_endgame();

	while(game_continues) {
		ui_blank();
		menu_choice = ui_menu(main_menu, center, 0.3);
		ui_blank();
		int restart = 1;
		int rs;

		switch(menu_choice) {
			case 0:
				while(restart) {
					restart = 0;
					game_init(board);
					rs = game_manager_easyai(board, ig_menu, endgame_menu);
					if(rs == RETURN_EXIT) game_continues = 0;
					if(rs == RETURN_RESTARTGAME) restart = 1;
				}
				break;
			case 1:
				while(restart) {
					restart = 0;
					game_init(board);
					rs = game_manager_minmax(board, ig_menu, endgame_menu);
					if(rs == RETURN_EXIT) game_continues = 0;
					if(rs == RETURN_RESTARTGAME) restart = 1;
				}
				break;
			case 2:
				while(restart) {
					restart = 0;
					game_init(board);
					rs = game_manager_hvh(board, ig_menu, endgame_menu);
					if(rs == RETURN_EXIT) game_continues = 0;
					if(rs == RETURN_RESTARTGAME) restart = 1;
				}
				break;
			case 3:	game_continues = 0;
			case RETURN_ESC: game_continues = 0;
		}
	 }
	stop_graphics();
	return EXIT_SUCCESS;
}
