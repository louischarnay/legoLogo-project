#include<SDL.h> //ne pas oublier
#include<SDL_ttf.h> //ne pas oublier
#include<iostream>
#include<fstream>
#include<ctime>
#include "config_sdl.h"
#include<math.h>
#include"declarations.h";

using namespace std;


int main(int argn, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
		return 1;
	}
	if (TTF_Init() != 0) {
		cout << "Echec de texte";
		return 1;
	}

	//fenetre
	SDL_Window* win = SDL_CreateWindow("LEGOLOGO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		FEN_LARGEUR,
		FEN_HAUTEUR,
		SDL_WINDOW_SHOWN
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	SDL_Surface* loadedImage = NULL;
	loadedImage = IMG_Load("Images/logo.png");
	SDL_SetSurfaceAlphaMod(loadedImage, 255);
	SDL_SetWindowIcon(win, loadedImage);


	//rendu
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED);


	//Définition de la regle et du rapporteur
	SDL_Texture* rapporteur = loadImage(rendu, "Images/Rapporteur_N.png");
	SDL_Texture* regle = loadImage(rendu, "Images/regle20cm.png");

	//Définition des couleurs
	SDL_Color bleu = { 0,0,255,255 };

	//Définition des boutons
	//on définit les rectangle à tracer
	//				{ X , Y , W , H };
	SDL_Rect bouton1{ 663,30,100,50 };					// BOUTON 1
	bool bouton1_clique = false;

	SDL_Rect bouton2{ 663,100,100,50 };					// BOUTON 2
	bool bouton2_clique = false;

	SDL_Rect bouton3{ 663,170,100,50 };					// BOUTON 3
	bool bouton3_clique = false;

	SDL_Rect bouton4{ 663,240,100,50 };					// BOUTON 4
	bool bouton4_clique = false;

	SDL_Rect bouton5{ 663,400,100,50 };					// BOUTON QUITTER/RETOUR

	//initialisation(rendu);

	SDL_Rect boutonProf{ 250,200,300,50 };
	SDL_Rect boutonELEVE{ 250,270,300,50 };
	SDL_Rect boutonQuit{ 300,400,200,40 };

	SDL_Rect Background{ -130, -1706, 960, 2559 };
	SDL_Rect legoBleu{ 50, 170, 157, 143 };
	SDL_Rect legoRouge{ 600, 170, 180, 132 };
	bool sens = true;

	int nbform = 0;
	Form* tabForm = new Form[0];
	int nbpoints = 0;
	Coord* points;
	points = new Coord[nbpoints];

	int state = 0;
	bool onetime = true;
	bool obst_activated = false;

	bool tracer = false;
	bool correction = false;
	bool tracerobs = false;
	bool tracerrap = false;
	bool tracereg = false;
	bool tracerreg = false;
	int x1, y1;
	int corpoint;

	char instr[3];
	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		while (SDL_PollEvent(&event)) {//attente d?un évènement
			switch (event.type) //test du type d?évènement
			{
			case SDL_QUIT: //clic sur la croix de fermeture				??? pas opti
				continuer = false;
				break;
			}

			switch (state)
			{
			case 0: //Menu principale
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP: //clic sur la croix de fermeture

					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if (event.button.x > boutonProf.x && event.button.x<boutonProf.x + boutonProf.w && event.button.y>boutonProf.y && event.button.y < boutonProf.y + boutonProf.h) {
							state = 1;
							obst_activated = true;
							initialisation(rendu);
							onetime = true;
						}
						if (event.button.x > boutonELEVE.x && event.button.x<boutonELEVE.x + boutonELEVE.w && event.button.y>boutonELEVE.y && event.button.y < boutonELEVE.y + boutonELEVE.h) {
							state = 1;
							obst_activated = false;
							initialisation(rendu);
							onetime = true;
						}
						if (event.button.x > boutonQuit.x && event.button.x<boutonQuit.x + boutonQuit.w && event.button.y>boutonQuit.y && event.button.y < boutonQuit.y + boutonQuit.h) {
							continuer = false;
						}
					}

				}
				break;
			case 1: //Menu #1
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP: //clic sur la croix de fermeture

					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if ((event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) && (obst_activated == true)) {
							state = 2;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							state = 3;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							state = 4;
							onetime = true;
						}
						if (event.button.x > bouton4.x && event.button.x<bouton4.x + bouton4.w && event.button.y>bouton4.y && event.button.y < bouton4.y + bouton4.h) {
							state = 5;
							cout << "Appuyez sur S pour effacer toutes les instructions.\nAppuyez sur ENTREE pour dessiner les instructions.\n";
							affiche_instructions("Instructions/instructions.txt");
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							state = 0;
							delete[]points;
							points = new Coord[nbpoints];
						}
					}
				}
				break;
			case 2: //Menu Obstacles
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP: //clic sur la croix de fermeture
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							state = 6;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = true;
							state = 7;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton3_clique = true;
							state = 8;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							state = 1;
							onetime = true;
						}
					}
				}
				break;
			case 3: //Menu Ligne
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							state = 9;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = true;
							state = 10;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton3_clique = false;
							state = 11;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							state = 1;
							onetime = true;
						}
					}
				}
				break;
			case 4: //Menu Mesures
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							state = 12;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = true;
							state = 13;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							state = 1;
							onetime = true;
						}
					}
				}
				break;
			case 5: //Menu #5
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
							strcpy_s(instr, "AV");
							ecritureFichier(instr, mainInstruction(argn, argv));
							bouton1_clique = false;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = true;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
							strcpy_s(instr, "RE");
							ecritureFichier(instr, mainInstruction(argn, argv));
							bouton2_clique = false;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton3_clique = true;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
							strcpy_s(instr, "D ");
							ecritureFichier(instr, mainInstruction(argn, argv));
							bouton3_clique = false;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
						}
						if (event.button.x > bouton4.x && event.button.x<bouton4.x + bouton4.w && event.button.y>bouton4.y && event.button.y < bouton4.y + bouton4.h) {
							bouton4_clique = true;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
							strcpy_s(instr, "G ");
							ecritureFichier(instr, mainInstruction(argn, argv));
							bouton4_clique = false;
							init_menubutton(rendu);
							affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							state = 1;
							onetime = true;
						}
					}
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_s) {
						ofstream sortie("Instructions/instructions.txt", ios::out);
						if (!sortie) cout << "Erreur d'ouverture du fichier";
						else {
							sortie.clear();
							for (int i = 0; i < 30; i++) cout << endl;
							cout << "Le fichier a bien ete clear\n";
						}
						
					}
					else if (event.key.keysym.sym == SDLK_RETURN) {
						points=lecture_deplacement("Instructions/instructions.txt", points, nbpoints);
						affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
					}
					break;
				}
				break;
			case 6: //Rectangle
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							if (tracerobs) {
								if (x1 + abs(x1 - x) < LARGEUR) { //obligé d'emboiter les if pour être sur que x1 et y1 soient initialisés.
									tabForm = obstacle_add(rendu, x1, y1, x, y, tabForm, nbform, CARRE);
									tracer_obstacle(rendu, tabForm[nbform - 1]);
									SDL_RenderPresent(rendu);
									tracerobs = false;
								}	
							}
							else {
								x1 = x;
								y1 = y;
								tracerobs = true;
							}
						}
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = false;
							state = 2;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton1_clique = false;
							bouton2_clique = true;
							state = 7;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton1_clique = false;
							bouton3_clique = true;
							state = 8;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							bouton1_clique = false;
							state = 1;
							onetime = true;
						}
					}
				case SDL_MOUSEMOTION:
					int x = event.button.x;
					int y = event.button.y;
					if (x < LARGEUR && y < HAUTEUR) {
						if (tracerobs) {
							if (x1 + abs(x1 - x) < LARGEUR) { 
								affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
								previsualisation_forme(rendu, win, x1, y1, x, y, CARRE);
								SDL_RenderPresent(rendu);
							}
						}
					}
				}
				break;

			case 7: //Cercle
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							if (tracerobs) {
								if (x1 + sqrt(pow(x1 - x, 2) + pow(y1 - y,2)) < LARGEUR) { // de même, emboitage de if obligatoire
									tabForm = obstacle_add(rendu, x1, y1, x, y, tabForm, nbform, CERCLE);
									tracer_obstacle(rendu, tabForm[nbform - 1]);
									SDL_RenderPresent(rendu);
									tracerobs = false;
								}
							}
							else {
								x1 = x;
								y1 = y;
								tracerobs = true;
							}
						}
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							bouton2_clique = false;
							state = 6;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = false;
							state = 2;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton2_clique = false;
							bouton3_clique = true;
							state = 8;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							bouton2_clique = false;
							state = 1;
							onetime = true;
						}
					}
				case SDL_MOUSEMOTION:
					int x = event.button.x;
					int y = event.button.y;
					if (x < LARGEUR && y < HAUTEUR) {
						if (tracerobs) {
							if (x1 + sqrt(pow(x1 - x, 2) + pow(y1 - y, 2)) < LARGEUR) {
								affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
								previsualisation_forme(rendu, win, x1, y1, x, y, CERCLE);
								SDL_RenderPresent(rendu);
							}
						}
					}
				}
				break;

			case 8: //Effacer
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						int x = event.button.x;
						int y = event.button.y;
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							bouton3_clique = false;
							state = 6;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = true;
							bouton3_clique = false;
							state = 7;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton3_clique = false;
							state = 2;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							bouton3_clique = false;
							state = 1;
							onetime = true;
						}
						for (int i = 0; i < nbform; i++) {
							int distancex = abs(tabForm[i].centre.x - tabForm[i].radius.x);
							int distancey = abs(tabForm[i].centre.y - tabForm[i].radius.y);
							if (tabForm[i].s == CARRE) {
								if (x >= tabForm[i].centre.x - distancex && x <= tabForm[i].centre.x + distancex && y >= tabForm[i].centre.y - distancey && y <= tabForm[i].centre.y + distancey) {
									tabForm = effacer_obstacle(tabForm, i, nbform);
									affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
								}
							}
							else {
								if (sqrt(pow(x - tabForm[i].centre.x, 2) + pow(y - tabForm[i].centre.y, 2)) <= sqrt(pow(tabForm[i].radius.x - tabForm[i].centre.x, 2) + pow(tabForm[i].radius.y - tabForm[i].centre.y, 2))) {
									tabForm = effacer_obstacle(tabForm, i, nbform);
									affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
								}
							}
						}
					}
				}
				break;
			case 9: //Points
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							if (tracer) {
								tracer_trait(rendu, points[nbpoints - 1].x, points[nbpoints - 1].y, x, y);
								tracer_contour(rendu, x, y);
								points = allouerplus(points, nbpoints);
								points[nbpoints - 1].x = x;
								points[nbpoints - 1].y = y;
							}
							else if (tracer == false && nbpoints == 0 && correction == false) {
								points = allouerplus(points, nbpoints);
								points[0].x = x;
								points[0].y = y;
								tracer_contour(rendu, x, y);
								tracer = true;
							}
							else if (correction == false) {
								for (int i = 0; i < nbpoints; i++) {
									if (x > points[i].x - CONTOUR_POINT && x < points[i].x + CONTOUR_POINT && y > points[i].y - CONTOUR_POINT && y < points[i].y + CONTOUR_POINT) {
										correction = true;
										corpoint = i;
									}
								}
								tracer = false;
							}
							else if (correction == true) correction = false;
						}
						else tracer = false;
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = false;
							state = 3;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton1_clique = false;
							bouton2_clique = true;
							state = 10;
							onetime = true;
						}
						if (event.button.x > bouton3.x && event.button.x<bouton3.x + bouton3.w && event.button.y>bouton3.y && event.button.y < bouton3.y + bouton3.h) {
							bouton1_clique = false;
							bouton3_clique = false;
							state = 11;
							onetime = true;
						}
						if (event.button.x > bouton4.x && event.button.x<bouton4.x + bouton4.w && event.button.y>bouton4.y && event.button.y < bouton4.y + bouton4.h) {
							bouton1_clique = false;
							state = 3;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							bouton1_clique = false;
							state = 1;
							onetime = true;
						}
					}; break;
				case SDL_MOUSEMOTION:
					if (tracer) {
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
							previsualisation_trait(rendu, win, x, y, points, nbpoints);
						}
						else {
							tracer = false;
							affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
						}
					}
					else if (correction) {
						int x = event.button.x;
						int y = event.button.y;
						points[corpoint].x = x;
						points[corpoint].y = y;
						affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
					}; break;
				}
				break;

			case 10: //Vérification
				break;

			case 11: //Effacer
				break;

			case 12: //Regle
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							if (tracereg) {
								affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
								tracer_regle(rendu, regle, x1, y1, x, y);
								init_menubutton(rendu);
								affichageMenuMesure(rendu, bouton1, bouton2, bouton5, bouton1_clique, bouton2_clique);
								tracereg = false;
							}
							else {
								x1 = x;
								y1 = y;
								tracereg = true;
							}
						}
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = false;
							state = 4;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton1_clique = false;
							bouton2_clique = true;
							state = 13;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							bouton1_clique = false;
							state = 1;
							onetime = true;
						}
					}
					break;
				case SDL_MOUSEMOTION:
					if (tracereg) {
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
							tracer_regle(rendu, regle, x1, y1, x, y);
							init_menubutton(rendu);
							affichageMenuMesure(rendu, bouton1, bouton2, bouton5, bouton1_clique, bouton2_clique);
						}
					}
				}
				break;
			case 13: //Rapporteur
				switch (event.type) //test du type d?évènement
				{
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							if (tracerrap) {
								affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
								tracer_rapporteur(rendu, rapporteur, x1, y1, x, y);
								init_menubutton(rendu);
								affichageMenuMesure(rendu, bouton1, bouton2, bouton5, bouton1_clique, bouton2_clique);
								tracerrap = false;
							}
							else {
								x1 = x;
								y1 = y;
								tracerrap = true;
							}
						}
						if (event.button.x > bouton1.x && event.button.x<bouton1.x + bouton1.w && event.button.y>bouton1.y && event.button.y < bouton1.y + bouton1.h) {
							bouton1_clique = true;
							bouton2_clique = false;
							state = 12;
							onetime = true;
						}
						if (event.button.x > bouton2.x && event.button.x<bouton2.x + bouton2.w && event.button.y>bouton2.y && event.button.y < bouton2.y + bouton2.h) {
							bouton2_clique = false;
							state = 4;
							onetime = true;
						}
						if (event.button.x > bouton5.x && event.button.x<bouton5.x + bouton5.w && event.button.y>bouton5.y && event.button.y < bouton5.y + bouton5.h) {
							bouton2_clique = false;
							state = 1;
							onetime = true;
						}
					}
					break;
				case SDL_MOUSEMOTION:
					if (tracerrap) {
						int x = event.button.x;
						int y = event.button.y;
						if (x < LARGEUR && y < HAUTEUR) {
							affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
							tracer_rapporteur(rendu, rapporteur, x1, y1, x, y);
							init_menubutton(rendu);
							affichageMenuMesure(rendu, bouton1, bouton2, bouton5, bouton1_clique, bouton2_clique);
						}
					}
				}
				break;
			}
			break;
		}
		if (state == 0) {
			menu_principal(rendu, Background, legoBleu, legoRouge, boutonProf, boutonELEVE, boutonQuit, sens);
		}
		else {

			switch (state) {
			case 1:
				if (onetime == true) {
					affichageMenu(rendu, bouton1, bouton2, bouton3, bouton4, bouton5);
					affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
					if (obst_activated == false) {
						croixRouge(rendu, bouton1);
					}
					onetime = false;
				}
				break;
			case 2:
			case 6:
			case 7:
			case 8:
				if (onetime == true) {
					init_menubutton(rendu);
					affichageMenuObstacles(rendu, bouton1, bouton2, bouton3, bouton5, bouton1_clique, bouton2_clique, bouton3_clique);
					onetime = false;
				}
				break;
			case 3:
			case 9:
			case 10:
			case 11:
				if (onetime == true) {
					init_menubutton(rendu);
					affichageMenuLignes(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique);
					onetime = false;
				}
				break;
			case 4:
			case 12:
			case 13:
				if (onetime == true) {
					init_menubutton(rendu);
					affichageMenuMesure(rendu, bouton1, bouton2, bouton5, bouton1_clique, bouton2_clique);
					onetime = false;
				}
				break;
			case 5:
				if (onetime == true) {
					init_menubutton(rendu);
					affichageMenuInstructions(rendu, bouton1, bouton2, bouton3, bouton4, bouton5, bouton1_clique, bouton2_clique, bouton3_clique, bouton4_clique);
					onetime = false;
				}
			}

			if (state == 10) {
				verification(rendu, points, nbpoints, tabForm, nbform);
				state = 3;
				bouton2_clique = false;
				onetime = true;
			}
			if (state == 11) {
				points = effacer_points(points, nbpoints);
				affiche_tout(rendu, win, points, nbpoints, tabForm, nbform);
				tracer = false;
				correction = false;
				state = 3;
			}
		}
	}

	delete[] tabForm;
	delete[] points;

	SDL_DestroyTexture(regle);
	SDL_DestroyTexture(rapporteur);

	TTF_Quit();
	SDL_FreeSurface(loadedImage);
	SDL_DestroyRenderer(rendu);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

