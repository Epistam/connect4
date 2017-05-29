/* pour compiler ce fichier:  gcc –o cocotte cocotte.c libgraphique.c –lglut –lGLU –lGL –lm */

/* indispensable pour compiler un programme utilisant la bibliothèque graphique: */
#include "libgraphique.h"

/* ces points sont les sommets d'une ligne polygonale représentant une cocotte en papier: */
#define  NB_POINTS 9
int points[NB_POINTS][2] = { { 1, 3 }, { 0, 2 }, { 0, 0 }, { 2, 2 }, { 2, 4 }, { 0, 4 }, { 4, 0 }, { 2, 0 }, { 1, 1 } };

/* WINDOW_WIDTH et WINDOW_HEIGHT sont deux pseudo-constantes définies dans libgraphique.h
   qui expriment respectivement la largeur et la hauteur de la zone graphique.
   COTE_MIN sera la plus petite de ces deux dimensions */
#define COTE_MIN (WINDOW_WIDTH < WINDOW_HEIGHT ? WINDOW_WIDTH : WINDOW_HEIGHT)

int main() {
    int i, xOrig, yOrig, xExtr, yExtr;

    /* un peu d'espace pour que le dessin ne soit pas collé aux bords de la zone graphique: */
    int marge = 50;

    /* constante de proportionnalité entre les coordonnées graphiques et celles des points ci-dessus: */
    int rapport = (COTE_MIN - 2 * marge) / 4;

    /* en commençant il faut "allumer" le mode graphique */
    start_graphics();

    /* pour tracer des lignes et écrire des textes on choisit la couleur noire */
    set_drawing_color(color_BLACK);

    /* chaque tour de cette boucle trace un segment joignant les points (xOrig, yOrig) et (xExtr, yExtr) */
    xOrig = rapport * points[0][0] + marge;
    yOrig = rapport * points[0][1] + marge;
    for (i = 1; i < NB_POINTS; i++) {
        xExtr = rapport * points[i][0] + marge;  /* pensez:  X = a.x + b */
        yExtr = rapport * points[i][1] + marge;  /*          Y = a.y + b */

        draw_line(xOrig, yOrig, xExtr, yExtr);

        xOrig = xExtr;
        yOrig = yExtr;
    }

    /* on écrit un texte */
    draw_string(9 * COTE_MIN / 16, 3 * COTE_MIN / 4, "Pour terminer, pressez une touche");

    /* on provoque l'affichage effectif découlant des opérations précédentes
       (qui, jusqu'ici, n'étaient que "virtuelles") */
    update_graphics();

    /* on attend la frappe d'une touche (quelconque) afin que l'utilisateur ait le temps de voir le dessin */
    get_key();

    /* on "éteint" le mode graphique */
    stop_graphics();

    return 0;
}
