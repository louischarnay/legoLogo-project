#include<SDL.h> //ne pas oublier
#include<SDL_ttf.h> //ne pas oublier
#include<iostream>
#include<fstream>
#include<ctime>
#include "config_sdl.h"
#include<math.h>

using namespace std;

struct Coord {
	int x;
	int y;
};

enum shape { CARRE, CERCLE };

struct Form {
	shape s;
	Coord centre;
	Coord radius;
};

const int LARGEUR = 640; //largeur fenetre
const int HAUTEUR = 480;  //hauteur fenetre
const int FEN_LARGEUR = LARGEUR + 160;
const int FEN_HAUTEUR = HAUTEUR;
const int CONTOUR_POINT = 4;
const int LARGEUR_ROBOT = 15;
const double pi = 3.14159265359;
const int HAUTEUR_REGLE = 300;
const int ECHELLE = HAUTEUR_REGLE / 20;


void fill_circle(SDL_Renderer* rendu, int cx, int cy, int radius);
Form* AjoutForm(Form* tab, Form f, int& n);
Form* obstacle_add(SDL_Renderer* rendu, int x1, int y1, int x2, int y2, Form* tabForm, int& nbform, shape s);
void tracer_obstacle(SDL_Renderer* rendu, Form f);
void affiche_obstacle(SDL_Renderer* rendu, Form* tabform, int nforme);
void previsualisation_forme(SDL_Renderer* rendu, SDL_Window* win, int x1, int y1, int x2, int y2, shape s);
void tracer_trait(SDL_Renderer* rendu, int x1, int y1, int x2, int y2);
Coord* allouerplus(Coord* coord, int& n);
void tracer_contour(SDL_Renderer* rendu, int x, int y);
void previsualisation_trait(SDL_Renderer* rendu, SDL_Window* win, int x, int y, Coord* points, int n);
void affiche_trait(SDL_Renderer* rendu, Coord* points, int n);
void affiche_tout(SDL_Renderer* rendu, SDL_Window* win, Coord* points, int npoints, Form*, int);
void verification(SDL_Renderer* rendu, Coord* points, int npoints, Form* obstacles, int nobstacles);
void check_collision(SDL_Renderer* rendu, int x1, int y1, Form* obstacles, int nbobstacles);
void init_boutons(SDL_Renderer* rendu);
Coord* effacer_points(Coord* points, int& n);
Coord* lecture_deplacement(const char nom_fichier[], Coord* points, int& nbpoints);
double convertir_degre(double degre);
double convertir_rad(double rad, int x1, int y1, int x2, int y2);
void tracer_rapporteur(SDL_Renderer* rendu, SDL_Texture* rapporteur, int xcentre, int ycentre, int x, int y);
void tracer_regle(SDL_Renderer* rendu, SDL_Texture* regle, int xcoin, int ycoin, int x, int y);
Form* effacer_obstacle(Form* obstacles, int n, int& nobstacles);
void affiche_instructions(const char nom_fichier[]);
void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);
void init_menubutton(SDL_Renderer* rendu);
void initialisation(SDL_Renderer* rendu);
void affichageCaseImage(SDL_Renderer* rendu, SDL_Rect bouton_nom, SDL_Rect texte_nom, SDL_Texture* image_nom, SDL_Texture* texture_nom);
void affichageCaseVide(SDL_Renderer* rendu, SDL_Rect bouton_nom, SDL_Rect texte_nom, SDL_Texture* texture_nom, SDL_Color couleur);
void croixRouge(SDL_Renderer* rendu, SDL_Rect bouton_nom);
void affichageMenu(SDL_Renderer* rendu, SDL_Rect bouton_obstacles, SDL_Rect bouton_ligne, SDL_Rect bouton_mesure, SDL_Rect bouton_instruction, SDL_Rect bouton_quitter);
void affichageMenuObstacles(SDL_Renderer* rendu, SDL_Rect bouton_rectangle, SDL_Rect bouton_cercle, SDL_Rect bouton_effacer1, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique, bool bouton3_clique);
void affichageMenuLignes(SDL_Renderer* rendu, SDL_Rect bouton_points, SDL_Rect bouton_verif, SDL_Rect bouton_effacer2, SDL_Rect bouton_effacer_tout, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique, bool bouton3_clique);
void affichageMenuMesure(SDL_Renderer* rendu, SDL_Rect bouton_rapporteur, SDL_Rect bouton_regle, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique);
void affichageMenuInstructions(SDL_Renderer* rendu, SDL_Rect bouton_avancer, SDL_Rect bouton_reculer, SDL_Rect bouton_tournerD, SDL_Rect bouton_tournerG, SDL_Rect bouton_retour, bool bouton1_clique, bool bouton2_clique, bool bouton3_clique, bool bouton4_clique);
void fenetreInstruction();
void affichageFenetreInstruction(SDL_Renderer* renduInstruction);
void affichageConsoleInstruction(SDL_Renderer* renduInstruction, SDL_Rect bouton_virg, SDL_Rect bouton_suppr, SDL_Rect bouton_0, SDL_Rect bouton_1, SDL_Rect bouton_2, SDL_Rect bouton_3, SDL_Rect bouton_4, SDL_Rect bouton_5, SDL_Rect bouton_6, SDL_Rect bouton_7, SDL_Rect bouton_8, SDL_Rect bouton_9, SDL_Rect bouton_valid);
void affichageResultat(SDL_Renderer* renduInstruction, double res);
void ecritureFichier(char instr[], float val);
float mainInstruction(int argn, char* argv[]);
void menu_principal(SDL_Renderer* rendu, SDL_Rect& background, SDL_Rect& lbleu, SDL_Rect& lrouge, SDL_Rect prof, SDL_Rect eleve, SDL_Rect quit, bool& sens);