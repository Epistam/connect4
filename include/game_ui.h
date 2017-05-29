#ifndef GAME_UI_H
#define GAME_UI_H


/********
 * MISC *
 ********/

typedef struct {int x,y;} COORDS; // TODO short int

void ui_init();
void ui_blank(); // Nettoyage affichage

/*********
 * BOXES *
 *********/

#define UI_BOX_MARGIN 5 // Marge interne des boîtes de dialogue

void ui_displaybox(COORDS pos, int xsize, int ysize);
void ui_rmbox(COORDS pos, int xsize, int ysize);

/***************
 * POPUP BOXES *
 ***************/

#define UI_POPUP_H2W 0.7 // Rapport hauteur / largeur

typedef char UI_POPUP_MSG[500];

void ui_popup(UI_POPUP_MSG txt, COORDS pos, float size);
void ui_displaypopup(UI_POPUP_MSG txt, COORDS pos, int xsize, int ysize);

/**************
 * MENU BOXES *
 **************/

#define UI_MENU_H2W 2
#define UI_MENUSIZE 10 // Taille max. du menu

typedef char UI_MENUENTRY[25];
typedef int UI_MENUGREY[UI_MENUSIZE]; // Cases grisées

typedef struct {
	UI_MENUENTRY entry_list[UI_MENUSIZE];
	UI_MENUGREY grey;
} UI_MENU;

int ui_menu(UI_MENU menu, COORDS pos, float size);
int ui_menumove(UI_MENUGREY grey, int current_entry, int increment); // Déplacement du cursuer dans le menu
void ui_displaymenu(UI_MENU menu, COORDS pos, int xsize, int ysize); // Affichage du corps du menu
void ui_displayoptions(UI_MENUENTRY entry_list[UI_MENUSIZE], COORDS pos, int xsize, int ysize); // Affichage des différentes options
void ui_menugrey(UI_MENU menu, COORDS pos, int xsize, int ysize); // Grisage des cases grisées
void ui_menuhover(UI_MENUENTRY entry_list[UI_MENUSIZE], int entry, COORDS pos, int xsize, int ysize); // Animation du curseur
void ui_menudehover(UI_MENUENTRY entry_list[UI_MENUSIZE], int entry, COORDS pos, int xsize, int ysize);

// Initialisation menus
UI_MENU ui_setmenu_ig();
UI_MENU ui_setmenu_main();
UI_MENU ui_setmenu_endgame();

/*********
 * BOARD *
 *********/

#define UI_BOARDMARGIN 3 // Marge internes dans la grille pour le placement des pions

void ui_drawarrow(int col, int color);
void ui_animcol(int color, int lastcol, int col); // Animer le déplacement du colonne à l'autre (IA)


#endif