/*#######################################################################################
FONCTIONS 
#######################################################################################*/

void previsualisation_trait(SDL_Renderer* rendu, SDL_Window* win, int x, int y, Coord* points, int n) {
	SDL_SetRenderDrawColor(rendu, 255, 165, 0, 140);
	SDL_RenderDrawLine(rendu, points[n - 1].x, points[n - 1].y, x, y);
	SDL_RenderPresent(rendu);
}

void affiche_tout(SDL_Renderer* rendu, SDL_Window* win, Coord* points, int npoints, Form* tabform, int nformes) {
	SDL_Rect fenetreB{ 0,0,LARGEUR,HAUTEUR };
	SDL_Texture* image_fond = loadImage(rendu, "Images/background.png");
	SDL_RenderCopy(rendu, image_fond, NULL, &fenetreB);

	affiche_obstacle(rendu, tabform, nformes);
	affiche_trait(rendu, points, npoints);

	//SDL_RenderPresent(rendu);
	SDL_DestroyTexture(image_fond);
}

void tracer_trait(SDL_Renderer* rendu, int x1, int y1, int x2, int y2) {
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, x1, y1, x2, y2);
	SDL_RenderPresent(rendu);
}

void tracer_contour(SDL_Renderer* rendu, int x, int y) {
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_Rect rect;
	rect.x = x - CONTOUR_POINT;
	rect.y = y - CONTOUR_POINT;
	rect.w = CONTOUR_POINT * 2;
	rect.h = CONTOUR_POINT * 2;
	SDL_RenderDrawRect(rendu, &rect);
	SDL_RenderPresent(rendu);
}

