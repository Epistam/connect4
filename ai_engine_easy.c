#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/game_ui.h"
#include "include/game_manager.h"
#include "include/ai_engine_easy.h"

// Fonction principale de l'IA : retourne son choix
int ai_easy_compute(BOARD board, int color) { // useless param
	int ai_color = 2, player_color = 1, buffer_col;

	// On complète un quadruplet allié
	buffer_col = ai_easy_quad(board, ai_color);
	if(buffer_col != NO_SOL_AVAIL) return buffer_col;

	// On bloque un quadruplet ennemi
	buffer_col = ai_easy_quad(board, player_color);
	if(buffer_col != NO_SOL_AVAIL) return buffer_col;

	// On complète un triplet allié
	buffer_col = ai_easy_triple(board, ai_color);
	if(buffer_col != NO_SOL_AVAIL) return buffer_col;

	// On bloque un triplet ennemi
	buffer_col = ai_easy_triple(board, player_color);
	if(buffer_col != NO_SOL_AVAIL) return buffer_col;

	// On fait un doublet
	buffer_col = ai_easy_double(board, ai_color);
	if(buffer_col != NO_SOL_AVAIL) return buffer_col;

	// Dernier recours (souvent au premier tour)
	srand(time(NULL));
	return rand()%BOARD_WIDTH; 

}
// Vérifie si un coup permet une victoire directe et renvoie sa colonne
int ai_easy_quad(BOARD board, int color) {

	int j, k, toprow, col;
	BOARD buffer_board;
	int length = 4; // Longueur du motif recherché, ici un quadruplet

	// Complétion de quadruplet
	for(col = 0 ; col < BOARD_WIDTH ; col++) {// On reprend la fonction de condition de victoire pour chaque possibilité de coup
	
		// On détermine le y de la colonne actuelle
		for(toprow = 0 ; toprow < BOARD_HEIGHT && board[col][toprow] != 0 ; toprow++); // Ici toprow est en réalité supérieur à la hauteur du pion le plus haut : on n'y touche pas car on veut la valeur du pion virtuel placé juste au dessus, dont toprow est la hauteur

		if(toprow < BOARD_HEIGHT) { // On vérifie que la colonne soit jouable

			// On copie le tableau dans le tableau tampon
			for(j = 0 ; j < BOARD_WIDTH ; j++) for(k = 0 ; k < BOARD_HEIGHT ; k++) buffer_board[j][k] = board[j][k];
			game_append(buffer_board, col, color);

			if(game_iswon(board, col, color, length)) return col; 

		}
	}
	return NO_SOL_AVAIL;
}

// Vérifie si un coup permet de compéter une suite de 3 pions et renvoie la colonne correspondante
int ai_easy_triple(BOARD board, int color) {
	int j, k, toprow, col;
	BOARD buffer_board;
	int length = 3; // Longueur du motif recherché, ici un quadruplet

	// Complétion de quadruplet
	for(col = 0 ; col < BOARD_WIDTH ; col++) {// On reprend la fonction de condition de victoire pour chaque possibilité de coup
	
		// On détermine le y de la colonne actuelle
		for(toprow = 0 ; toprow < BOARD_HEIGHT && board[col][toprow] != 0 ; toprow++); // Ici toprow est en réalité supérieur à la hauteur du pion le plus haut : on n'y touche pas car on veut la valeur du pion virtuel placé juste au dessus, dont toprow est la hauteur

		if(toprow < BOARD_HEIGHT) { // On vérifie que la colonne soit jouable

			// On copie le tableau dans le tableau tampon
			for(j = 0 ; j < BOARD_WIDTH ; j++) for(k = 0 ; k < BOARD_HEIGHT ; k++) buffer_board[j][k] = board[j][k];
			game_append(buffer_board, col, color);

			if(game_iswon(board, col, color, length)) return col; 

		}
	}
	return NO_SOL_AVAIL;
}

// Vérifie si un coup permet de compléter une série de deux pions et envoie la colonne
int ai_easy_double(BOARD board, int color) {
	int j, k, toprow, col;
	BOARD buffer_board;
	int length = 2; // Longueur du motif recherché, ici un quadruplet

	// Complétion de quadruplet
	for(col = 0 ; col < BOARD_WIDTH ; col++) {// On reprend la fonction de condition de victoire pour chaque possibilité de coup
	
		// On détermine le y de la colonne actuelle
		for(toprow = 0 ; toprow < BOARD_HEIGHT && board[col][toprow] != 0 ; toprow++); // Ici toprow est en réalité supérieur à la hauteur du pion le plus haut : on n'y touche pas car on veut la valeur du pion virtuel placé juste au dessus, dont toprow est la hauteur

		if(toprow < BOARD_HEIGHT) { // On vérifie que la colonne soit jouable

			// On copie le tableau dans le tableau tampon
			for(j = 0 ; j < BOARD_WIDTH ; j++) for(k = 0 ; k < BOARD_HEIGHT ; k++) buffer_board[j][k] = board[j][k];
			game_append(buffer_board, col, color);

			if(game_iswon(board, col, color, length)) return col; 

		}
	}
	return NO_SOL_AVAIL;
}
