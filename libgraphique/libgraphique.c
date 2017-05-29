#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdarg.h>
#include "libgraphique.h"
#include "libgraphique_fonts.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_pixel(int x, int y, couleurs color);




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tampon Affichage
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Le tampon d'affichage (matrice de la taille de la fenetre).
 */
GLubyte *SCREEN_BUFFER;
int SCREEN_BUFFER_MODIFIED;

int init_screen_buffer() {
  if ((SCREEN_BUFFER = (GLubyte *)malloc(3*sizeof(GLubyte)*WINDOW_WIDTH*WINDOW_HEIGHT)) == NULL) {
    fprintf(stderr, "! Erreur d'allocation (%s)\n", strerror(errno));
    return(-1);
  }
  return(0);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion Couleurs
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Les couleurs en cours d'utilisation, pour respectivement le dessin (figures et texte), le fond et le remplissage.
 */
couleurs VG_COLOR_DRAWING;
couleurs VG_COLOR_BACKGROUND;
couleurs VG_COLOR_FILL;

/**
 * Initialise les couleurs de dessin et de fond.
 */
void init_colors() {
  set_drawing_color(color_BLACK);
  set_background_color(color_WHITE);
  set_fill_color(color_LIGHTGRAY);
}

/**
 * Change la couleur courante. (publique)
 * @param color La nouvelle couleur courante.
 */
void set_drawing_color(couleurs color) {
  VG_COLOR_DRAWING = color;
}

/**
 * Change la couleur du fond. (publique)
 * @param color La nouvelle couleur de fond.
 */
void set_background_color(couleurs color) {
  VG_COLOR_BACKGROUND = color;
}

/**
 * Change la couleur de remplissage. (publique)
 * @param color La nouvelle couleur de remplissage.
 */
void set_fill_color(couleurs color) {
  VG_COLOR_FILL = color;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion affichage caracteres
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * La police en cours d'utilisation.
 */
const struct font *FONT;

/**
 * Initialise la police d'affichage des caracteres à HELVETICA 10.
 */
void init_font() {
  set_font(font_HELVETICA_10);
}

/**
 * Change la police de caractère courante.
 * @param font La nouvelle police à utiliser.
 */
void set_font(polices font) {
  switch (font) {
  case font_HELVETICA_10 :
    FONT = &FONT_HELVETICA_10;
    break;
  case font_HELVETICA_12 :
    FONT = &FONT_HELVETICA_12;
    break;
  case font_HELVETICA_18 :
    FONT = &FONT_HELVETICA_18;
    break;
  default :
    fprintf(stderr, "set_font : police de caractère inconnue (%d)\n", font);
  }
}

/**
 * Affiche une chaine de caractere c a la position (x,y)
 * @param x
 * @param y
 * @param c
 */
void draw_string(int x, int y, char *c) {
  int w, h = FONT->height, x1, y1;
  int i, i1, j, k, l;
  for (i=0; i<strlen(c); i++) {
    i1 = c[i];
    if ((i1 >= 0) && (i1<256)) {
      // Affichage caractere
      w = FONT->faces[i1][0];
      for (j=0; j<h; j++) {
	for (k=0; k<(w/8+(w%8!=0?1:0)); k++) {
	  for (l=0; l<8; l++) {
	    if ((FONT->faces[i1][1+j*(w/8+(w%8!=0?1:0))+k] & (GLubyte)1<<l) != 0) {
	      y1 = j;
	      x1 = 8*(k+1)-l;
	      set_pixel(x+x1,y+y1, VG_COLOR_DRAWING);
	    }
	  }
	}
      }
      x += w;
    }
  }
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche une chaine de caractere c utilisant les codes de remplacement de la famille de fonction printf() a la position (x,y)
 * @param x
 * @param y
 * @param c
 * @param ...
 */
void draw_printf(int x, int y, const char *c, ...) {
  va_list args;
  char tmp[4096];
  va_start(args, c);
  vsprintf(tmp, c, args);
  va_end(args);
  draw_string(x, y, tmp);
  SCREEN_BUFFER_MODIFIED = 1;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tampon Clavier (fifo)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Le tampon des caracteres entres au clavier.
 */
int SIZE_TAMPON_CLAVIER;
int *TAMPON_CLAVIER;

/**
 * Initialise le buffer du clavier.
 */
void init_key_buffer() {
  SIZE_TAMPON_CLAVIER = 0;
  TAMPON_CLAVIER = NULL;
}

/**
 * Ajouter un caractere au buffer du clavier.
 * @param key Le caractere a ajouter.
 */
void empiler_key(int key) {
  int *tmp;
  if ((tmp = (int *)malloc((SIZE_TAMPON_CLAVIER+1)*sizeof(int))) == NULL) {
    fprintf(stderr, "! erreur d'allocation (%s)\n", strerror(errno));
    return;
  }
  if (SIZE_TAMPON_CLAVIER > 0) {
    memcpy(tmp, TAMPON_CLAVIER, SIZE_TAMPON_CLAVIER*sizeof(int));
    free(TAMPON_CLAVIER);
  }
  TAMPON_CLAVIER = tmp;
  TAMPON_CLAVIER[SIZE_TAMPON_CLAVIER] = key;
  SIZE_TAMPON_CLAVIER++;
}

/**
 * Enlever le caractere du buffer du clavier.
 * @return le premier caractere du buffer du clavier (EOF si le buffer est vide).
 */
int depiler_key() {
  int ret=EOF;
  if (SIZE_TAMPON_CLAVIER == 0)
    return(ret);
  else {
    ret = TAMPON_CLAVIER[0];
    SIZE_TAMPON_CLAVIER--;
    if (SIZE_TAMPON_CLAVIER > 0) {
      int *tmp;
      if ((tmp = (int *)malloc(SIZE_TAMPON_CLAVIER*sizeof(int))) == NULL) {
	fprintf(stderr, "! erreur d'allocation (%s)\n", strerror(errno));
	return(EOF);
      }
      memcpy(tmp, TAMPON_CLAVIER+1, SIZE_TAMPON_CLAVIER*sizeof(int));
      free(TAMPON_CLAVIER);
      TAMPON_CLAVIER = tmp;
    } else {
      free(TAMPON_CLAVIER);
      TAMPON_CLAVIER = NULL;
    }
  }
  return(ret);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Handlers
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _call_reshape(int h, int w) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLfloat)WINDOW_WIDTH, 0.0, (GLfloat)WINDOW_HEIGHT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0,0,h,w);
}

void _call_display() {  
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos2i(0,0);
  glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, SCREEN_BUFFER);
  glFlush();
}

void _call_keyboard(unsigned char key, int x, int y) {
  //  printf("hk::touche %d\n", key);
  empiler_key(key);
}

void _call_keyboard_special(int key, int x, int y) {
  //  printf("hks::touche %d\n", key);
  empiler_key((key<<8));
}
  



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives (publiques)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialisation, démarrage et fermeture ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GRAPHIC = 0;
int WINDOW = -1;

/**
 * Initialise GLUT et créer le BUFFER dans lequel sont réalisés les dessins.
 * @param hc Gestionnaire de clavier.
 * @param ha Gestionnaire d'affichage.
 */
void start_graphics() {
  if (!GRAPHIC) {
    // Initialisation tampon IO
    init_key_buffer();
    // Initialisation tampon affichage
    init_screen_buffer();
    SCREEN_BUFFER_MODIFIED = 0;
    // Initialisation couleurs
    init_colors();
    // Initialisation police de caracteres
    init_font();
    // Initialisation GLUT
    int tmp=0;
    glutInit(&tmp, NULL);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(200, 100);
    WINDOW = glutCreateWindow("Affichage graphique");
    // Initialisation handlers
    glutReshapeFunc(_call_reshape);
    glutDisplayFunc(_call_display);
    glutKeyboardFunc(_call_keyboard);
    glutSpecialFunc(_call_keyboard_special);
    // Efface ecran
    clear_screen();
    // Un tour de boucle pour lancer l'affichage
    glutMainLoopEvent();
    GRAPHIC = 1;
  } else {
    fprintf(stderr, "! start_graphics : Une fenetre graphique est deja active.\n");
  }
}

/**
 * Actualise l'affichage
 */
void update_graphics() {
  if (SCREEN_BUFFER_MODIFIED) {
    glutPostRedisplay();
    glutMainLoopEvent();
    SCREEN_BUFFER_MODIFIED = 0;
  }
}

/**
 * Ferme l'affichage graphique.
 */
void stop_graphics() {
  if (GRAPHIC) {
    glutDestroyWindow(WINDOW);
    glutMainLoopEvent();
  } else {
    fprintf(stderr, "! stop_graphics : Pas de fenetre graphique active.\n");
  }
}


// Dessin ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_pixel(int x, int y, couleurs color) {
  if ((x>=0) && (x<WINDOW_WIDTH) && (y>=0) && (y<WINDOW_HEIGHT)) {
    SCREEN_BUFFER[3*(y*WINDOW_WIDTH+x)+0] = (unsigned int)((color&0xFF000000)>>24);
    SCREEN_BUFFER[3*(y*WINDOW_WIDTH+x)+1] = (unsigned int)((color&0xFF0000)>>16);
    SCREEN_BUFFER[3*(y*WINDOW_WIDTH+x)+2] = (unsigned int)((color&0xFF00)>>8);
  }
}