void affiche_trait(SDL_Renderer* rendu, Coord* points, int n) {
	tracer_contour(rendu, points[0].x, points[0].y);
	for (int i = 0; i < n - 1; i++) {
		tracer_contour(rendu, points[i + 1].x, points[i + 1].y);
		tracer_trait(rendu, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
	}
}

Coord* allouerplus(Coord* coord, int& n) {
	n++;
	Coord* newcoord = new Coord[n];
	for (int i = 0; i < n - 1; i++) {
		newcoord[i].x = coord[i].x;
		newcoord[i].y = coord[i].y;
	}
	delete[] coord;
	return newcoord;
}

void fill_circle(SDL_Renderer* rendu, int cx, int cy, int radius)
{
	for (double dy = 1; dy <= radius; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_RenderDrawLine(rendu, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(rendu, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
	}
}

Form* AjoutForm(Form* tab, Form f, int& n) {
	n++;
	Form* tab2 = new Form[n];
	for (int i = 0; i < n - 1; i++)
		tab2[i] = tab[i];
	delete[]tab;
	tab2[n - 1] = f;
	return tab2;
}

Form* obstacle_add(SDL_Renderer* rendu, int x1, int y1, int x2, int y2, Form* tabForm, int& nbform, shape s) {
	Form f = {
		s,
		x1,
		y1,
		{x2,y2}
	};
	return AjoutForm(tabForm, f, nbform);
}

void tracer_obstacle(SDL_Renderer* rendu, Form f) {
	SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
	if (f.s == CARRE) {
		SDL_Rect rect;
		rect.x = f.centre.x - abs(f.centre.x - f.radius.x);
		rect.y = f.centre.y - abs(f.centre.y - f.radius.y);
		rect.w = abs(rect.x - f.centre.x) * 2;
		rect.h = abs(rect.y - f.centre.y) * 2;
		SDL_RenderFillRect(rendu, &rect);
		SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
		SDL_RenderDrawRect(rendu, &rect);
	}
	else if (f.s == CERCLE) {
		fill_circle(rendu, f.centre.x, f.centre.y, sqrt(pow(f.centre.x - f.radius.x, 2) + pow(f.centre.y - f.radius.y, 2)));
		SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
		DrawCircle(rendu, f.centre.x, f.centre.y, sqrt(pow(f.centre.x - f.radius.x, 2) + pow(f.centre.y - f.radius.y, 2)));
	}
	//SDL_RenderPresent(rendu);
}

void affiche_obstacle(SDL_Renderer* rendu, Form* tabform, int nforme) {
	for (int i = 0; i < nforme; i++) tracer_obstacle(rendu, tabform[i]);
	//SDL_RenderPresent(rendu);
}

void previsualisation_forme(SDL_Renderer* rendu, SDL_Window* win, int x1, int y1, int x2, int y2, shape s) {
	Coord rad = { x2,y2 };
	Form tmp = { s,x1, y1,rad };
	tracer_obstacle(rendu, tmp);
}

void verification(SDL_Renderer* rendu, Coord* points, int npoints, Form* obstacles, int nobstacles) {
	Form Robot = { CERCLE, {points[0].x,points[0].y}, {LARGEUR_ROBOT / 2,LARGEUR_ROBOT / 2} };
	double a, b, dist;
	SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
	for (int i = 1; i < npoints; i++) {
		a = double(points[i].y - points[i - 1].y) / (points[i].x - points[i - 1].x);
		b = points[i - 1].y - a * points[i - 1].x;
		if (abs(points[i - 1].x - points[i].x) >= abs(points[i - 1].y - points[i].y)) {
			if (points[i - 1].x < points[i].x) {
				for (int j = points[i - 1].x; j < points[i].x; j++) {
					check_collision(rendu, j, a * j + b, obstacles, nobstacles);
					fill_circle(rendu, j, a * j + b, LARGEUR_ROBOT / 2);
					SDL_RenderPresent(rendu);
					SDL_Delay(10);
				}
			}
			else {
				for (int j = points[i - 1].x; j > points[i].x; j--) {
					check_collision(rendu, j, a * j + b, obstacles, nobstacles);
					fill_circle(rendu, j, a * j + b, LARGEUR_ROBOT / 2);
					SDL_RenderPresent(rendu);
					SDL_Delay(10);
				}
			}
		}
		else {
			if (points[i - 1].y < points[i].y) {
				for (int j = points[i - 1].y; j < points[i].y; j++) {
					check_collision(rendu, (j - b) / a, j, obstacles, nobstacles);
					fill_circle(rendu, (j - b) / a, j, LARGEUR_ROBOT / 2);
					SDL_RenderPresent(rendu);
					SDL_Delay(10);
				}
			}
			else {
				for (int j = points[i - 1].y; j > points[i].y; j--) {
					check_collision(rendu, (j - b) / a, j, obstacles, nobstacles);
					fill_circle(rendu, (j - b) / a, j, LARGEUR_ROBOT / 2);
					SDL_RenderPresent(rendu);
					SDL_Delay(10);
				}
			}
		}

	}
}

void check_collision(SDL_Renderer* rendu, int x1, int y1 /*coord du robot*/, Form* obstacles, int nbobstacles) {
	double distance, distancerect = 0;
	int l, L;
	for (int i = 0; i < nbobstacles; i++) {
		distance = sqrt(pow(x1 - obstacles[i].centre.x, 2) + pow(y1 - obstacles[i].centre.y, 2));
		if (obstacles[i].s == CARRE) {
			double alpha = asin(abs(x1 - obstacles[i].centre.x) / distance);
			l = abs(obstacles[i].centre.x - obstacles[i].radius.x); //demi-hauteur
			L = abs(obstacles[i].centre.y - obstacles[i].radius.y); //demi-largeur
			if (L > l) { //différence entre rectangle vertical et horizontal
				if (obstacles[i].centre.y + abs(obstacles[i].centre.y - obstacles[i].radius.y)/*calcul de la hauteur du plus bas sommet*/ >= y1 && obstacles[i].centre.y - abs(obstacles[i].centre.y - obstacles[i].radius.y) <= y1) {
					distancerect = abs(l / sin(alpha));
				}
				else {
					distancerect = abs(l / cos(alpha));
				}

			}
			else {
				if (obstacles[i].centre.x + abs(obstacles[i].centre.x - obstacles[i].radius.x)/*calcul de la hauteur du plus bas sommet*/ >= x1 && obstacles[i].centre.x - abs(obstacles[i].centre.x - obstacles[i].radius.x) <= x1) {
					distancerect = abs(L / cos(alpha));
				}
				else {
					distancerect = abs(L / sin(alpha));
				}
			}

			if (distance - distancerect - double(LARGEUR_ROBOT) / 2 <= 0) {
				SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
				return;
			}
		}
		else if (obstacles[i].s == CERCLE) {
			double rcercle = sqrt(pow(obstacles[i].centre.x - obstacles[i].radius.x, 2) + pow(obstacles[i].centre.y - obstacles[i].radius.y, 2));
			if (distance - LARGEUR_ROBOT / 2 - rcercle <= 0) {
				SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
				return;
			}
		}
	}
	SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
}

void init_boutons(SDL_Renderer* rendu) {
	SDL_Rect rect;
	rect.x = LARGEUR;
	rect.y = 0;
	rect.w = FEN_LARGEUR - LARGEUR;
	rect.h = HAUTEUR;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect);
	SDL_RenderPresent(rendu);
}

Coord* effacer_points(Coord* points, int& n) {
	delete[] points;
	n = 0;
	points = new Coord[n];
	return points;
}

Coord* lecture_deplacement(const char nom_fichier[], Coord* points, int& nbpoints) {
	ifstream entree(nom_fichier, ios::in);
	if (!entree) cout << "Enregistrement non effectué !";
	char ligne[6];
	float valeur;
	float pixels, angle = 0;
	int x, y;
	if (nbpoints == 0) {
		points = allouerplus(points, nbpoints);
		points[0].x = 0;
		points[0].y = HAUTEUR;
	}
	while (!entree.eof()) {
		entree >> ligne;
		if (strcmp(ligne, "AV") == 0) {
			entree >> valeur;
			pixels = valeur * ECHELLE;
			x = points[nbpoints - 1].x + cos(angle) * pixels;
			y = points[nbpoints - 1].y - sin(angle) * pixels;
			points = allouerplus(points, nbpoints);
			points[nbpoints - 1].x = x;
			points[nbpoints - 1].y = y;
		}
		else if (strcmp(ligne, "RE") == 0) {
			entree >> valeur;
			pixels = valeur * ECHELLE;
			x = points[nbpoints - 1].x - cos(angle) * pixels;
			y = points[nbpoints - 1].y + sin(angle) * pixels;
			points = allouerplus(points, nbpoints);
			points[nbpoints - 1].x = x;
			points[nbpoints - 1].y = y;
		}
		else if (strcmp(ligne, "G") == 0) {
			entree >> valeur;
			angle = angle + convertir_degre(valeur);
		}
		else if (strcmp(ligne, "D") == 0) {
			entree >> valeur;
			angle = angle - convertir_degre(valeur);
		}
	}
	entree.close();
	return points;
}

double convertir_degre(double degre)
{
	return (degre * (pi / 180));
}

double convertir_rad(double rad, int x1, int y1, int x2, int y2) {
	rad = rad * (180 / pi);
	if (x1 > x2 && y1 < y2) return rad + 180;
	else if (x1 < x2 && y1 < y2) return -(rad + 180);
	else if (x1 > x2 && y1 > y2) return -(rad);
	else return (rad);
}

void tracer_rapporteur(SDL_Renderer* rendu, SDL_Texture* rapporteur, int xcentre, int ycentre, int x, int y) {
	double rayonrap = sqrt(pow(xcentre - x, 2) + pow(ycentre - y, 2));
	double angle;
	if (xcentre == x && y - ycentre == rayonrap) angle = 180;
	else if (xcentre == x && y - ycentre == -rayonrap) angle = 0;
	else if (xcentre - x == rayonrap && y - ycentre == 0) angle = 270;
	else if (xcentre - x == -rayonrap && y - ycentre == 0) angle = 90;
	else angle = convertir_rad(atan(double(abs(xcentre - x)) / abs(ycentre - y)), xcentre, ycentre, x, y);
	SDL_Rect rect;
	rect.x = xcentre - rayonrap;
	rect.y = ycentre - rayonrap;
	rect.w = rayonrap * 2;//abs(ycentre - y) * 2;
	rect.h = rayonrap * 2;//abs(ycentre - y) * 2;
	SDL_RenderCopyEx(rendu, rapporteur, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(rendu);
}

void tracer_regle(SDL_Renderer* rendu, SDL_Texture* regle, int xcoin, int ycoin, int x, int y) {
	double l = sqrt(pow(xcoin - x, 2) + pow(ycoin - y, 2));
	double angle;
	if (xcoin == x && y - ycoin == l) angle = 180;
	else if (xcoin == x && y - ycoin == -l) angle = 0;
	else if (xcoin - x == l && y - ycoin == 0) angle = 270;
	else if (xcoin - x == -l && y - ycoin == 0) angle = 90;
	else angle = convertir_rad(atan(double(abs(xcoin - x)) / abs(ycoin - y)), xcoin, ycoin, x, y);
	SDL_Rect r;
	r.h = HAUTEUR_REGLE;
	r.w = r.h / 7;
	r.x = xcoin;
	r.y = ycoin - r.h;
	SDL_Rect rognage = r;
	SDL_Point point;
	point.x = 0;
	point.y = r.h;
	SDL_RenderCopyEx(rendu, regle, NULL, &r, angle, &point, SDL_FLIP_NONE);
	SDL_RenderPresent(rendu);
}

Form* effacer_obstacle(Form* obstacles, int n, int& nobstacles) {
	Form* newobstacle = new Form[nobstacles - 1];
	int j = 0;
	for (int i = 0; i < nobstacles; i++) {
		if (i != n) {
			newobstacle[j] = obstacles[i];
			j++;
		}
	}
	nobstacles--;
	delete[] obstacles;
	return newobstacle;
}

void affiche_instructions(const char nom_fichier[]) {
	ifstream entree(nom_fichier, ios::in);
	if (!entree) return;
	char ligne[15];
	while (!entree.eof()) {
		entree >> ligne;
		cout << ligne << " ";
		entree >> ligne;
		cout << ligne << endl;
	}
	entree.close();
}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void init_menubutton(SDL_Renderer* rendu) {
	SDL_Rect fenetreN{ LARGEUR,0,160,HAUTEUR };
	SDL_Texture* image_menu = loadImage(rendu, "Images/background_menu.png");
	SDL_RenderCopy(rendu, image_menu, NULL, &fenetreN);
	SDL_DestroyTexture(image_menu);
}

void initialisation(SDL_Renderer* rendu) {
	SDL_Rect fenetreB{ 0,0,LARGEUR,HAUTEUR };
	SDL_Texture* image_fond = loadImage(rendu, "Images/background.png");
	SDL_RenderCopy(rendu, image_fond, NULL, &fenetreB);
	init_menubutton(rendu);
	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(image_fond);

}

void affichageCaseImage(SDL_Renderer* rendu, SDL_Rect bouton_nom, SDL_Rect texte_nom, SDL_Texture* image_nom, SDL_Texture* texture_nom) {
	SDL_QueryTexture(image_nom, NULL, NULL, &bouton_nom.w, &bouton_nom.h);
	SDL_RenderCopy(rendu, image_nom, NULL, &bouton_nom);
	SDL_QueryTexture(texture_nom, NULL, NULL, &texte_nom.w, &texte_nom.h);
	SDL_RenderCopy(rendu, texture_nom, NULL, &texte_nom);
	SDL_DestroyTexture(texture_nom);
}

void affichageCaseVide(SDL_Renderer* rendu, SDL_Rect bouton_nom, SDL_Rect texte_nom, SDL_Texture* texture_nom, SDL_Color couleur) {
	SDL_SetRenderDrawColor(rendu, couleur.r, couleur.g, couleur.b, couleur.a);  //pinceau bleu
	SDL_RenderDrawRect(rendu, &bouton_nom); //on trace un rectangle plein
	SDL_QueryTexture(texture_nom, NULL, NULL, &texte_nom.w, &texte_nom.h);
	SDL_RenderCopy(rendu, texture_nom, NULL, &texte_nom);
	SDL_DestroyTexture(texture_nom);
}

void croixRouge(SDL_Renderer* rendu, SDL_Rect bouton_nom) {
	SDL_Texture* image_croix = loadImage(rendu, "Images/croix.png");
	SDL_QueryTexture(image_croix, NULL, NULL, &bouton_nom.w, &bouton_nom.h);
	SDL_RenderCopy(rendu, image_croix, NULL, &bouton_nom);
	SDL_DestroyTexture(image_croix);
	SDL_RenderPresent(rendu);
}

void affichageMenu(SDL_Renderer* rendu, SDL_Rect bouton_obstacles, SDL_Rect bouton_ligne, SDL_Rect bouton_mesure, SDL_Rect bouton_instruction, SDL_Rect bouton_quitter) {
	TTF_Font* font = TTF_OpenFont("Font/Pixeltype.ttf", 24);
	SDL_Color blanc = { 255,255,255 }; //on définit une couleur de texte
	SDL_Texture* image_bouton = loadImage(rendu, "Images/bouton.png");
	SDL_Texture* image_bouton_quitter = loadImage(rendu, "Images/bouton_quitter.png");

	SDL_Rect texte_obstacles; //rectangle définissant le positionnement du texte, et sa taille
	texte_obstacles.x = 680;
	texte_obstacles.y = 53;
	SDL_Texture* texture_obstacles = loadText(rendu, "OBSTACLES", blanc, font);
	affichageCaseImage(rendu, bouton_obstacles, texte_obstacles, image_bouton, texture_obstacles);

	SDL_Rect texte_ligne; //rectangle définissant le positionnement du texte, et sa taille
	texte_ligne.x = 693;
	texte_ligne.y = 123;
	SDL_Texture* texture_ligne = loadText(rendu, "LIGNES", blanc, font);
	affichageCaseImage(rendu, bouton_ligne, texte_ligne, image_bouton, texture_ligne);

	SDL_Rect texte_mesure; //rectangle définissant le positionnement du texte, et sa taille
	texte_mesure.x = 690;
	texte_mesure.y = 193;
	SDL_Texture* texture_mesure = loadText(rendu, "MESURES", blanc, font);
	affichageCaseImage(rendu, bouton_mesure, texte_mesure, image_bouton, texture_mesure);
	SDL_Rect texte_instruction; //rectangle définissant le positionnement du texte, et sa taille
	texte_instruction.x = 668;
	texte_instruction.y = 264;
	SDL_Texture* texture_instruction = loadText(rendu, "INSTRUCTIONS", blanc, font);
	affichageCaseImage(rendu, bouton_instruction, texte_instruction, image_bouton, texture_instruction);

	SDL_Rect texte_quitter; //rectangle définissant le positionnement du texte, et sa taille
	texte_quitter.x = 673;
	texte_quitter.y = 423;
	SDL_Texture* texture_quitter = loadText(rendu, "RETOUR MENU", blanc, font);
	affichageCaseImage(rendu, bouton_quitter, texte_quitter, image_bouton_quitter, texture_quitter);

	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(image_bouton);
	SDL_DestroyTexture(image_bouton_quitter);
	TTF_CloseFont(font);
}

void affichageMenuObstacles(SDL_Renderer* rendu, SDL_Rect bouton_rectangle, SDL_Rect bouton_cercle, SDL_Rect bouton_effacer1, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique, bool bouton3_clique) {
	TTF_Font* font = TTF_OpenFont("Font/Pixeltype.ttf", 24);
	SDL_Color blanc = { 255,255,255 }; //on définit une couleur de texte
	SDL_Texture* image_bouton = loadImage(rendu, "Images/bouton.png");
	SDL_Texture* image_bouton_clique = loadImage(rendu, "Images/bouton_clique.png");
	SDL_Texture* image_bouton_quitter = loadImage(rendu, "Images/bouton_quitter.png");

	SDL_Rect texte_rectangle; //rectangle définissant le positionnement du texte, et sa taille
	texte_rectangle.x = 680;
	texte_rectangle.y = 54;
	SDL_Texture* texture_rectangle = loadText(rendu, "RECTANGLE", blanc, font);
	if (bouton1_clique == false)
		affichageCaseImage(rendu, bouton_rectangle, texte_rectangle, image_bouton, texture_rectangle);
	else
		affichageCaseImage(rendu, bouton_rectangle, texte_rectangle, image_bouton_clique, texture_rectangle);

	SDL_Rect texte_cercle; //rectangle définissant le positionnement du texte, et sa taille
	texte_cercle.x = 693;
	texte_cercle.y = 124;
	SDL_Texture* texture_cercle = loadText(rendu, "CERCLE", blanc, font);
	if (bouton2_clique == false)
		affichageCaseImage(rendu, bouton_cercle, texte_cercle, image_bouton, texture_cercle);
	else
		affichageCaseImage(rendu, bouton_cercle, texte_cercle, image_bouton_clique, texture_cercle);

	SDL_Rect texte_effacer1; //rectangle définissant le positionnement du texte, et sa taille
	texte_effacer1.x = 690;
	texte_effacer1.y = 194;
	SDL_Texture* texture_effacer1 = loadText(rendu, "EFFACER", blanc, font);
	if (bouton3_clique == false)
		affichageCaseImage(rendu, bouton_effacer1, texte_effacer1, image_bouton, texture_effacer1);
	else
		affichageCaseImage(rendu, bouton_effacer1, texte_effacer1, image_bouton_clique, texture_effacer1);

	SDL_Rect texte_retour; //rectangle définissant le positionnement du texte, et sa taille
	texte_retour.x = 693;
	texte_retour.y = 423;
	SDL_Texture* texture_retour = loadText(rendu, "RETOUR", blanc, font);
	affichageCaseImage(rendu, bouton_retour, texte_retour, image_bouton_quitter, texture_retour);

	SDL_DestroyTexture(image_bouton);
	SDL_DestroyTexture(image_bouton_clique);
	SDL_DestroyTexture(image_bouton_quitter);
	TTF_CloseFont(font);
	SDL_RenderPresent(rendu);
}

void affichageMenuLignes(SDL_Renderer* rendu, SDL_Rect bouton_points, SDL_Rect bouton_verif, SDL_Rect bouton_effacer2, SDL_Rect bouton_effacer_tout, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique, bool bouton3_clique) {
	TTF_Font* font = TTF_OpenFont("Font/Pixeltype.ttf", 24);
	SDL_Color blanc = { 255,255,255 }; //on définit une couleur de texte
	SDL_Texture* image_bouton = loadImage(rendu, "Images/bouton.png");
	SDL_Texture* image_bouton_clique = loadImage(rendu, "Images/bouton_clique.png");
	SDL_Texture* image_bouton_quitter = loadImage(rendu, "Images/bouton_quitter.png");

	SDL_Rect texte_points; //rectangle définissant le positionnement du texte, et sa taille
	texte_points.x = 694;
	texte_points.y = 54;
	SDL_Texture* texture_points = loadText(rendu, "POINTS", blanc, font);
	if (bouton1_clique == false)
		affichageCaseImage(rendu, bouton_points, texte_points, image_bouton, texture_points);
	else
		affichageCaseImage(rendu, bouton_points, texte_points, image_bouton_clique, texture_points);

	SDL_Rect texte_verif; //rectangle définissant le positionnement du texte, et sa taille
	texte_verif.x = 672;
	texte_verif.y = 124;
	SDL_Texture* texture_verif = loadText(rendu, "VERIFICATION", blanc, font);
	if (bouton2_clique == false)
		affichageCaseImage(rendu, bouton_verif, texte_verif, image_bouton, texture_verif);
	else
		affichageCaseImage(rendu, bouton_verif, texte_verif, image_bouton_clique, texture_verif);

	SDL_Rect texte_effacer2; //rectangle définissant le positionnement du texte, et sa taille
	texte_effacer2.x = 668;
	texte_effacer2.y = 194;
	SDL_Texture* texture_effacer2 = loadText(rendu, "EFFACER TOUT", blanc, font);
	if (bouton3_clique == false)
		affichageCaseImage(rendu, bouton_effacer2, texte_effacer2, image_bouton, texture_effacer2);
	else
		affichageCaseImage(rendu, bouton_effacer2, texte_effacer2, image_bouton_clique, texture_effacer2);

	SDL_Rect texte_retour; //rectangle définissant le positionnement du texte, et sa taille
	texte_retour.x = 693;
	texte_retour.y = 423;
	SDL_Texture* texture_retour = loadText(rendu, "RETOUR", blanc, font);
	affichageCaseImage(rendu, bouton_retour, texte_retour, image_bouton_quitter, texture_retour);

	SDL_DestroyTexture(image_bouton);
	SDL_DestroyTexture(image_bouton_clique);
	SDL_DestroyTexture(image_bouton_quitter);
	TTF_CloseFont(font);
	SDL_RenderPresent(rendu);
}

void affichageMenuMesure(SDL_Renderer* rendu, SDL_Rect bouton_rapporteur, SDL_Rect bouton_regle, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique) {
	TTF_Font* font = TTF_OpenFont("Font/Pixeltype.ttf", 24);
	SDL_Color blanc = { 255,255,255 }; //on définit une couleur de texte
	SDL_Texture* image_bouton = loadImage(rendu, "Images/bouton.png");
	SDL_Texture* image_bouton_clique = loadImage(rendu, "Images/bouton_clique.png");
	SDL_Texture* image_bouton_quitter = loadImage(rendu, "Images/bouton_quitter.png");

	SDL_Rect texte_rapporteur; //rectangle définissant le positionnement du texte, et sa taille
	texte_rapporteur.x = 697;
	texte_rapporteur.y = 54;
	SDL_Texture* texture_rapporteur = loadText(rendu, "REGLE", blanc, font);
	if (bouton1_clique == false)
		affichageCaseImage(rendu, bouton_rapporteur, texte_rapporteur, image_bouton, texture_rapporteur);
	else
		affichageCaseImage(rendu, bouton_rapporteur, texte_rapporteur, image_bouton_clique, texture_rapporteur);

	SDL_Rect texte_regle; //rectangle définissant le positionnement du texte, et sa taille
	texte_regle.x = 676;
	texte_regle.y = 124;
	SDL_Texture* texture_regle = loadText(rendu, "RAPPORTEUR", blanc, font);
	if (bouton2_clique == false)
		affichageCaseImage(rendu, bouton_regle, texte_regle, image_bouton, texture_regle);
	else
		affichageCaseImage(rendu, bouton_regle, texte_regle, image_bouton_clique, texture_regle);

	SDL_Rect texte_retour; //rectangle définissant le positionnement du texte, et sa taille
	texte_retour.x = 693;
	texte_retour.y = 423;
	SDL_Texture* texture_retour = loadText(rendu, "RETOUR", blanc, font);
	affichageCaseImage(rendu, bouton_retour, texte_retour, image_bouton_quitter, texture_retour);

	SDL_DestroyTexture(image_bouton);
	SDL_DestroyTexture(image_bouton_clique);
	SDL_DestroyTexture(image_bouton_quitter);
	TTF_CloseFont(font);
	SDL_RenderPresent(rendu);
}

void affichageMenuInstructions(SDL_Renderer* rendu, SDL_Rect bouton_avancer, SDL_Rect bouton_reculer, SDL_Rect bouton_tournerD, SDL_Rect bouton_tournerG, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique, bool bouton3_clique, bool bouton4_clique) {
	TTF_Font* font = TTF_OpenFont("Font/Pixeltype.ttf", 24);
	SDL_Color blanc = { 255,255,255 }; //on définit une couleur de texte
	SDL_Texture* image_bouton = loadImage(rendu, "Images/bouton.png");
	SDL_Texture* image_bouton_clique = loadImage(rendu, "Images/bouton_clique.png");
	SDL_Texture* image_bouton_quitter = loadImage(rendu, "Images/bouton_quitter.png");

	SDL_Rect texte_avancer; //rectangle définissant le positionnement du texte, et sa taille
	texte_avancer.x = 690;
	texte_avancer.y = 54;
	SDL_Texture* texture_avancer = loadText(rendu, "AVANCER", blanc, font);
	if (bouton1_clique == false)
		affichageCaseImage(rendu, bouton_avancer, texte_avancer, image_bouton, texture_avancer);
	else
		affichageCaseImage(rendu, bouton_avancer, texte_avancer, image_bouton_clique, texture_avancer);

	SDL_Rect texte_reculer; //rectangle définissant le positionnement du texte, et sa taille
	texte_reculer.x = 690;
	texte_reculer.y = 124;
	SDL_Texture* texture_reculer = loadText(rendu, "RECULER", blanc, font);
	if (bouton2_clique == false)
		affichageCaseImage(rendu, bouton_reculer, texte_reculer, image_bouton, texture_reculer);
	else
		affichageCaseImage(rendu, bouton_reculer, texte_reculer, image_bouton_clique, texture_reculer);

	SDL_Rect texte_tournerD; //rectangle définissant le positionnement du texte, et sa taille
	texte_tournerD.x = 683;
	texte_tournerD.y = 194;
	SDL_Texture* texture_tournerD = loadText(rendu, "TOURNER D", blanc, font);
	if (bouton3_clique == false)
		affichageCaseImage(rendu, bouton_tournerD, texte_tournerD, image_bouton, texture_tournerD);
	else
		affichageCaseImage(rendu, bouton_tournerD, texte_tournerD, image_bouton_clique, texture_tournerD);

	SDL_Rect texte_tournerG; //rectangle définissant le positionnement du texte, et sa taille
	texte_tournerG.x = 683;
	texte_tournerG.y = 264;
	SDL_Texture* texture_tournerG = loadText(rendu, "TOURNER G", blanc, font);
	if (bouton4_clique == false)
		affichageCaseImage(rendu, bouton_tournerG, texte_tournerG, image_bouton, texture_tournerG);
	else
		affichageCaseImage(rendu, bouton_tournerG, texte_tournerG, image_bouton_clique, texture_tournerG);

	SDL_Rect texte_retour; //rectangle définissant le positionnement du texte, et sa taille
	texte_retour.x = 693;
	texte_retour.y = 423;
	SDL_Texture* texture_retour = loadText(rendu, "RETOUR", blanc, font);
	affichageCaseImage(rendu, bouton_retour, texte_retour, image_bouton_quitter, texture_retour);

	SDL_DestroyTexture(image_bouton);
	SDL_DestroyTexture(image_bouton_clique);
	SDL_DestroyTexture(image_bouton_quitter);
	TTF_CloseFont(font);
	SDL_RenderPresent(rendu);
}

void fenetreInstruction() {
	SDL_Window* win = SDL_CreateWindow("Enregistrement Instruction",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		300, 			//largeur en pixels			
		400, 			//hauteur en pixels
		SDL_WINDOW_SHOWN //d’autres options (plein ecran, resizable, sans bordure...)
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	//Création d’un dessin associé à la fenêtre (1 seul renderer par fenetre)
	SDL_Renderer* renduInstruction = SDL_CreateRenderer(
		win,  //nom de la fenêtre
		-1, //par défaut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

	SDL_Rect fenetreB;
	fenetreB.x = 0;
	fenetreB.y = 0;
	fenetreB.w = 300;
	fenetreB.h = 400;
	SDL_SetRenderDrawColor(renduInstruction, 0, 0, 0, 255);
	SDL_RenderFillRect(renduInstruction, &fenetreB);
	SDL_RenderPresent(renduInstruction);

	bool continuer = true;

	SDL_Event event;//gestion des évènements souris/clavier, 
					//SDL_Event est de type struct
	while (continuer)
	{
		SDL_WaitEvent(&event);//attente d’un évènement
		switch (event.type) //test du type d’évènement
		{
		case SDL_QUIT: //clic sur la croix de fermeture
			continuer = false;
			break;
		}
	}
	//destruction du renderer à la fin
	SDL_DestroyRenderer(renduInstruction);

	//destruction à la fin
	SDL_DestroyWindow(win);   //equivalent du delete

	//fermeture
	SDL_Quit();
}

void affichageFenetreInstruction(SDL_Renderer* renduInstruction) {
	SDL_Rect fenetreInstruction;
	fenetreInstruction.x = 0;
	fenetreInstruction.y = 0;
	fenetreInstruction.w = 300;
	fenetreInstruction.h = 400;
	SDL_SetRenderDrawColor(renduInstruction, 0, 0, 0, 255);
	SDL_RenderFillRect(renduInstruction, &fenetreInstruction);
	SDL_RenderPresent(renduInstruction);
}

void affichageConsoleInstruction(SDL_Renderer* renduInstruction, SDL_Rect bouton_virg, SDL_Rect bouton_suppr, SDL_Rect bouton_0, SDL_Rect bouton_1, SDL_Rect bouton_2, SDL_Rect bouton_3, SDL_Rect bouton_4, SDL_Rect bouton_5, SDL_Rect bouton_6, SDL_Rect bouton_7, SDL_Rect bouton_8, SDL_Rect bouton_9, SDL_Rect bouton_valid) {
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	SDL_Color blanc = { 255,255,255 };

	//Définition de la console
	SDL_Rect cadre_console; //on définit le rectangle à tracer
	cadre_console.x = 25;
	cadre_console.y = 25;
	cadre_console.w = 247;  //largeur
	cadre_console.h = 75;  //hauteur

	SDL_SetRenderDrawColor(renduInstruction, blanc.r, blanc.g, blanc.b, blanc.a);  //pinceau bleu
	SDL_RenderDrawRect(renduInstruction, &cadre_console); //on trace un rectangle vide

	SDL_Rect texte_virg; //rectangle définissant le positionnement du texte, et sa taille
	texte_virg.x = 48;
	texte_virg.y = 333;
	SDL_Texture* texture_virg = loadText(renduInstruction, ".", blanc, font);
	affichageCaseVide(renduInstruction, bouton_virg, texte_virg, texture_virg, blanc);

	SDL_Rect texte_suppr; //rectangle définissant le positionnement du texte, et sa taille
	texte_suppr.x = 161;
	texte_suppr.y = 337;
	SDL_Texture* texture_suppr = loadText(renduInstruction, "DEL", blanc, font);
	affichageCaseVide(renduInstruction, bouton_suppr, texte_suppr, texture_suppr, blanc);

	SDL_Rect texte_0; //rectangle définissant le positionnement du texte, et sa taille
	texte_0.x = 110;
	texte_0.y = 337;
	SDL_Texture* texture_0 = loadText(renduInstruction, "0", blanc, font);
	affichageCaseVide(renduInstruction, bouton_0, texte_0, texture_0, blanc);

	SDL_Rect texte_1; //rectangle définissant le positionnement du texte, et sa taille
	texte_1.x = 44;
	texte_1.y = 273;
	SDL_Texture* texture_1 = loadText(renduInstruction, "1", blanc, font);
	affichageCaseVide(renduInstruction, bouton_1, texte_1, texture_1, blanc);

	SDL_Rect texte_2; //rectangle définissant le positionnement du texte, et sa taille
	texte_2.x = 109;
	texte_2.y = 273;
	SDL_Texture* texture_2 = loadText(renduInstruction, "2", blanc, font);
	affichageCaseVide(renduInstruction, bouton_2, texte_2, texture_2, blanc);

	SDL_Rect texte_3; //rectangle définissant le positionnement du texte, et sa taille
	texte_3.x = 174;
	texte_3.y = 273;
	SDL_Texture* texture_3 = loadText(renduInstruction, "3", blanc, font);
	affichageCaseVide(renduInstruction, bouton_3, texte_3, texture_3, blanc);

	SDL_Rect texte_4; //rectangle définissant le positionnement du texte, et sa taille
	texte_4.x = 44;
	texte_4.y = 208;
	SDL_Texture* texture_4 = loadText(renduInstruction, "4", blanc, font);
	affichageCaseVide(renduInstruction, bouton_4, texte_4, texture_4, blanc);

	SDL_Rect texte_5; //rectangle définissant le positionnement du texte, et sa taille
	texte_5.x = 109;
	texte_5.y = 208;
	SDL_Texture* texture_5 = loadText(renduInstruction, "5", blanc, font);
	affichageCaseVide(renduInstruction, bouton_5, texte_5, texture_5, blanc);

	SDL_Rect texte_6; //rectangle définissant le positionnement du texte, et sa taille
	texte_6.x = 173;
	texte_6.y = 208;
	SDL_Texture* texture_6 = loadText(renduInstruction, "6", blanc, font);
	affichageCaseVide(renduInstruction, bouton_6, texte_6, texture_6, blanc);

	SDL_Rect texte_7; //rectangle définissant le positionnement du texte, et sa taille
	texte_7.x = 44;
	texte_7.y = 143;
	SDL_Texture* texture_7 = loadText(renduInstruction, "7", blanc, font);
	affichageCaseVide(renduInstruction, bouton_7, texte_7, texture_7, blanc);

	SDL_Rect texte_8; //rectangle définissant le positionnement du texte, et sa taille
	texte_8.x = 109;
	texte_8.y = 143;
	SDL_Texture* texture_8 = loadText(renduInstruction, "8", blanc, font);
	affichageCaseVide(renduInstruction, bouton_8, texte_8, texture_8, blanc);

	SDL_Rect texte_9; //rectangle définissant le positionnement du texte, et sa taille
	texte_9.x = 174;
	texte_9.y = 143;
	SDL_Texture* texture_9 = loadText(renduInstruction, "9", blanc, font);
	affichageCaseVide(renduInstruction, bouton_9, texte_9, texture_9, blanc);

	SDL_Rect texte_valid; //rectangle définissant le positionnement du texte, et sa taille
	texte_valid.x = 229;
	texte_valid.y = 237;
	SDL_Texture* texture_valid = loadText(renduInstruction, "OK", blanc, font);
	affichageCaseVide(renduInstruction, bouton_valid, texte_valid, texture_valid, blanc);

	SDL_RenderPresent(renduInstruction);
	TTF_CloseFont(font);
}

void affichageResultat(SDL_Renderer* renduInstruction, double res) {
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	SDL_Color blanc = { 255,255,255 };


	if (res < 10000) {
		char s[50] = "";
		sprintf_s(s, "%3.1f", res);

		SDL_Rect texte_res; //rectangle définissant le positionnement du texte, et sa taille
		texte_res.x = 50;
		texte_res.y = 50;
		SDL_Texture* texture_res = loadText(renduInstruction, s, blanc, font);
		SDL_QueryTexture(texture_res, NULL, NULL, &texte_res.w, &texte_res.h);
		SDL_RenderCopy(renduInstruction, texture_res, NULL, &texte_res);
		SDL_DestroyTexture(texture_res);
		SDL_RenderPresent(renduInstruction);
	}
	else {
		SDL_Rect texte_max; //rectangle définissant le positionnement du texte, et sa taille
		texte_max.x = 50;
		texte_max.y = 50;
		SDL_Texture* texture_max = loadText(renduInstruction, "Valeur trop grande", blanc, font);
		SDL_QueryTexture(texture_max, NULL, NULL, &texte_max.w, &texte_max.h);
		SDL_RenderCopy(renduInstruction, texture_max, NULL, &texte_max);
		SDL_DestroyTexture(texture_max);
		SDL_RenderPresent(renduInstruction);
	}
}

void ecritureFichier(char instr[], float val) {
	ofstream sortie("Instructions/instructions.txt", ios::app);
	sortie << instr << " " << val << endl;
	cout << instr << " " << val << endl;
	sortie.close();
}

float mainInstruction(int argn, char* argv[]) {
	SDL_Window* winInstruction = SDL_CreateWindow("Enregistrement Instruction",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		300,             //largeur en pixels            
		400,             //hauteur en pixels
		SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS //d?autres options (plein ecran, resizable, sans bordure...)
	);

	if (winInstruction == NULL)
		cout << "erreur ouverture fenetre";


	//Création d?un dessin associé à la fenêtre (1 seul renderer par fenetre)
	SDL_Renderer* renduInstruction = SDL_CreateRenderer(
		winInstruction,  //nom de la fenêtre
		-1, //par défaut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

	//Définition des boutons
	SDL_Rect bouton_0; //on définit le rectangle à tracer
	bouton_0.x = 90;
	bouton_0.y = 325;
	bouton_0.w = 50;  //largeur
	bouton_0.h = 50;  //hauteur

	SDL_Rect bouton_1; //on définit le rectangle à tracer
	bouton_1.x = 25;
	bouton_1.y = 260;
	bouton_1.w = 50;  //largeur
	bouton_1.h = 50;  //hauteur

	SDL_Rect bouton_2; //on définit le rectangle à tracer
	bouton_2.x = 90;
	bouton_2.y = 260;
	bouton_2.w = 50;  //largeur
	bouton_2.h = 50;  //hauteur

	SDL_Rect bouton_3; //on définit le rectangle à tracer
	bouton_3.x = 155;
	bouton_3.y = 260;
	bouton_3.w = 50;  //largeur
	bouton_3.h = 50;  //hauteur

	SDL_Rect bouton_4; //on définit le rectangle à tracer
	bouton_4.x = 25;
	bouton_4.y = 195;
	bouton_4.w = 50;  //largeur
	bouton_4.h = 50;  //hauteur

	SDL_Rect bouton_5; //on définit le rectangle à tracer
	bouton_5.x = 90;
	bouton_5.y = 195;
	bouton_5.w = 50;  //largeur
	bouton_5.h = 50;  //hauteur

	SDL_Rect bouton_6; //on définit le rectangle à tracer
	bouton_6.x = 155;
	bouton_6.y = 195;
	bouton_6.w = 50;  //largeur
	bouton_6.h = 50;  //hauteur

	SDL_Rect bouton_7; //on définit le rectangle à tracer
	bouton_7.x = 25;
	bouton_7.y = 130;
	bouton_7.w = 50;  //largeur
	bouton_7.h = 50;  //hauteur

	SDL_Rect bouton_8; //on définit le rectangle à tracer
	bouton_8.x = 90;
	bouton_8.y = 130;
	bouton_8.w = 50;  //largeur
	bouton_8.h = 50;  //hauteur

	SDL_Rect bouton_9; //on définit le rectangle à tracer
	bouton_9.x = 155;
	bouton_9.y = 130;
	bouton_9.w = 50;  //largeur
	bouton_9.h = 50;  //hauteur

	SDL_Rect bouton_virg; //on définit le rectangle à tracer
	bouton_virg.x = 25;
	bouton_virg.y = 325;
	bouton_virg.w = 50;  //largeur
	bouton_virg.h = 50;  //hauteur

	SDL_Rect bouton_suppr; //on définit le rectangle à tracer
	bouton_suppr.x = 155;
	bouton_suppr.y = 325;
	bouton_suppr.w = 50;  //largeur
	bouton_suppr.h = 50;  //hauteur

	SDL_Rect bouton_valid; //on définit le rectangle à tracer
	bouton_valid.x = 220;
	bouton_valid.y = 130;
	bouton_valid.w = 50;  //largeur
	bouton_valid.h = 245;  //hauteur

	bool continuer2 = true, virg = false;
	double res = 0;

	affichageFenetreInstruction(renduInstruction);
	affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
	affichageResultat(renduInstruction, res);

	SDL_Event eventInstruction;//gestion des évènements souris/clavier, 
					//SDL_Event est de type struct
	while (continuer2)
	{
		SDL_WaitEvent(&eventInstruction);//attente d?un évènement
		if (eventInstruction.type == SDL_WINDOWEVENT) {
			if (eventInstruction.type == SDL_WINDOWEVENT_CLOSE) {
				continuer2 = false;
			}
		}
		switch (eventInstruction.type) //test du type d?évènement
		{
		case SDL_MOUSEBUTTONUP:
			if (eventInstruction.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
				if (eventInstruction.button.x > bouton_0.x && eventInstruction.button.x<bouton_0.x + bouton_0.w && eventInstruction.button.y>bouton_0.y && eventInstruction.button.y < bouton_0.y + bouton_0.h) {
					if (virg == false)
						res = res * 10 + 0;
					else
						continue;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_1.x && eventInstruction.button.x<bouton_1.x + bouton_1.w && eventInstruction.button.y>bouton_1.y && eventInstruction.button.y < bouton_1.y + bouton_1.h) {
					if (virg == false)
						res = res * 10 + 1;
					else
						res = res + 0.1;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_2.x && eventInstruction.button.x<bouton_2.x + bouton_2.w && eventInstruction.button.y>bouton_2.y && eventInstruction.button.y < bouton_2.y + bouton_2.h) {
					if (virg == false)
						res = res * 10 + 2;
					else
						res = res + 0.2;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_3.x && eventInstruction.button.x<bouton_3.x + bouton_3.w && eventInstruction.button.y>bouton_3.y && eventInstruction.button.y < bouton_3.y + bouton_3.h) {
					if (virg == false)
						res = res * 10 + 3;
					else
						res = res + 0.3;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_4.x && eventInstruction.button.x<bouton_4.x + bouton_4.w && eventInstruction.button.y>bouton_4.y && eventInstruction.button.y < bouton_4.y + bouton_4.h) {
					if (virg == false)
						res = res * 10 + 4;
					else
						res = res + 0.4;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_5.x && eventInstruction.button.x<bouton_5.x + bouton_5.w && eventInstruction.button.y>bouton_5.y && eventInstruction.button.y < bouton_5.y + bouton_5.h) {
					if (virg == false)
						res = res * 10 + 5;
					else
						res = res + 0.5;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_6.x && eventInstruction.button.x<bouton_6.x + bouton_6.w && eventInstruction.button.y>bouton_6.y && eventInstruction.button.y < bouton_6.y + bouton_6.h) {
					if (virg == false)
						res = res * 10 + 6;
					else
						res = res + 0.6;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_7.x && eventInstruction.button.x<bouton_7.x + bouton_7.w && eventInstruction.button.y>bouton_7.y && eventInstruction.button.y < bouton_7.y + bouton_7.h) {
					if (virg == false)
						res = res * 10 + 7;
					else
						res = res + 0.7;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_8.x && eventInstruction.button.x<bouton_8.x + bouton_8.w && eventInstruction.button.y>bouton_8.y && eventInstruction.button.y < bouton_8.y + bouton_8.h) {
					if (virg == false)
						res = res * 10 + 8;
					else
						res = res + 0.8;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_9.x && eventInstruction.button.x<bouton_9.x + bouton_9.w && eventInstruction.button.y>bouton_9.y && eventInstruction.button.y < bouton_9.y + bouton_9.h) {
					if (virg == false)
						res = res * 10 + 9;
					else
						res = res + 0.9;
					virg = false;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_virg.x && eventInstruction.button.x<bouton_virg.x + bouton_virg.w && eventInstruction.button.y>bouton_virg.y && eventInstruction.button.y < bouton_virg.y + bouton_virg.h) {
					virg = true;
				}
				if (eventInstruction.button.x > bouton_suppr.x && eventInstruction.button.x<bouton_suppr.x + bouton_suppr.w && eventInstruction.button.y>bouton_suppr.y && eventInstruction.button.y < bouton_suppr.y + bouton_suppr.h) {
					res = 0;
					affichageFenetreInstruction(renduInstruction);
					affichageConsoleInstruction(renduInstruction, bouton_virg, bouton_suppr, bouton_0, bouton_1, bouton_2, bouton_3, bouton_4, bouton_5, bouton_6, bouton_7, bouton_8, bouton_9, bouton_valid);
					affichageResultat(renduInstruction, res);
				}
				if (eventInstruction.button.x > bouton_valid.x && eventInstruction.button.x<bouton_valid.x + bouton_valid.w && eventInstruction.button.y>bouton_valid.y && eventInstruction.button.y < bouton_valid.y + bouton_valid.h) {
					continuer2 = false;
				}
			}
		}
	}


	//destruction du renderer à la fin
	SDL_DestroyRenderer(renduInstruction);

	//destruction à la fin
	SDL_DestroyWindow(winInstruction);   //equivalent du delete
	return res;
}

void menu_principal(SDL_Renderer* rendu, SDL_Rect& background, SDL_Rect& lbleu, SDL_Rect& lrouge, SDL_Rect prof, SDL_Rect eleve, SDL_Rect quit, bool& sens) {
	SDL_Texture* t = loadImage(rendu, "Images/fond-defilant.png");
	SDL_RenderCopy(rendu, t, NULL, &background);

	SDL_Texture* t2 = loadImage(rendu, "Images/lego2.png");
	SDL_RenderCopy(rendu, t2, NULL, &lbleu);

	SDL_Texture* t3 = loadImage(rendu, "Images/lego3.png");
	SDL_RenderCopyEx(rendu, t3, NULL, &lrouge, 340, NULL, SDL_FLIP_NONE);

	TTF_Font* font = TTF_OpenFont("Font/Pixeltype.ttf", 52);
	SDL_SetRenderDrawColor(rendu, 100, 100, 100, 255);
	SDL_RenderFillRect(rendu, &prof);
	SDL_RenderFillRect(rendu, &eleve);
	SDL_SetRenderDrawColor(rendu, 10, 10, 10, 255);
	SDL_RenderDrawRect(rendu, &prof);
	SDL_RenderDrawRect(rendu, &eleve);

	SDL_Rect tt1{ prof.x + 12,prof.y + 12,prof.w - 40,prof.h - 10 };
	SDL_Texture* txt1 = loadText(rendu, "MODE ENSEIGNANT", SDL_Color{ 30,30,30,255 }, font);
	SDL_QueryTexture(txt1, NULL, NULL, &tt1.w, &tt1.h);
	SDL_RenderCopy(rendu, txt1, NULL, &tt1);

	SDL_Rect tt2{ eleve.x + 60,eleve.y + 12,eleve.w - 40,eleve.h - 10 };
	SDL_Texture* txt2 = loadText(rendu, "MODE ELEVE", SDL_Color{ 30,30,30,255 }, font);
	SDL_QueryTexture(txt2, NULL, NULL, &tt2.w, &tt2.h);
	SDL_RenderCopy(rendu, txt2, NULL, &tt2);
	SDL_SetRenderDrawColor(rendu, 220, 0, 0, 255);                //QUITTER
	SDL_RenderFillRect(rendu, &quit);
	SDL_SetRenderDrawColor(rendu, 10, 10, 10, 255);
	SDL_RenderDrawRect(rendu, &quit);
	SDL_Rect ttq{ quit.x + 38, quit.y + 8, quit.w - 40, quit.h - 10 };
	SDL_Texture* txtq = loadText(rendu, "QUITTER", SDL_Color{ 30,30,30,255 }, font);
	SDL_QueryTexture(txtq, NULL, NULL, &ttq.w, &ttq.h);
	SDL_RenderCopy(rendu, txtq, NULL, &ttq);

	TTF_Font* font_title = TTF_OpenFont("Font/LEGO BRIX.ttf", 72);
	SDL_Rect ttt{ 220, 70, 400, 150 };
	SDL_Texture* txtt = loadText(rendu, "LEGOLOGO", SDL_Color{ 30,30,30,255 }, font_title);
	SDL_QueryTexture(txtt, NULL, NULL, &ttt.w, &ttt.h);
	SDL_RenderCopy(rendu, txtt, NULL, &ttt);

	for (int t = 0; t < 5; t++) {
		if (sens == true) {
			lbleu.y--;
			lrouge.y--;
			if (lbleu.y <= 160)
				sens = false;
		}
		else if (sens == false) {
			lbleu.y++;
			lrouge.y++;
			if (lbleu.y >= 180)
				sens = true;
		}
		SDL_RenderPresent(rendu);
	}

	if (background.y == 0) {
		background.y = -1706;
	}
	else {
		background.y++;
	}

	SDL_DestroyTexture(txtt);
	TTF_CloseFont(font_title);
	SDL_DestroyTexture(txtq);
	SDL_DestroyTexture(txt2);
	SDL_DestroyTexture(txt1);
	TTF_CloseFont(font);
	SDL_DestroyTexture(t3);
	SDL_DestroyTexture(t2);
	SDL_DestroyTexture(t);
}