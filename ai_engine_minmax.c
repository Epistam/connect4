#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "include/game_ui.h"
#include "include/game_manager.h"
#include "include/ai_engine_minmax.h"

// Travail non achevé, IA non fonctionnelle

int ai_minmax_compute(BOARD board, int ai_color) {
	int col = 0;
	int max = INT_MIN, temp_max = INT_MIN;
	BOARD buffer_board;
	int i,j;

	// Création d'un tableau virtuel
	for(i = 0 ; i < BOARD_WIDTH ; i++) for(j = 0 ; j < BOARD_HEIGHT ; j++) buffer_board[i][j] = board[i][j];

	for(i = 0 ; i < BOARD_WIDTH ; i++) {
		temp_max = ai_negamax(buffer_board, AI_MAXDEPTH, ai_color);
		if(temp_max > max) {
			col = i;
			max = temp_max;
		}
	}

	return col;
}
/*
int ai_negamax(BOARD board, int col, int ai_color, int depth) {
	// Condition d'arrêt
	if(depth == 0 || game_iswon(board, ai_color, col, 4) || game_iswon(board, game_swapcolor(ai_color), col, 4)) return ai_minmax_eval(board, ai_color);

	int node_value, max = INT_MIN, temp_max = INT_MIN;
	int i;

	for(i = 0 ; i < BOARD_WIDTH ; i++) {
		

		
	}


	return - node_value;
}*/

int ai_negamax(BOARD board, int depth, int color) {
	int temp_max = INT_MIN, max = INT_MIN;
	int i, j;
	for(i = 0 ; i < BOARD_WIDTH ; i++) {
		for(j = 0 ; j < BOARD_HEIGHT && board[i][j] != 0; j++); 

		if(!board[i][BOARD_HEIGHT - 1]) { // == 0 => On peut ajouter un dernier jeton au moins
		
			game_append(board, i, color);
			if(game_iswon(board, i, color, 4) || depth - 1 == 0) {
				board[i][j] = 0;
				return ai_minmax_eval(board, color, i);
			}
			else {
				temp_max = - ai_negamax(board, depth - 1, game_swapcolor(color));
				if(temp_max > max) max = temp_max;
			}

			board[i][j] = 0;
		}
	}

	return max;
}

// Evaluation heuristique
int ai_minmax_eval(BOARD board, int color, int col) {
	if(game_iswon(board, col, color, 4)) return 1000;

	int i, j, k,triplet_counter = 0;

	for(i = 0 ; i < BOARD_WIDTH ; i++) {
		for(j = 0 ; j < BOARD_HEIGHT && board[i][j] != 0; j++);
		// Coup possible ? 
		if(!board[i][BOARD_HEIGHT - 1]) {
			for(k = j ; k < BOARD_HEIGHT ; k++) {
				board[i][j] = color;
			
				triplet_counter += game_iswon(board, i, color, 4);
				board[i][j] = 0;
			}

		}

	}
	return triplet_counter;


// WiP
/*
int ai_minmax_compute(BOARD board, int ai_color) {
	BOARD buffer_board;
	int i,j, buff, max = - INT_MAX, col = 0;
	for(i = 0 ; i < BOARD_WIDTH ; i++) for(j = 0 ; j < BOARD_HEIGHT ; j++) buffer_board[i][j] = board[i][j];

	for(i = 0 ; i < BOARD_WIDTH ; i++) {
		buff = ai_minmax_max(buffer_board, AI_DEPTH, i, ai_color);
		if(buff > max) {
			max = buff;
			col = i;
		}
	}

	return col;
}

int ai_minmax_max(BOARD board, int depth, int lastcol, int ai_color) { // Ici, passer la dernière colonne à chaque fois est plus efficace que d'utiliser une condition de victoire qui vérifie tout 
	if(depth == 0 || game_iswon(board, lastcol, game_swapcolor(ai_color), 4)) return ai_minmax_eval(board, ai_color, lastcol);

	int max = INT_MIN, i, j, tempmax, toprow;

	for(i = 0 ; i < BOARD_WIDTH ; i++) {

		// On détermine le y du dernier coup
		for(j = 0 ; j < BOARD_HEIGHT && board[lastcol][j] != 0; j++);
		toprow = j - 1;
		// Coup possible ? 
		if(toprow < BOARD_HEIGHT - 1) {

			game_append(board, i, ai_color);

			tempmax = ai_minmax_min(board, depth - 1, i, ai_color);

			if(tempmax > max) max = tempmax;

			board[i][j] = 0; // On nettoie le tableau, un pion plus haut parce qu'on en a placé un nouvea

		}
	}
	return max;		
}

int ai_minmax_min(BOARD board, int depth, int lastcol, int ai_color) {
	if(depth == 0 || game_iswon(board, lastcol, ai_color, 4)) return ai_minmax_eval(board, ai_color, lastcol);

	int min = INT_MAX, i, j, tempmin, toprow;

	for(i = 0 ; i < BOARD_WIDTH ; i++) {

		// On détermine le y du dernier coup
		for(j = 0 ; j < BOARD_HEIGHT && board[lastcol][j] != 0; j++);
		toprow = j - 1;
		// Coup possible ? 
		if(toprow < BOARD_HEIGHT - 1) {

			game_append(board, i, game_swapcolor(ai_color));

			tempmin = ai_minmax_max(board, depth - 1, i, ai_color);

			if(tempmin < min) min = tempmin;

			board[i][j] = 0; // On nettoie le tableau, un pion plus haut parce qu'on en a placé un nouvea

		}
	}
	return min;

}
*/

/*
	for(i = 0 ; i < BOARD_WIDTH ; i++) {

		for(j = 0 ; j < BOARD_HEIGHT && board[lastcol][j] != 0; j++);
		topblank = j;
		// Coup possible ? 
		if(topblank < BOARD_HEIGHT) {
			for(j = topblank ; j < BOARD_HEIGHT ; j++) {
				game_append(board, i, game_swapcolor(ai_color));
			
				triplet_counter += game_iswon(board, i, game_swapcolor(ai_color), 4);

			}
			for(j = BOARD_HEIGHT - 1 ; j >= topblank ; j--) board[i][j] = 0;

		}

	}
	score -= triplet_counter;*/

}
