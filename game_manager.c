#include <stdio.h>
#include "libgraphique/libgraphique.h"
#include "include/game_ui.h"
#include "include/game_manager.h"
#include "include/ai_engine_easy.h"
#include "include/ai_engine_minmax.h"


/********
 * MISC *
 ********/
// Retourne le minimum entre a et b
int min(int a, int b) {
	if(a < b) return a;
	else return b;
}
// Retourne le maximum entre a et b
int max(int a, int b) {
	if(a > b) return a;
	else return b;
}

/*********
 * BOARD *
 *********/
// Initialisation du plateau de jeu
void game_init(BOARD board) {
	int i,j;
	for(i = 0 ; i <  BOARD_WIDTH ; i++) for(j = 0 ; j < BOARD_HEIGHT ; j++) board[i][j] = 0;
	ui_blank();
	ui_displayboard(board);
}
// Ajout d'un pion à une colonne donnée
void game_append(BOARD board, int col, int color) { // Append the checked play to the board
	int i;
	for(i = 0 ; i < BOARD_HEIGHT && board[col][i] != 0 ; i++);
	board[col][i] = color;
}
// Affichage du plateau
void ui_displayboard(BOARD board) { // Displays and update on-screen board
	// Dessin grille
	int i, j;
	int currentX=0, currentY=0;
	set_drawing_color(color_BLACK);
	// Lignes
	for(i = 1 ; i < BOARD_WIDTH ; i++) {
		currentX = (WINDOW_WIDTH/BOARD_WIDTH)*i;
		draw_line(currentX, 0, currentX, WINDOW_HEIGHT);
	}
	// Colonnes
	for(i = 1 ; i < BOARD_HEIGHT + 1; i++) { // Une colonne en plus pour l'interface de sélection
		currentY = (WINDOW_HEIGHT/(BOARD_HEIGHT+1))*i;
		draw_line(0, currentY, WINDOW_WIDTH, currentY);
	}
	
	// Nettoyage marge haute
	set_drawing_color(color_WHITE);
	draw_rectangle_full(0, ((BOARD_HEIGHT*WINDOW_HEIGHT)/(BOARD_HEIGHT+1)), WINDOW_WIDTH, WINDOW_HEIGHT);
	
	// Remplissage avec jetons
	set_drawing_color(color_BLACK);
	for(i = 0 ; i < BOARD_WIDTH ; i++) for(j = 0 ; j < BOARD_HEIGHT ; j++) {
		if(board[i][j] == 2) draw_circle_full(WINDOW_WIDTH/(2*BOARD_WIDTH) + i*WINDOW_WIDTH/BOARD_WIDTH, WINDOW_HEIGHT/(2*(BOARD_HEIGHT+1)) + j*WINDOW_HEIGHT/(BOARD_HEIGHT+1), min((WINDOW_WIDTH/(2*BOARD_WIDTH)) - UI_BOARDMARGIN, (WINDOW_HEIGHT/(2*(BOARD_HEIGHT+1)) - UI_BOARDMARGIN)));
		if(board[i][j] == 1) draw_circle(WINDOW_WIDTH/(2*BOARD_WIDTH) + i*WINDOW_WIDTH/BOARD_WIDTH, WINDOW_HEIGHT/(2*(BOARD_HEIGHT+1)) + j*WINDOW_HEIGHT/(BOARD_HEIGHT+1), min((WINDOW_WIDTH/(2*BOARD_WIDTH)) - UI_BOARDMARGIN, (WINDOW_HEIGHT/(2*(BOARD_HEIGHT+1)) - UI_BOARDMARGIN)));
	}
	update_graphics();
}

/*****************
 * GAME MANAGERS *
 *****************/
