test/*
 ============================================================================
 Name        : sdl_test.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

char running = 1;
float posX = 50;
float posY = 50; //MODYFIKACJA
int directionX = 1; //BLOKADA
int directionY = 1;
float speed = 10;
float accelerationX = 0, accelerationY = 0;
int size = 50;

#define SCREEN_W 800
#define SCREEN_H 600


typedef enum FIELD_TYPE {

	FIELD_WALL = 0,
	FIELD_HIGLIGHT = 1,
	FIELD_PATH = 2

}fieldType_e;

fieldType_e map[16][12];

SDL_Surface* Surf_Display;
SDL_Surface* Surf_Pig;
SDL_Surface* Surf_Path;
SDL_Surface* Surf_Wall;
SDL_Surface* Surf_Higlight;



int delaycounter = 19999999;
int delaycounter2 = 19999999;

void OnKeyDowndd(SDLKey sym) {
	switch(sym) {

	case (SDLK_d): {
		accelerationX = 0.5;
		accelerationY = 0;
		break;
	}
	case (SDLK_a): {
		accelerationX = -0.5;
		accelerationY = 0;
		break;
	}
	case (SDLK_w): {
		accelerationY = -0.5;
		accelerationX = 0;
		break;
	}
	case (SDLK_s): {
		accelerationY = 0.5;
		accelerationX = 0;
		break;
	}
	default: {}
	}

}

void OnEvent(SDL_Event* Event) {

	switch(Event->type) {
	case(SDL_QUIT): {
		running = 0;
		break;
	}
	case(SDL_KEYDOWN): {
		OnKeyDowndd(Event->key.keysym.sym);
		break;
	}
	case(SDL_KEYUP): {
		break;
	}
	case(SDL_MOUSEBUTTONDOWN): {
		break;
	}
	}
}



int displaySurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {//bool CSurface::ondraw
	if(Surf_Dest == NULL || Surf_Src == NULL) {
		return 0;
	}

	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);//

	return 0;
}

void doLogic() {

	posX += speed*accelerationX;

	if((posX < 0))
	{
		posX = 0;
	}
	else if ( posX > (SCREEN_W - size))
	{
		posX = SCREEN_W - size ;

	}

	posY += speed*accelerationY;

	if (posY < 0)
	{
		posY = 0;
	}
	else if (posY > (SCREEN_H - size))
	{
		posY = SCREEN_H - size;
	}
	}

void doGraphics() {

	int i,j;
	for(i = 0; i < 18; i++) {
		for(j = 0; j < 14; j++) {

			switch(map[i][j]) {

			case(FIELD_PATH): {
				displaySurface(Surf_Display, Surf_Path, i*Surf_Path->w, j*Surf_Path->h);
				break;
			}
			case(FIELD_WALL): {
			displaySurface(Surf_Display, Surf_Wall, i*Surf_Wall->w, j*Surf_Wall->h);

			break;
					}

			case(FIELD_HIGLIGHT): {
				displaySurface(Surf_Display, Surf_Higlight, i*Surf_Higlight->w, j*Surf_Higlight->h);
			break;
			}
			default:{}
					}
				}
			}

			displaySurface(Surf_Display, Surf_Pig, posX, posY);
		}

int isObjectOnTile (int objX, int objY, int tileX, int tileY){
	if((objX == tileX*size) && (objY == tileY*size)){

		return 1;
	}
	return 0;
	}

void higlightTile(){

int i = posX/size;
int j = posY/size;
	map[i][j] = FIELD_HIGLIGHT;

	if((accelerationX > 0)){
		map[i-1][j] = FIELD_PATH;
	}

	if((accelerationX < 0) ){
		map[i+1][j] = FIELD_PATH;
	}
	if((accelerationY < 0) ){
			map[i][j+1] = FIELD_PATH;
		}
	if((accelerationY > 0) ){
				map[i][j-1] = FIELD_PATH;
			}

	//map[i-1][j] = FIELD_PATH;
//	map[i+1][j] = FIELD_PATH;

}


void higlightMuffinTile(){
	int i, j;
	for(i = 0; i < 18; i++){
		for(j = 0; j < 14; j++){
			if(isObjectOnTile(posX, posY, i, j)){
				higlightTile();
			}
		}
	}
}

////////////////** chodzenie po ścieżce ** /////////////////
/*int isOnFieldPath(int i, int j){
	for(i = 0; i < 18; i++){
		for(j = 0, j < 14, j++){
			if(isObjectOnTile){
				ifmap[i][j] = FIELD_WALL{

				}
			}
		}
	}
}
*/


