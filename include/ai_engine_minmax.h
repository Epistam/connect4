#ifndef AI_ENGINE_MINMAX_H
#define AI_ENGINE_MINMAX_H

#define AI_MAXDEPTH 3

int ai_minmax_compute(BOARD board, int ai_color);
/*
int ai_minmax_max(BOARD board, int depth, int lastcol, int color);
int ai_minmax_min(BOARD board, int depth, int lastcol, int color);
int ai_minmax_eval(BOARD board, int color, int lastcol);
*/

int ai_negamax(BOARD board, int depth, int color);
int ai_minmax_eval(BOARD board, int color, int col);
#endif