// Controleur de jeu pour l'IA de l'énoncé : retourne l'action souhaitée après la sortie de la fonction
int game_manager_easyai(BOARD board, UI_MENU ig_menu, UI_MENU endgame_menu) {
	int win = 0, lastcol=0, player_col, return_menu, turn_count = 0, current_player = 1;
	while(!win) {
		// Joueur 1
		// Choix joueur
		lastcol = game_selectcol(ig_menu, board, current_player, lastcol);
		if(lastcol == RETURN_EXIT) return RETURN_EXIT;
		if(lastcol == RETURN_MAINMENU) return RETURN_MAINMENU;
		if(lastcol == RETURN_RESTARTGAME) return RETURN_RESTARTGAME;
		// On applique à la grille et on l'actualise
		game_append(board, lastcol, current_player);
		ui_displayboard(board);
		if(game_iswon(board, lastcol, current_player, 4)) win = 1;
		current_player = game_swapcolor(current_player);
		turn_count++;
		
		if(!win) {
			// Appel IA
			player_col = lastcol;
			lastcol = ai_easy_compute(board, 2);
			ui_animcol(2, player_col, lastcol); 
			game_append(board, lastcol, 2);
			ui_displayboard(board);
			if(game_iswon(board, lastcol, 2, 4)) win = 1;
			current_player = game_swapcolor(current_player);
			turn_count++;
		}
	}
	COORDS center;
	center.x = WINDOW_WIDTH/2;
	center.y = WINDOW_HEIGHT/2;

	if(turn_count == 42 && !win) ui_popup("Match nul", center, 0.35);
	if(current_player == 2 && win) ui_popup("Victoire des blancs", center, 0.35); 
	if(current_player == 1 && win) ui_popup("Victoire des noirs", center, 0.35);

	ui_blank(); // Si jamais le popup est plus gros que le menu

	return_menu = ui_menu(endgame_menu, center, 0.35);
	if(return_menu == 0) return RETURN_RESTARTGAME;
	if(return_menu == 1) return RETURN_MAINMENU;
	if(return_menu == 2) return RETURN_EXIT;
	return EXIT_SUCCESS;
}

// Controleur de jeu pour l'IA heuristique / minmax : retourne l'action souhaitée après la sortie de la fonction
int game_manager_minmax(BOARD board, UI_MENU ig_menu, UI_MENU endgame_menu) {
	int win = 0, lastcol=0, player_col, return_menu, turn_count = 0, current_player = 1;
	while(!win) {
		// Joueur 1
		// Choix joueur
		lastcol = game_selectcol(ig_menu, board, current_player, lastcol);
		if(lastcol == RETURN_EXIT) return RETURN_EXIT;
		if(lastcol == RETURN_MAINMENU) return RETURN_MAINMENU;
		if(lastcol == RETURN_RESTARTGAME) return RETURN_RESTARTGAME;
		// On applique à la grille et on l'actualise
		game_append(board, lastcol, current_player);
		ui_displayboard(board);
		if(game_iswon(board, lastcol, current_player, 4)) win = 1;
		current_player = game_swapcolor(current_player);
		turn_count++;
		
		if(!win) {
			// Appel IA
			player_col = lastcol;
			lastcol = ai_minmax_compute(board, 2);
			ui_animcol(2, player_col, lastcol); 
			game_append(board, lastcol, 2);
			ui_displayboard(board);
			if(game_iswon(board, lastcol, 2, 4)) win = 1;
			current_player = game_swapcolor(current_player);
			turn_count++;
		}
	}
	COORDS center;
	center.x = WINDOW_WIDTH/2;
	center.y = WINDOW_HEIGHT/2;

	if(turn_count == 42 && !win) ui_popup("Match nul", center, 0.35);
	if(current_player == 2 && win) ui_popup("Victoire des blancs", center, 0.35); 
	if(current_player == 1 && win) ui_popup("Victoire des noirs", center, 0.35);

	ui_blank(); // Si jamais le popup est plus gros que le menu

	return_menu = ui_menu(endgame_menu, center, 0.35);
	if(return_menu == 0) return RETURN_RESTARTGAME;
	if(return_menu == 1) return RETURN_MAINMENU;
	if(return_menu == 2) return RETURN_EXIT;
	return EXIT_SUCCESS;
}

// Controleur de jeu pour le jeu humain contre humain : retourne l'action souhaitée après la sortie de la fonction
int game_manager_hvh(BOARD board, UI_MENU ig_menu, UI_MENU endgame_menu) {

	int win = 0, current_player = 1, lastcol=0, return_menu, turn_count = 0;
	while(!win && turn_count < 42) {
		// Joueur 1
		// Choix joueur
		lastcol = game_selectcol(ig_menu, board, current_player, lastcol);
		if(lastcol == RETURN_EXIT) return RETURN_EXIT;
		if(lastcol == RETURN_MAINMENU) return RETURN_MAINMENU;
		if(lastcol == RETURN_RESTARTGAME) return RETURN_RESTARTGAME;
		// On applique à la grille et on l'actualise
		game_append(board, lastcol, current_player);
		ui_displayboard(board);
		if(game_iswon(board, lastcol, current_player, 4)) win = 1;
		// TODO selection nombre de joueurs ? UI barres ?
		current_player = game_swapcolor(current_player);
		turn_count++;
		
	}
	COORDS center;
	center.x = WINDOW_WIDTH/2;
	center.y = WINDOW_HEIGHT/2;

	// ATTENTION : jouers inversés car changement en fin de boucle
	if(turn_count == 42 && !win) ui_popup("Match nul", center, 0.35);
	if(current_player == 2 && win) ui_popup("Victoire des blancs", center, 0.35); 
	if(current_player == 1 && win) ui_popup("Victoire des noirs", center, 0.35);

	ui_blank(); // Si jamais le popup est plus gros que le menu

	return_menu = ui_menu(endgame_menu, center, 0.35);
	if(return_menu == 0) return RETURN_RESTARTGAME;
	if(return_menu == 1) return RETURN_MAINMENU;
	if(return_menu == 2) return RETURN_EXIT;
		
	return EXIT_SUCCESS;
}

