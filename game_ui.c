#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libgraphique/libgraphique.h"
#include "include/game_ui.h"
#include "include/game_manager.h"

// TODO : refacto toutes les fonctions (sauf ui_set*) en utilisant la struct UI_MENU
// TODO : fix bug : si UI_MENUSIZE != 10, l'affichage par en couilles (mb def statique...)

/********
 * MISC *
 ********/
// Initialisation de l'interface utilisateur
void ui_init() {
	start_graphics();
	ui_blank();
}
// Nettoie l'affichage
void ui_blank() {
	set_drawing_color(color_WHITE);
	draw_rectangle_full(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

/**********
 * BOXES *
 *********/

// Affichage du corps d'une boîte de dialogue, menu ou popup
void ui_displaybox(COORDS pos, int xsize, int ysize) { // mode = 0 for popup, 1 for menu


	// Clean the menu area
	set_drawing_color(color_WHITE);
	draw_rectangle_full(pos.x - xsize/2, pos.y - ysize/2, pos.x + xsize/2, pos.y + ysize/2);

	// Draw the main rectangle
	set_drawing_color(color_BLACK);
	draw_rectangle(pos.x - xsize/2, pos.y - ysize/2, pos.x + xsize/2, pos.y + ysize/2);
	
	// Header drawing
	draw_rectangle(pos.x - xsize/2, pos.y + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2, pos.y + ysize/2); // Top 1/5 of the window

}

// Effacement d'une boîte de dialogue
void ui_rmbox(COORDS pos, int xsize, int ysize) {
	// Clean the previous the menu area
	set_drawing_color(color_WHITE);
	draw_rectangle_full(pos.x - xsize/2, pos.y - ysize/2, pos.x + xsize/2, pos.y + ysize/2);
	
	update_graphics();
}

/**********
 * POPUPS *
 **********/

// Procédure d'appel popup principale
void ui_popup(UI_POPUP_MSG txt, COORDS pos, float size) {

	// Applying global modifier
	int xsize = WINDOW_WIDTH*size; // float / int ???
	int ysize;
	
	ysize = xsize * UI_POPUP_H2W;
	
	int key = 42;
	
	ui_displaypopup(txt, pos, xsize, ysize);
	
	while(key != key_ESCAPE && key != key_ENTER) key = get_key();

	ui_rmbox(pos, xsize, ysize);
}
// Affichage du popup et de son message
void ui_displaypopup(UI_POPUP_MSG txt, COORDS pos, int xsize, int ysize) {


	ui_displaybox(pos, xsize, ysize);

	set_drawing_color(color_BLACK);
	// Displaying box title
	set_font(font_HELVETICA_18);
	draw_string(pos.x - 40, pos.y + 4*(ysize/UI_MENUSIZE) - 10 , "Message"); // Placing message at the center of the top 1/5 rectangle, and then offsetting it by half its size to make it truly centered


	// Displaying popup content
	set_font(font_HELVETICA_12);
	draw_string(pos.x - xsize / 2 + UI_BOX_MARGIN , pos.y + 3*(ysize/UI_MENUSIZE) - UI_BOX_MARGIN - 15, txt); // Newline management ????

	update_graphics();
}


/*********
 * MENUS * 
 *********/
// Fonction d'affichage menu principale : retourne la sélection faire par l'utilisateur
int ui_menu(UI_MENU menu, COORDS pos, float size) {
	int key = 42, current_entry = 0, old_entry = 0; // Current cursor position for menu nav


	// Applying global modifier
	int xsize = WINDOW_WIDTH*size; // float / int ???
	int ysize;

	ysize = xsize * UI_MENU_H2W;

	ui_displaymenu(menu, pos, xsize, ysize);

	ui_menuhover(menu.entry_list, 0, pos, xsize, ysize);
	update_graphics();
	while(key != key_ENTER && key != key_ESCAPE) {
		key = get_key();
		if(key == key_DOWN) {
			old_entry = current_entry;
			current_entry = ui_menumove(menu.grey, current_entry, 1);
		}
		if(key == key_UP) {
			old_entry = current_entry;
			current_entry = ui_menumove(menu.grey, current_entry, -1);
		}
		ui_menudehover(menu.entry_list, old_entry, pos, xsize, ysize);
		ui_menuhover(menu.entry_list, current_entry, pos, xsize, ysize);
		update_graphics();
	}

	ui_rmbox(pos, xsize, ysize);

	if(key == key_ENTER) return current_entry;
	else return RETURN_ESC; // key == key_ESCAPE -> première option, retour au niveau précédent / jeu
}

// Opère le mouvement du curseur de bas en haut après action de l'utilisateur, en prenant en compte les cases grisées et les limites supérieures et inférieures du menu
// Retourne le numéro de la nouvelle entrée après action de l'utilisateur
// Ici on ajoute UI_MENUSIZE à toutes les comparaisons avec modulo pour éviter de se retrouver avec un membre négatif
int ui_menumove(UI_MENUGREY grey, int current_entry, int increment) { // increment = -1 pour decrem, +1 pour increm
	int pass_count=0, buffer_entry = current_entry;
	while(pass_count < UI_MENUSIZE) {
		if(!grey[(buffer_entry + increment + UI_MENUSIZE)%UI_MENUSIZE]) {
			return (buffer_entry + increment + UI_MENUSIZE)%UI_MENUSIZE; // Si entrée suivante non grisée
		}
		else buffer_entry = (buffer_entry + increment + UI_MENUSIZE) % UI_MENUSIZE;
		pass_count++;
	}
	return current_entry; // Si toutes cases grisées sauf actuelle 
}

// Affichage du menu en tant que tel
void ui_displaymenu(UI_MENU menu, COORDS pos, int xsize, int ysize) {
	ui_displaybox(pos, xsize, ysize);

	set_drawing_color(color_BLACK);
	// Displaying box title
	set_font(font_HELVETICA_18);
	draw_string(pos.x - 20, pos.y + 4*(ysize/UI_MENUSIZE), "Menu");

	ui_displayoptions(menu.entry_list, pos, xsize, ysize);
	ui_menugrey(menu, pos, xsize, ysize);

	update_graphics();
}

// Affichage des différentes options du menu (texte + délimitations)
void ui_displayoptions(UI_MENUENTRY entry_list[UI_MENUSIZE], COORDS pos, int xsize, int ysize) {
	int option_height = ((4*ysize)/(5*UI_MENUSIZE)); // Using euclidian division to work around floaty results
	int i;

	set_drawing_color(color_BLACK);

	// Ici, le reste de la division euclidienne pour option_height se traduit par une marge potentielle en bas des différentes options. Pour homogénéiser le tout, on applique la marge générique horizontale pour les boutons.
	for(i = 0 ; i < UI_MENUSIZE ; i++) {
		// Dessin contour bouton
		draw_rectangle(pos.x - xsize/2 + UI_BOX_MARGIN, pos.y - i*option_height + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2 - UI_BOX_MARGIN, pos.y - (i+1)*option_height + 3*(ysize/UI_MENUSIZE));
		// Contenu texte bouton
		draw_string(pos.x - xsize/2 + 2*UI_BOX_MARGIN, pos.y - (i+1)*option_height + 3*(ysize/UI_MENUSIZE) + UI_BOX_MARGIN, entry_list[i]); // Flemme de centrer / ici i+1 car i est la limite supérieure du rectangle dans l'instruction précédente
	}
}

// Grisage des cases grisées
void ui_menugrey(UI_MENU menu, COORDS pos, int xsize, int ysize) {

	int option_height = ((4*ysize)/(5*UI_MENUSIZE)); // Using euclidian division to work around floaty results
	int i;
	for(i = 0 ; i < UI_MENUSIZE ; i++) {
		if(menu.grey[i]) {
			set_drawing_color(color_LIGHTGRAY);
			// Bouton
			draw_rectangle_full(pos.x - xsize/2 + UI_BOX_MARGIN, pos.y - i*option_height + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2 - UI_BOX_MARGIN, pos.y - (i+1)*option_height + 3*(ysize/UI_MENUSIZE));
			set_drawing_color(color_BLACK);
			draw_rectangle(pos.x - xsize/2 + UI_BOX_MARGIN, pos.y - i*option_height + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2 - UI_BOX_MARGIN, pos.y - (i+1)*option_height + 3*(ysize/UI_MENUSIZE));
			// Texte
			set_drawing_color(color_WHITE);
			draw_string(pos.x - xsize/2 + 2*UI_BOX_MARGIN, pos.y - (i+1)*option_height + 3*(ysize/UI_MENUSIZE) + UI_BOX_MARGIN, menu.entry_list[i]); // Flemme de centrer / ici i+1 car i est la limite supérieure du rectangle dans l'instruction précédente
		}
	}
}

// Animation de survol quand le curseur est sur une entrée
void ui_menuhover(UI_MENUENTRY entry_list[UI_MENUSIZE], int entry, COORDS pos, int xsize, int ysize) {
	
	int option_height = ((4*ysize)/(5*UI_MENUSIZE)); // Using euclidian division to work around floaty results

	set_drawing_color(color_BLACK);
	// Bouton
	draw_rectangle_full(pos.x - xsize/2 + UI_BOX_MARGIN, pos.y - entry*option_height + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2 - UI_BOX_MARGIN, pos.y - (entry+1)*option_height + 3*(ysize/UI_MENUSIZE));

	// Texte
	set_drawing_color(color_WHITE);
	draw_string(pos.x - xsize/2 + 2*UI_BOX_MARGIN, pos.y - (entry+1)*option_height + 3*(ysize/UI_MENUSIZE) + UI_BOX_MARGIN, entry_list[entry]); // Flemme de centrer / ici i+1 car i est la limite supérieure du rectangle dans l'instruction précédente
}

// Retour à l'état normal après survol
void ui_menudehover(UI_MENUENTRY entry_list[UI_MENUSIZE], int entry, COORDS pos, int xsize, int ysize) {

	int option_height = ((4*ysize)/(5*UI_MENUSIZE)); // Using euclidian division to work around floaty results

	// Dessin contour bouton
	set_drawing_color(color_WHITE);
	draw_rectangle_full(pos.x - xsize/2 + UI_BOX_MARGIN, pos.y - entry*option_height + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2 - UI_BOX_MARGIN, pos.y - (entry+1)*option_height + 3*(ysize/UI_MENUSIZE));
	set_drawing_color(color_BLACK);
	draw_rectangle(pos.x - xsize/2 + UI_BOX_MARGIN, pos.y - entry*option_height + 3*(ysize/UI_MENUSIZE), pos.x + xsize/2 - UI_BOX_MARGIN, pos.y - (entry+1)*option_height + 3*(ysize/UI_MENUSIZE));

	// Contenu texte bouton
	set_drawing_color(color_BLACK);
	draw_string(pos.x - xsize/2 + 2*UI_BOX_MARGIN, pos.y - (entry+1)*option_height + 3*(ysize/UI_MENUSIZE) + UI_BOX_MARGIN, entry_list[entry]); // Flemme de centrer / ici i+1 car i est la limite supérieure du rectangle dans l'instruction précédente
}

// Définition des différents menus possibles (ici, menu en jeu) / retourne le menu en tant que struct
UI_MENU ui_setmenu_ig() {

	UI_MENU menu;

	strcpy(menu.entry_list[0], "Retour au jeu");
	strcpy(menu.entry_list[1], "Recommencer");
	strcpy(menu.entry_list[2], "Retour au menu");
	strcpy(menu.entry_list[3], "Quitter le jeu");
	strcpy(menu.entry_list[4], "");
	strcpy(menu.entry_list[5], "");
	strcpy(menu.entry_list[6], "");
	strcpy(menu.entry_list[7], "");
	strcpy(menu.entry_list[8], "");
	strcpy(menu.entry_list[9], "");

	int buffer[UI_MENUSIZE]	= {0,0,0,0,1,1,1,1,1,1};
	memcpy(menu.grey, buffer, sizeof(buffer));
	
	return menu;
}

// Définition des différents menus possibles (ici, menu principal) / retourne le menu en tant que struct
UI_MENU ui_setmenu_main() {

	UI_MENU menu;

	strcpy(menu.entry_list[0], "Nouvelle partie IA1");
	strcpy(menu.entry_list[1], "Nouvelle partie IA2");
	strcpy(menu.entry_list[2], "Nouvelle partie HvH");
	strcpy(menu.entry_list[3], "Quitter le jeu");
	strcpy(menu.entry_list[4], "");
	strcpy(menu.entry_list[5], "");
	strcpy(menu.entry_list[6], "");
	strcpy(menu.entry_list[7], "");
	strcpy(menu.entry_list[8], "");
	strcpy(menu.entry_list[9], "");

	int buffer[UI_MENUSIZE]	= {0,0,0,0,1,1,1,1,1,1};
	memcpy(menu.grey, buffer, sizeof(buffer));

	return menu;
}
// Définition des différents menus possibles (ici, menu de fin de jeu) / retourne le menu en tant que struct
UI_MENU ui_setmenu_endgame() {

	UI_MENU menu;

	strcpy(menu.entry_list[0], "Recommencer");
	strcpy(menu.entry_list[1], "Retour au menu");
	strcpy(menu.entry_list[2], "Quitter le jeu");
	strcpy(menu.entry_list[3], "");
	strcpy(menu.entry_list[4], "");
	strcpy(menu.entry_list[5], "");
	strcpy(menu.entry_list[6], "");
	strcpy(menu.entry_list[7], "");
	strcpy(menu.entry_list[8], "");
	strcpy(menu.entry_list[9], "");

	int buffer[UI_MENUSIZE]	= {0,0,0,1,1,1,1,1,1,1};
	memcpy(menu.grey, buffer, sizeof(buffer));
	
	return menu;
}

/*********
 * BOARD *
 * *******/
// Dessin des flèches désignant la colonne choisie pour jouer (et nettoyage préalable de l'espace flèches)
void ui_drawarrow(int col, int color) { // 1 = white, 2 = black, 3 = red
	// Nettoyage marge haute
	set_drawing_color(color_WHITE);
	draw_rectangle_full(0, ((BOARD_HEIGHT*WINDOW_HEIGHT)/(BOARD_HEIGHT+1)), WINDOW_WIDTH, WINDOW_HEIGHT);
	// Triangle
	int baselength = min(WINDOW_WIDTH/(2*BOARD_WIDTH), WINDOW_HEIGHT/(2*(BOARD_HEIGHT+1)));
	COORDS origin;
	origin.x = WINDOW_WIDTH/(2*BOARD_WIDTH) + col*(WINDOW_WIDTH/BOARD_WIDTH);
	origin.y = WINDOW_HEIGHT - 1;

	set_drawing_color(color_BLACK);

	draw_line(origin.x, origin.y, origin.x - baselength, origin.y);
	draw_line(origin.x, origin.y, origin.x + baselength, origin.y);

	draw_line(origin.x - baselength, origin.y, origin.x, origin.y - baselength);
	draw_line(origin.x + baselength, origin.y, origin.x, origin.y - baselength);

	set_fill_color(color_BLACK);
	if(color == 2) fill_surface(origin.x, origin.y - 3, color_BLACK);
	if(color == 3) {
		set_fill_color(color_RED);
		fill_surface(origin.x, origin.y - 3, color_BLACK);
	}
	update_graphics();

}
// Animation du mouvement du curseur, dans le cas où une IA joue
void ui_animcol(int color, int lastcol, int col) {
	int i, sign;

	if(lastcol < col) sign = 1;
	else if(lastcol > col) sign = -1;
	else sign = col - lastcol;

	for(i = lastcol ; i != col ; i += sign) {
		ui_drawarrow(i, color);
		usleep(300000);
	}

}


