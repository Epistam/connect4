#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

/********
 * MISC *
 ********/

int min(int a, int b);

/***********************
 * STATUS RETURN CODES *
 ***********************/

#define RETURN_EXIT -1
#define RETURN_MAINMENU -2
#define RETURN_RESTARTGAME -3
#define RETURN_ESC -4

/*********
 * BOARD *
 *********/

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

typedef short int BOARD[BOARD_WIDTH][BOARD_HEIGHT]; // 3 valeurs possibles, pas besoin de plus qu'un short

void game_init(BOARD board);
void game_append(BOARD board, int col, int color);
void ui_displayboard(BOARD board); // Devrait être dans game_ui, mais ici à cause des dépendances entre headers et de l'ordre de leur inclusion

/*****************
 * GAME MANAGERS *
 *****************/

int game_manager_easyai(BOARD board, UI_MENU ig_menu, UI_MENU endgame_menu);
int game_manager_minmax(BOARD board, UI_MENU ig_menu, UI_MENU endgame_menu);
int game_manager_hvh(BOARD board, UI_MENU ig_menu, UI_MENU endgame_menu);

/*****************
 * IG MANAGEMENT *
 *****************/

int game_selectcol(UI_MENU ig_menu, BOARD board, int color, int lastcol); // Sélection de colonne par l'utilisateur
int game_iswon(BOARD board, int lastcol, int color, int length); // Condition de victoire
int game_swapcolor(int color);

#endif