/*****************
 * IG MANAGEMENT *
 *****************/

// Retourne la colonne sélectionnée par le joueur en vérifiant au préalable si le coup est possible
int game_selectcol(UI_MENU ig_menu, BOARD board, int color, int lastcol) {
	int selected = 0, key = 42, col = lastcol;
	int temp;
	COORDS center;
	center.x = WINDOW_WIDTH/2;
	center.y = WINDOW_HEIGHT/2;

	ui_drawarrow(col, color);

	while(!selected) {
		key = get_key();
		if(key == key_ESCAPE) {
			temp = ui_menu(ig_menu, center, 0.35);
			ui_displayboard(board);
			ui_drawarrow(col, color);
			if(temp == 3) return RETURN_EXIT;
			if(temp == 2) return RETURN_MAINMENU;
			if(temp == 1) return RETURN_RESTARTGAME;
		}
		if(key == key_LEFT && col > 0) {
			col--;
			ui_drawarrow(col, color);
		}
		if(key == key_RIGHT && col < BOARD_WIDTH - 1) {
			col++;
			ui_drawarrow(col, color);
		}
		if(key == key_ENTER && board[col][BOARD_HEIGHT - 1] == 0) selected = 1;
		else if(key == key_ENTER && board[col][BOARD_HEIGHT - 1] != 0 ) {
			ui_drawarrow(col, 3);
		}
	}
	return col; // escape et check win
}

// Utilisée comme condition de victoire si length = 4, permet autrement de vérifier si le dernier ajout a complété une série d'une longueur donnée ; retourne un boléen corresponant à la réponse à cette question
int game_iswon(BOARD board, int lastcol, int color, int length) {
	int iswon=0, row_counter = 0, line_counter = 0, diag1_counter = 0, diag2_counter = 0, i, toprow;
	
	// On détermine le y du dernier coup
	for(i = 0 ; i < BOARD_HEIGHT && board[lastcol][i] != 0; i++);
	toprow = i - 1;
	
	// Horizotal check
	for(i = - min(lastcol, length - 1) ; i <= min(length - 1, BOARD_WIDTH - lastcol) ; i++) {
		if(board[lastcol + i][toprow] == color) row_counter++;
		else row_counter = 0;
		if(row_counter >= length) iswon = 1;
	}
	// Vertical check
	for(i = - min(toprow, length - 1) ; i <= min(length - 1, BOARD_HEIGHT - toprow) ; i++) {
		if(board[lastcol][toprow + i] == color) line_counter++;
		else line_counter = 0;
		if(line_counter >= length) iswon = 1; 
	}
	// Diag / check
	for(i = - min(length - 1, min(lastcol, toprow)) ; i <= min(length - 1, min(BOARD_WIDTH - lastcol - 1, BOARD_HEIGHT - toprow - 1)) ; i++) {
		if(board[lastcol + i][toprow + i] == color) diag2_counter++;
		else diag2_counter = 0;
		if(diag2_counter >= length) iswon = 1;
	}
	// Diag \ check
	for(i = - min(length - 1, min(lastcol, BOARD_HEIGHT - toprow - 1)) ; i <= min(length - 1, min(BOARD_WIDTH - lastcol - 1, toprow)) ; i++) {
		if(board[lastcol + i][toprow - i] == color) diag1_counter++;
		else diag1_counter = 0;
		if(diag1_counter >= length) iswon = 1; 
	}


	if(iswon) return 1;
	else return 0;
}

// Retourne la couleur du joueur opposé au joueur fourni : utile si on offre la possibilité aux joueurs de choisir leur couleur, entre eux ou contre l'IA
int game_swapcolor(int color) {
	if(color == 1) return 2;
	else return 1;
}
