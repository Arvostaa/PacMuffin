/*
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
#include "field.h"
#include "object.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

char running = 1;
//float posX = 50;
//float posY = 50;
//int directionX = 1; //BLOKADA
//int directionY = 1;
//float speed = 10;
//float accelerationX = 0, accelerationY = 0;
//int size = 50;

#define SCREEN_W 800
#define SCREEN_H 600

object_s muffin;

field_s map[16][12];

SDL_Surface* Surf_Display;
SDL_Surface* Surf_Pig;
SDL_Surface* Surf_Path;
SDL_Surface* Surf_Wall;
SDL_Surface* Surf_Higlight;



void OnKeyDowndd(SDLKey sym) {
	switch(sym) {

	case (SDLK_d): {
		muffin.accelerationX = 0.5; /////////////////////////////////////////
		muffin.accelerationY = 0;
		break;
	}
	case (SDLK_a): {
		muffin.accelerationX = -0.5;
		muffin.accelerationY = 0;
		break;
	}
	case (SDLK_w): {
		muffin.accelerationY = -0.5;
		muffin.accelerationX = 0;
		break;
	}
	case (SDLK_s): {
		muffin.accelerationY = 0.5;
		muffin.accelerationX = 0;
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


int displaySurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
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


muffin.posX += muffin.speed*muffin.accelerationX;

	if((muffin.posX < 0))
	{
		muffin.posX = 0;
	}
	else if ( muffin.posX > (SCREEN_W - muffin.size))
	{
		muffin.posX = SCREEN_W - muffin.size ;

	}

	muffin.posY += muffin.speed*muffin.accelerationY;

	if (muffin.posY < 0)
	{
		muffin.posY = 0;
	}
	else if (muffin.posY > (SCREEN_H - muffin.size))
	{
		muffin.posY = SCREEN_H - muffin.size;
	}
}


int isObjectOnTile (int objX, int objY, int tileX, int tileY){

	if((objX == tileX*muffin.size) && (objY == tileY*muffin.size)){

		return 1;
	}
	return 0;
	}

void higlightTile(){

int i = muffin.posX/muffin.size;
int j = muffin.posY/muffin.size;
	map[i][j].type = FIELD_HIGLIGHT;

	if((muffin.accelerationX > 0)){
		map[i-1][j].type = FIELD_PATH;
	}

	if((muffin.accelerationX < 0) ){
		map[i+1][j].type = FIELD_PATH;
	}
	if((muffin.accelerationY < 0) ){
			map[i][j+1].type = FIELD_PATH;
		}
	if((muffin.accelerationY > 0) ){
				map[i][j-1].type = FIELD_PATH;
			}

}

void higlightTile2(int X, int Y){
	map[X][Y].higlighted = 1;

}
void higlightMuffinTile(){
	int i, j;
	for(i = 0; i < 18; i++){
		for(j = 0; j < 14; j++){
			if(isObjectOnTile(muffin.posX, muffin.posY, i, j)){
				higlightTile2(muffin.posX, muffin.posY);
			}
		}
	}
}

void doGraphics() {

	int i,j;
	for(i = 0; i < 18; i++) {
		for(j = 0; j < 14; j++) {


			if(map[i][j].higlighted){
							displaySurface(Surf_Display, Surf_Higlight, i*Surf_Higlight->w, j*Surf_Higlight->h);
							}
			else{

			switch(map[i][j].type) {

			case(FIELD_PATH): {
				displaySurface(Surf_Display, Surf_Path, i*Surf_Path->w, j*Surf_Path->h);
				break;
			}
			case(FIELD_WALL): {
			displaySurface(Surf_Display, Surf_Wall, i*Surf_Wall->w, j*Surf_Wall->h);

			break;
					}

		//	case(FIELD_HIGLIGHT): {
		//		displaySurface(Surf_Display, Surf_Higlight, i*Surf_Higlight->w, j*Surf_Higlight->h);
		//	break;
	//		}
			default:{}

					}
				}
			}
}

			displaySurface(Surf_Display, Surf_Pig, muffin.posX, muffin.posY);

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

			if(isObjectOnTile(muffin.posX, muffin.posY, i, j)){

				if((inDirectionXd(muffin.accelerationX)) && (muffin.posX != muffin.size)){

				map[i+1][j].type = FIELD_PATH;

				}
			}
		}
	}

}

////////////////////////////////////////////////////




int main(void) {

	int i, j, k, l;

	muffin.size = 50;
	muffin.speed = 10;
	muffin.posX = 50;
	muffin.posY = 50;

	for(i = 0; i < 18; i++) {
		for(j = 0; j < 14; j++) {
			map[i][j].type = FIELD_WALL;

		}
	}
	for(k = 1; k < 11; k++){


		map[1][k].type = FIELD_PATH;
		map[14][k].type = FIELD_PATH;
	}
	for(l = 1; l < 14; l++){
		map[l][1].type = FIELD_PATH;
		map[l][8].type = FIELD_PATH;
		map[l][10].type = FIELD_PATH;
	}


	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}
	SDL_EnableKeyRepeat(20,1);


	if((Surf_Display = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return 0;
	}


	if((Surf_Pig = SDL_LoadBMP("piggy.bmp")) == NULL) {

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
			higlightMuffinTile();
			doGraphics();

			SDL_Flip(Surf_Display);

		}
}