couleurs get_pixel(int x, int y) {
  couleurs col=0;
  col += ((SCREEN_BUFFER[3*(y*WINDOW_WIDTH+x)+0]&0xFF)<<24);
  col += ((SCREEN_BUFFER[3*(y*WINDOW_WIDTH+x)+1]&0xFF)<<16);
  col += ((SCREEN_BUFFER[3*(y*WINDOW_WIDTH+x)+2]&0xFF)<<8);
  return(col);
}


/**
 * Met tous les pixels du BUFFER en blanc.
 */
void clear_screen() {
  int i;
  for (i=0; i<WINDOW_WIDTH*WINDOW_HEIGHT; i++)
    set_pixel(i/WINDOW_HEIGHT, i%WINDOW_HEIGHT, VG_COLOR_BACKGROUND);
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche un pixel de coordonnées (x,y).
 * @param x Coordonnées du pixel.
 * @param y Coordonnées du pixel.
 */
void draw_pixel(int x, int y) {
  set_pixel(x,y, VG_COLOR_DRAWING);
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche une ligne entre les points (x1,y1) et (x2,y2).
 * @param x1 Coordonnées du point 1.
 * @param y1 Coordonnées du point 1.
 * @param x2 Coordonnées du point 2.
 * @param y2 Coordonnées du point 2.
 */
void draw_line(int x1, int y1, int x2, int y2) {
  int x, y, j, sx, sy, l;
  // Calcul sens sx et sy
  if (x1 != x2)
    sx = (x2-x1)/abs(x2-x1);
  else
    sx = 1;
  if (y1 != y2)
    sy = (y2-y1)/abs(y2-y1);
  else
    sy = 1;
  // Calcul longueur
  l = sqrt(abs(x2-x1)*abs(x2-x1) + abs(y2-y1)*abs(y2-y1));
  // Dessin ligne
  for (j=0; j<l; j++) {
    x = x1+ sx*j*((float)abs(x2-x1)/l);
    y = y1+ sy*j*((float)abs(y2-y1)/l);
    set_pixel(x,y, VG_COLOR_DRAWING);
  }
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche un rectangle de coin superieur gauche (x1,y1) et inferieur droit (x2,y2).
 * @param x1 Coordonnées du point 1.
 * @param y1 Coordonnées du point 1.
 * @param x2 Coordonnées du point 2.
 * @param y2 Coordonnées du point 2.
 */
void draw_rectangle(int x1, int y1, int x2, int y2) {
  int j, sx, sy;
  // Calcul sens sx et sy
  if (x1 != x2)
    sx = (x2-x1)/abs(x2-x1);
  else
    sx = 1;
  if (y1 != y2)
    sy = (y2-y1)/abs(y2-y1);
  else
    sy = 1;
  for (j=0; j<=abs(x2-x1); j++) {
    set_pixel((x1+sx*j), y1, VG_COLOR_DRAWING);
    set_pixel((x1+sx*j), y2, VG_COLOR_DRAWING);
  }
  for (j=0; j<=abs(y2-y1); j++) {
    set_pixel(x1, y1+sy*j, VG_COLOR_DRAWING);
    set_pixel(x2, y1+sy*j, VG_COLOR_DRAWING);
  }
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche un rectangle plein de coin superieur gauche (x1,y1) et inferieur droit (x2,y2).
 * @param x1 Coordonnées du point 1.
 * @param y1 Coordonnées du point 1.
 * @param x2 Coordonnées du point 2.
 * @param y2 Coordonnées du point 2.
 */
void draw_rectangle_full(int x1, int y1, int x2, int y2) {
  int i, j, sx, sy;
  // Calcul sens sx et sy
  if (x1 != x2)
    sx = (x2-x1)/abs(x2-x1);
  else
    sx = 1;
  if (y1 != y2)
    sy = (y2-y1)/abs(y2-y1);
  else
    sy = 1;
  for (i=0; i<=abs(x2-x1); i++) {
    for (j=0; j<=abs(y2-y1); j++) {
      set_pixel(x1+sx*i, y1+sy*j, VG_COLOR_DRAWING);
    }
  }
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche un cercle de centre (x,y) et de rayon r.
 * @param x Coordonnées du centre.
 * @param y Coordonnées du centre.
 * @param r Rayon du cercle.
 */
void draw_circle(int x, int y, int r) {
  if (r==0) {
    set_pixel(x,y, VG_COLOR_DRAWING);
  } else {
    double j, inc = asin((double)1/r);
    for (j=0; j<360; j+=inc) {
      set_pixel((x+cos(j)*r), (y+sin(j)*r), VG_COLOR_DRAWING);
    }
  }
  SCREEN_BUFFER_MODIFIED = 1;
}

/**
 * Affiche un cercle plein de centre (x,y) et de rayon r.
 * @param x Coordonnées du centre.
 * @param y Coordonnées du centre.
 * @param r Rayon du cercle.
 */
void draw_circle_full(int x, int y, int r) {
  int i, j;
  for (i=0; i<r; i++) {
    for (j=0; j<sqrt(r*r - i*i); j++) {
      set_pixel(x+j, y+i, VG_COLOR_DRAWING);
      set_pixel(x-j, y+i, VG_COLOR_DRAWING);
      set_pixel(x+j, y-i, VG_COLOR_DRAWING);
      set_pixel(x-j, y-i, VG_COLOR_DRAWING);
    }
  }
  SCREEN_BUFFER_MODIFIED = 1;
}



void fill_surface_r(int x, int y, couleurs couleur_limite) {
  // Verification coordonnees
  if ((x<0) || (x>=WINDOW_WIDTH) || (y<0) || (y>=WINDOW_HEIGHT))
    return;
  // Verification couleur limite
  if (get_pixel(x,y) == couleur_limite)
    return;
  // Verification deja rempli
  if (get_pixel(x,y) == VG_COLOR_FILL)
    return;
  // Remplissage et propagation
  set_pixel(x,y,VG_COLOR_FILL);
  fill_surface_r(x,y+1,couleur_limite);
  fill_surface_r(x,y-1,couleur_limite);
  fill_surface_r(x+1,y,couleur_limite);
  fill_surface_r(x-1,y,couleur_limite);
}

/**
 * Rempli une surface delimite par couleur_limite.
 * @param x
 * @param y
 * @param couleur_limite
 */
void fill_surface(int x, int y, couleurs couleur_limite) {
  fill_surface_r(x,y,couleur_limite);
  SCREEN_BUFFER_MODIFIED = 1;
}



// Entrées / Sortie /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Renvoi le caractère entré au clavier (fonction bloquante).
 * @return Le code du caractere entré au clavier.
 */
int get_key() {
  int key=-1;
  glutMainLoopEvent();
  key = depiler_key();
  while (key == EOF) {
#ifdef _WINDOWS_
    Sleep(10);
#else
    usleep(10000);
#endif
    glutMainLoopEvent();
    key = depiler_key();
  }
  return(key);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonction de presentation des capacitées d'affichage
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Affiche la palette des 16 couleurs a partir de la position (x,y) et sur une hauteur t.
 * @param x
 * @param y
 * @param t
 */
void demo_afficher_couleurs(int x, int y, int t) {
  int i;
  couleurs couleurs[16] = {color_BLACK, color_BLUE, color_GREEN, color_CYAN, color_RED, color_MAGENTA, color_BROWN, color_LIGHTGRAY, color_DARKGRAY, color_LIGHTBLUE, color_LIGHTGREEN, color_LIGHTCYAN, color_LIGHTRED, color_LIGHTMAGENTA, color_YELLOW, color_WHITE};
  for (i=0; i<16; i++) {
    set_drawing_color(color_BLACK);
    draw_rectangle(x,y,x+t,y+t);
    set_fill_color(couleurs[i]);
    fill_surface(x+1,y+1,color_BLACK);
    x += t;
  }
}

/**
 * Affiche les figures possibles a partir de la position (x,y).
 * @param x
 * @param y
 */
void demo_afficher_figures(int x, int y) {
  set_drawing_color(color_BLACK);
  draw_pixel(x, y+15);
  draw_line(x+5,y, x+15,y+30);
  draw_rectangle(x+20,y,x+30,y+30);
  draw_rectangle_full(x+40,y,x+50,y+30);
  //  set_fill_color(color_BLACK);
  //  fill_surface(x+41,y+1,color_BLACK);
  draw_circle(x+80,y+15,15);
  draw_circle_full(x+115, y+15, 15);
  //  fill_surface(x+115,y+15,color_BLACK);
}

/**
 * Affiche les caractères de l'alphabet a partir de la position (x,y).
 * @param x
 * @param y
 */
void demo_afficher_caracteres(int x, int y) {
  set_drawing_color(color_BLACK);
  set_font(font_HELVETICA_12);
  draw_string(x+30,y,"abcdefghijklmnopqrstuvwxyz");
  draw_string(x,y-15,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  draw_string(x+75,y-30,"1234567890");
}
