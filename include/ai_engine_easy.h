#ifndef AI_ENGINE_EASY_H
#define AI_ENGINE_EASY_H

#define NO_SOL_AVAIL -1 // Aucune solution pour cette sous routine

int ai_easy_compute(BOARD board, int color); // Routine principale

int ai_easy_quad(BOARD board, int color); // Complétion d'un quadruplet (victoire) 
int ai_easy_triple(BOARD board, int color); // Complétion d'un triplet
int ai_easy_double(BOARD board, int color); // Complétion d'un doublet


#endif
