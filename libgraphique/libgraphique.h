#ifndef _LIB_GRAPHIQUE_H
#define _LIB_GRAPHIQUE_H

#include <GL/freeglut.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types enumeres
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Liste des couleurs disponibles.
 */
typedef enum t_colors {
  color_BLACK=0,
  color_BLUE=128<<8,
  color_GREEN=128<<16,
  color_CYAN=(128<<16)+(128<<8),
  color_RED=128<<24,
  color_MAGENTA=(128<<24)+(128<<8),
  color_BROWN=(91<<24)+(59<<16)+(17<<8),
  color_LIGHTGRAY=(192<<24)+(192<<16)+(192<<8),
  color_DARKGRAY=(128<<24)+(128<<16)+(128<<8),
  color_LIGHTBLUE=255<<8,
  color_LIGHTGREEN=255<<16,
  color_LIGHTCYAN=(255<<16)+(255<<8),
  color_LIGHTRED=255<<24,
  color_LIGHTMAGENTA=(255<<24)+(255<<8),
  color_YELLOW=(255<<24)+(255<<16),
  color_WHITE=(255<<24)+(255<<16)+(255<<8)
} couleurs;


/**
 * Liste des touches spéciales disponibles. Pour les autres, utiliser le caractère correspondant.
 */
typedef enum t_special_keys {
  key_F1 = (GLUT_KEY_F1<<8),
  key_F2 = (GLUT_KEY_F2<<8),
  key_F3 = (GLUT_KEY_F3<<8),
  key_F4 = (GLUT_KEY_F4<<8),
  key_F5 = (GLUT_KEY_F5<<8),
  key_F6 = (GLUT_KEY_F6<<8),
  key_F7 = (GLUT_KEY_F7<<8),
  key_F8 = (GLUT_KEY_F8<<8),
  key_F9 = (GLUT_KEY_F9<<8),
  key_F10 = (GLUT_KEY_F10<<8),
  key_F11 = (GLUT_KEY_F11<<8),
  key_F12 = (GLUT_KEY_F12<<8),
  key_PAGE_UP = (GLUT_KEY_PAGE_UP<<8),
  key_PAGE_DOWN = (GLUT_KEY_PAGE_DOWN<<8),
  key_HOME = (GLUT_KEY_HOME<<8),
  key_END = (GLUT_KEY_END<<8),
  key_INSERT = (GLUT_KEY_INSERT<<8),
  key_LEFT = (GLUT_KEY_LEFT<<8),
  key_RIGHT = (GLUT_KEY_RIGHT<<8),
  key_UP = (GLUT_KEY_UP<<8),
  key_DOWN = (GLUT_KEY_DOWN<<8),
  key_ESCAPE = 27,
  key_ENTER = 13 // fix
} touches;

/**
 * Liste des polices / tailles disponibles.
 */
typedef enum t_fonts {
  /* font_8_BY_13, */
  /* font_9_BY_15, */
  /* font_TIMES_ROMAN_10, */
  /* font_TIMES_ROMAN_24, */
  font_HELVETICA_10,
  font_HELVETICA_12,
  font_HELVETICA_18
} polices;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion Couleurs
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Change la couleur courante.
 * @param color La nouvelle couleur courante.
 */
void set_drawing_color(couleurs color);

/**
 * Change la couleur du fond.
 * @param color La nouvelle couleur de fond
 */
void set_background_color(couleurs color);

/**
 * Change la couleur de remplissage.
 * @param color La nouvelle couleur de remplissage.
 */
void set_fill_color(couleurs color);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion affichage caracteres
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Change la police de caractère courante.
 * @param font La nouvelle police à utiliser.
 */
void set_font(polices font);

/**
 * Affiche une chaine de caractere c a la position (x,y)
 * @param x
 * @param y
 * @param c
 */
void draw_string(int x, int y, char *c);

/**
 * Affiche une chaine de caractere c utilisant les codes de remplacement de la famille de fonction printf() a la position (x,y)
 * @param x
 * @param y
 * @param c
 * @param ...
 */
void draw_printf(int x, int y, const char *c, ...);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives (publiques)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialisation, démarrage et fermeture ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Initialise GLUT et créer le BUFFER dans lequel sont réalisés les dessins.
 * @param hc Gestionnaire de clavier.
 * @param ha Gestionnaire d'affichage.
 */
void start_graphics();

/**
 * Actualise l'affichage
 */
void update_graphics();

/**
 * Ferme l'affichage graphique.
 */
void stop_graphics();

// Dessin ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Met tous les pixels du BUFFER en blanc.
 */
void clear_screen();

/**
 * Affiche un pixel de coordonnées (x,y).
 * @param x Coordonnées du pixel.
 * @param y Coordonnées du pixel.
 */
void draw_pixel(int x, int y);

/**
 * Affiche une ligne entre les points (x1,y1) et (x2,y2).
 * @param x1 Coordonnées du point 1.
 * @param y1 Coordonnées du point 1.
 * @param x2 Coordonnées du point 2.
 * @param y2 Coordonnées du point 2.
 */
void draw_line(int x1, int y1, int x2, int y2);

/**
 * Affiche un rectangle de coin superieur gauche (x1,y1) et inferieur droit (x2,y2).
 * @param x1 Coordonnées du point 1.
 * @param y1 Coordonnées du point 1.
 * @param x2 Coordonnées du point 2.
 * @param y2 Coordonnées du point 2.
 */
void draw_rectangle(int x1, int y1, int x2, int y2);

/**
 * Affiche un rectangle plein de coin superieur gauche (x1,y1) et inferieur droit (x2,y2).
 * @param x1 Coordonnées du point 1.
 * @param y1 Coordonnées du point 1.
 * @param x2 Coordonnées du point 2.
 * @param y2 Coordonnées du point 2.
 */
void draw_rectangle_full(int x1, int y1, int x2, int y2);

/**
 * Affiche un cercle de centre (x,y) et de rayon r.
 * @param x Coordonnées du centre.
 * @param y Coordonnées du centre.
 * @param r Rayon du cercle.
 */
void draw_circle(int x, int y, int r);

/**
 * Affiche un cercle plein de centre (x,y) et de rayon r.
 * @param x Coordonnées du centre.
 * @param y Coordonnées du centre.
 * @param r Rayon du cercle.
 */
void draw_circle_full(int x, int y, int r);

/**
 * Rempli une surface delimite par couleur_limite.
 * @param x
 * @param y
 * @param couleur_limite
 */
void fill_surface(int x, int y, couleurs couleur_limite);



// Entrées / Sortie /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Renvoi le caractère entré au clavier (fonction bloquante).
 * @return Le code du caractere entré au clavier.
 */
int get_key();



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonction de presentation des capacitées d'affichage
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Affiche la palette des 16 couleurs a partir de la position (x,y) et sur une hauteur t.
 * @param x
 * @param y
 * @param t
 */
void demo_afficher_couleurs(int x, int y, int t);

/**
 * Affiche les figures possibles a partir de la position (x,y).
 * @param x
 * @param y
 */
void demo_afficher_figures(int x, int y);

/**
 * Affiche les caractères de l'alphabet a partir de la position (x,y).
 * @param x
 * @param y
 */
void demo_afficher_caracteres(int x, int y);







#endif