///////////////////////////////////////////////////////
int inDirectionXd(float accelX){
	if(accelX > 0){
		return 1;
	}
	else return 0;
}

int inDirectionXa(float accelX){
	if(accelX < 0){
		return 1;
	}
	else return 0;
}

int inDirectionYs(float accelY){
	if(accelY > 0){
		return 1;
	}
	else return 0;
}

int inDirectionYw(float accelerationY){
	if(accelerationY < 0){
		return 1;
	}
	else return 0;
}

void dishiglightTile(){


	int i, j;
	for(i = 0; i < 18; i++){
		for(j = 0; j < 14; j++){

			if(isObjectOnTile(posX, posY, i, j)){

				if((inDirectionXd(accelerationX)) && (posX != size)){

				map[i+1][j] = FIELD_PATH;

				}
			}
		}
	}

}

////////////////////////////////////////////////////




int main(void) {


	int i, j, k, l;


	for(i = 0; i < 18; i++) {
		for(j = 0; j < 14; j++) {
			map[i][j] = FIELD_WALL;

		}
	}
	for(k = 1; k < 11; k++){


		map[1][k] = FIELD_PATH;
		map[14][k] = FIELD_PATH;
	}
	for(l = 1; l < 14; l++){
		map[l][1] = FIELD_PATH;
		map[l][8] = FIELD_PATH;
		map[l][10] = FIELD_PATH;
	}
	/*map[1][4] = FIELD_WATER;
	map[1][5] = FIELD_WATER;
	map[1][6] = FIELD_WATER;
	map[1][7] = FIELD_WATER;
	map[1][8] = FIELD_WATER;
	map[1][9] = FIELD_WATER;
	map[1][10] = FIELD_WATER;
	map[2][10] = FIELD_WATER;
	map[3][10] = FIELD_WATER;
	map[4][10] = FIELD_WATER;
	map[5][10] = FIELD_WATER;
	map[6][10] = FIELD_WATER;
	map[7][10] = FIELD_WATER;
	map[8][10] = FIELD_WATER;
	*/


	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}
	SDL_EnableKeyRepeat(20,1);


	if((Surf_Display = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return 0;
	}


	if((Surf_Pig = SDL_LoadBMP("piggy.bmp")) == NULL) { // SDL_LoadMap

		printf("error while loading BMP\n");

	}
	Uint32 colorkey = SDL_MapRGB( Surf_Pig->format, 0, 0, 0 );
			SDL_SetColorKey(Surf_Pig, SDL_SRCCOLORKEY, colorkey);

	if((Surf_Wall = SDL_LoadBMP("wall.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}


	if((Surf_Path = SDL_LoadBMP("path.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}

	if((Surf_Higlight = SDL_LoadBMP("higlight.bmp")) == NULL) {
			printf("error while loading BMP\n");
		}

	SDL_Event Event;

	while(running) {


			while(SDL_PollEvent(&Event)) {
				OnEvent(&Event);
			}

			SDL_FillRect(Surf_Display, NULL, 12852252);
			doLogic();
			doGraphics();
			higlightMuffinTile();
		//	dishiglightTile();


			//printf("%lf     %lf  \n\n", accelerationY, accelerationX);
			//if(accelerationX > 0){
			//	printf("vsd\n\n");
			//}
			SDL_Flip(Surf_Display);

		}

	}
