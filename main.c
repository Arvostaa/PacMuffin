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
#define MAP_W 16
#define MAP_H 12
#define TILE_SIZE 50
object_s muffin;

field_s map[16][12];

SDL_Surface* Surf_Display;
SDL_Surface* Surf_Pig;
SDL_Surface* Surf_Path;
SDL_Surface* Surf_Wall;
SDL_Surface* Surf_Highlight;



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


int isObjectOnTile (int objX, int objY, int tileX, int tileY){

	if((objX == tileX*muffin.size) && (objY == tileY*muffin.size)){

		return 1;
	}
	return 0;
}


int isTileOnTile(int x1, int y1, int x2, int y2, int size){

	if((x1 == x2) && (y1==y2)){
		return 1;
	}
	else if (y1 == y2){

		if (   (x1 <= x2)  &&  ( abs(x1 - x2) < size)   )  {

			return 1;
		}
		else if ( (x1 <= x2)  && ( abs(x1 - x2) == 0 ) ) {

			return 1;
		}
		else if ( (x2 <= x1) &&  ( abs(x1 - x2) < size)  ) {
			return 1;
		}
		else if (((x2 <= x1)  &&  (abs(x1 - x2) == 0)) ){
			return 1;
		}
		return 0;
	}

	if((x1 == x2) && (y1==y2)){
			return 1;
		}
	else if (x1 == x2){

		if (   (y1 <= y2)  &&  ( abs(y1 - y2) < size)   )  {

			return 1;
		}
		else if ( (y1 <= y2)  && ( abs(y1 - y2) == 0 ) ) {

			return 1;
		}
		else if ( (y2 <= y1) &&  ( abs(y1 - y2) < size)  ) {
			return 1;
		}
		else if (((y2 <= y1)  &&  (abs(y1 - y2) == 0)) ){
			return 1;
		}
		return 0;
	}
	else return 0;
}

int isObjectOnWall(int wallX, int wallY){

	if(map[wallX][wallY].type == FIELD_WALL){
			return 1;
		}
	else return 0;
}




void goToPathx(int wallX, int wallY){


		if(muffin.accelerationX < 0){
			muffin.posX = map[wallX][wallY].x + muffin.size;
		}
		else if(muffin.accelerationX > 0){
			muffin.posX = map[wallX][wallY].x - muffin.size;

		}
	}

void goToPathy(int wallX, int wallY){
		if(muffin.accelerationY < 0){
			muffin.posY = map[wallX][wallY].y + muffin.size;
		}
		else if(muffin.accelerationY > 0){
			muffin.posY = map[wallX][wallY].y - muffin.size;
		}

}

void goToPath(){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile(muffin.posX, muffin.posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if (isObjectOnWall(i,j)){
					goToPathx(i, j);
					goToPathy(i, j);
				}
			}

		}
	}
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





void highlightTile2(int X, int Y){
	map[X][Y].highlighted = 1;
}

void unhighlightTile2(int X, int Y){
	map[X][Y].highlighted = 0;
}

void highlightMuffinTile(){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if(isObjectOnTile(muffin.posX, muffin.posY, i, j)){
				highlightTile2(i, j);
			}
			else{
				unhighlightTile2(i,j);
			}
		}
	}
}


void initFields (){

	int i, j, k, l;

	for(i = 0; i < MAP_W; i++) {
		for(j = 0; j < MAP_H; j++) {
			map[i][j].type = FIELD_WALL;
			map[i][j].x = i*TILE_SIZE;
			map[i][j].y = j*TILE_SIZE;


		}
	}

	for(k = 1; k < MAP_H - 1; k++){

		map[1][k].type = FIELD_PATH;

		map[1][k].x = TILE_SIZE;
		map[1][k].y = TILE_SIZE;
		map[14][k].type = FIELD_PATH;

		map[14][k].x = 14*TILE_SIZE;
		map[14][k].y =k*TILE_SIZE;
	}

	for(l = 1; l < MAP_W - 2; l++){
		map[l][1].type = FIELD_PATH;

		map[l][1].x = l*TILE_SIZE;
		map[l][1].y = TILE_SIZE;
		map[l][8].type = FIELD_PATH;
		;
		map[l][8].x = l*TILE_SIZE;
		map[l][8].y = 8*TILE_SIZE;
		map[l][10].type = FIELD_PATH;

		map[l][10].x = l*TILE_SIZE;
		map[l][10].y = 10*TILE_SIZE;
	}

}

void doGraphics() {

	highlightMuffinTile();


	int a,b;
	for(a = 0; a < MAP_W; a++) {
		for(b = 0; b < MAP_H; b++) {
			//if(isTileOnTile(muffin.posX, muffin.posY, map[a][b].x, map[a][b].y, TILE_SIZE)){
			//	printf("zazaza\n\n");
		//	}else printf("NOTNOT\n");

			if(map[a][b].highlighted){
			displaySurface(Surf_Display, Surf_Highlight, a*Surf_Highlight->w, b*Surf_Highlight->h);
			}
			else{

				switch(map[a][b].type) {

				case(FIELD_PATH): {
					displaySurface(Surf_Display, Surf_Path, a*Surf_Path->w, b*Surf_Path->h);
					break;
				}
				case(FIELD_WALL): {
					displaySurface(Surf_Display, Surf_Wall, a*Surf_Wall->w, b*Surf_Wall->h);

					break;
				}

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



int main(void) {

	muffin.size = 50;
	muffin.speed = 10;
	muffin.posX = 50;
	muffin.posY = 50;


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

	if((Surf_Highlight = SDL_LoadBMP("highlight.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}

	SDL_Event Event;

	while(running) {

		while(SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}

		SDL_FillRect(Surf_Display, NULL, 12852252);


		doLogic();
		goToPath();

		initFields();
		doGraphics();

		SDL_Flip(Surf_Display);

	}
}
