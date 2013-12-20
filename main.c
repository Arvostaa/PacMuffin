/*
 ============================================================================
 Name        : PacMUFFIN
 Author      :AnnSko
 Version     :0.1
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "field.h"
#include "object.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

char running = 1;
int release_counter = 1;
int release_pos = 999;

#define SCREEN_W 800
#define SCREEN_H 600
#define MAP_W 16
#define MAP_H 12
#define TILE_SIZE 50

object_s muffin;
object_s ghost1;
object_s ghost2;
object_s ghost3;
object_s ghost4;
field_s map[16][12];

SDL_Surface* Surf_Display;
SDL_Surface* Surf_Pig;
SDL_Surface* Surf_Path;
SDL_Surface* Surf_Wall;
SDL_Surface* Surf_Gate;
SDL_Surface* Surf_Highlight;
SDL_Surface* Surf_Ghost1;
SDL_Surface* Surf_Ghost2;
SDL_Surface* Surf_Ghost3;
SDL_Surface* Surf_Ghost4;

void OnKeyDowndd(SDLKey sym) {
	switch(sym) {

	case (SDLK_d): {
		muffin.accelerationX = 0.5;
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
	else if ( ( abs(y1 - y2) < size) && ( abs(x1 - x2) < size) ){
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

int isObjectOnPath(int wallX, int wallY){

	if(map[wallX][wallY].type == FIELD_PATH){
		return 1;
	}
	else return 0;
}

void ghost1ToWallx(int wallX, int wallY, object_s *ghost1){
	if(ghost1->accelerationX < 0){
		ghost1->posX = map[wallX][wallY].x + TILE_SIZE;
	}
	else if(ghost1->accelerationX > 0){
		ghost1->posX = map[wallX][wallY].x - TILE_SIZE;
	}
}

void ghost1ToWally(int wallX, int wallY, object_s *ghost1){
	if(ghost1->accelerationY < 0){

		ghost1->posY = map[wallX][wallY].y + TILE_SIZE;
	}
	else if(ghost1->accelerationY > 0){
		ghost1->posY = map[wallX][wallY].y - TILE_SIZE;
	}

}

void goToPathx(int wallX, int wallY, object_s *obj){

	if((obj->accelerationX) < 0){
		obj->posX= map[wallX][wallY].x + TILE_SIZE;
	}
	else if(obj->accelerationX > 0){
		obj->posX = map[wallX][wallY].x - TILE_SIZE;
	}
}

void goToPathy(int wallX, int wallY, object_s *obj){
	if(obj->accelerationY < 0){

		obj->posY = map[wallX][wallY].y + TILE_SIZE;
	}
	else if(obj->accelerationY > 0){
		obj->posY = map[wallX][wallY].y - TILE_SIZE;
	}

}

void goToPathMuffin(){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile(muffin.posX, muffin.posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if (isObjectOnWall(i,j)){
					goToPathx(i, j, &muffin);
					goToPathy(i, j, &muffin);
				}
			}

		}
	}
}
int isGhostInGate(int wallX, int wallY){

	if(map[wallX][wallY].y == 3*TILE_SIZE){
		return 1;
	}
	else return 0;
}

void changeReleasePos (){
	release_pos = 3*TILE_SIZE;
}

void finallyReleaseTheGhost(object_s *ghost){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile(ghost->posX, ghost->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if(isGhostInGate(i,j)){
					if(release_pos == 3*TILE_SIZE){
						ghost->posY = j*TILE_SIZE;
						ghost->accelerationY = 0;
						ghost->accelerationX = -0.5;
					}
				}
			}
		}
	}
}

void walkinGhostdown(){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if(release_counter%11 == 0){
				release_pos = 3*TILE_SIZE;
				ghost1.accelerationY = -(ghost1.accelerationY);
				ghost2.accelerationY = -(ghost2.accelerationY);
				ghost3.accelerationY = -(ghost3.accelerationY);
				ghost4.accelerationY = -(ghost4.accelerationY);
			}

			if (isTileOnTile(ghost4.posX, ghost4.posY, map[i][j].x, map[i][j].y, TILE_SIZE)){

						if(isObjectOnPath(i, j)){
						goToPathy(i, j, &ghost4);
						ghost1.accelerationY = -(ghost1.accelerationY);
						ghost2.accelerationY = -(ghost2.accelerationY);
						ghost3.accelerationY = -(ghost3.accelerationY);
						ghost4.accelerationY = -(ghost4.accelerationY);
						release_counter++;
					}

					printf("%d\n", release_counter);
				}
			}

		}
	}

	void doLogic() {

		muffin.posX += muffin.speed*muffin.accelerationX;
		//
		if(muffin.posY < 200){
			ghost2.close_counter = 1;
			ghost3.close_counter = 1;
		}
		if(muffin.posY < 100){
			ghost4.close_counter = 1;
		}
		//
		if((muffin.posX < 0))
		{
			muffin.posX = 0;
		}
		else if ( muffin.posX > (SCREEN_W - muffin.size))
		{
			muffin.posX = SCREEN_W - muffin.size;

		}

		muffin.posY += muffin.speed*muffin.accelerationY;

		ghost1.posY += ghost1.speed*ghost1.accelerationY;
		ghost2.posY += ghost1.speed*ghost2.accelerationY;
		ghost3.posY += ghost1.speed*ghost3.accelerationY;
		ghost4.posY += ghost1.speed*ghost4.accelerationY;

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

		int i, j, k, l,m,n;

		for(i = 0; i < MAP_W; i++) {
			for(j = 0; j < MAP_H; j++) {
				map[i][j].type = FIELD_WALL;
				map[i][j].x = i*TILE_SIZE;
				map[i][j].y = j*TILE_SIZE;
			}
		}

		for(k = 1; k < MAP_H - 1; k++){
			map[1][k].type = FIELD_PATH; // 1 i 14: kolumny
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
			map[l][5].type = FIELD_PATH;
			map[l][5].x = l*TILE_SIZE;
			map[l][5].y = 5*TILE_SIZE;
			map[l][8].type = FIELD_PATH;
			map[l][8].x = l*TILE_SIZE;
			map[l][8].y = 8*TILE_SIZE;
			map[l][10].type = FIELD_PATH;

			map[l][10].x = l*TILE_SIZE;
			map[l][10].y = 10*TILE_SIZE;
		}
		for(n = 1; n < 6; n++){
			map[5][n].type = FIELD_PATH;
			map[5][n].x = 5*TILE_SIZE;
			map[5][n].y = n*TILE_SIZE;
		}

		for(m = 1; m < 6; m++){
			map[10][m].type = FIELD_PATH;
			map[10][m].x = 10*TILE_SIZE;
			map[10][m].y = m*TILE_SIZE;
		}
		map[11][9].type = FIELD_PATH;
		map[11][9].x = 11*TILE_SIZE;
		map[11][9].y = 9*TILE_SIZE;
		map[4][9].type = FIELD_PATH;
		map[4][9].x = 4*TILE_SIZE;
		map[4][9].y = 8*TILE_SIZE;

		map[5][3].type = FIELD_GATE;
		map[5][3].x = 8*TILE_SIZE;
		map[5][3].y = 3*TILE_SIZE;

		map[10][3].type = FIELD_GATE;
		map[10][3].x = 7*TILE_SIZE;
		map[10][3].y = 3*TILE_SIZE;

	}

	void doGraphics() {

		highlightMuffinTile();

		int a,b;
		for(a = 0; a < MAP_W; a++) {
			for(b = 0; b < MAP_H; b++) {

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
					case(FIELD_GATE): {
						displaySurface(Surf_Display, Surf_Gate, a*Surf_Gate->w, b*Surf_Gate->h);
						break;
					}

					default:{}

					}
				}
			}

		}

		displaySurface(Surf_Display, Surf_Pig, muffin.posX, muffin.posY);
		displaySurface(Surf_Display, Surf_Ghost1, ghost1.posX, ghost1.posY);
		displaySurface(Surf_Display, Surf_Ghost2, ghost2.posX, ghost2.posY);
		displaySurface(Surf_Display, Surf_Ghost3, ghost3.posX, ghost3.posY);
		displaySurface(Surf_Display, Surf_Ghost4, ghost4.posX, ghost4.posY);

	}

	int main(void) {

		muffin.size = 50;
		muffin.speed = 10;
		muffin.posX = 50;
		muffin.posY = 50;

		ghost1.posX = 300;
		ghost1.posY = 100;
		ghost2.posX = 350;
		ghost2.posY = 100;
		ghost3.posX = 400;
		ghost3.posY = 100;
		ghost4.posX = 450;
		ghost4.posY = 100;

		ghost1.accelerationX = 0.5;
		ghost1.accelerationY = 0.5;
		ghost1.speed = 12;
		ghost2.accelerationX = 0.5;
		ghost2.accelerationY = 0.5;
		ghost3.accelerationX = 0.5;
		ghost3.accelerationY = 0.5;
		ghost4.accelerationX = 0.5;
		ghost4.accelerationY = 0.5;

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
		if((Surf_Gate = SDL_LoadBMP("gate.bmp")) == NULL) {

			printf("error while loading BMP\n");

		}

		if((Surf_Ghost1 = SDL_LoadBMP("ghost.bmp")) == NULL) {

			printf("error while loading BMP\n");

		}
		Uint32 colorkey2 = SDL_MapRGB( Surf_Ghost1->format, 255, 255, 255 );
		SDL_SetColorKey(Surf_Ghost1, SDL_SRCCOLORKEY, colorkey2);

		if((Surf_Ghost2 = SDL_LoadBMP("ghost.bmp")) == NULL) {

			printf("error while loading BMP\n");

		}
		Uint32 colorkey3 = SDL_MapRGB( Surf_Ghost2->format, 255, 255, 255 );
		SDL_SetColorKey(Surf_Ghost2, SDL_SRCCOLORKEY, colorkey3);

		if((Surf_Ghost3 = SDL_LoadBMP("ghost.bmp")) == NULL) {

			printf("error while loading BMP\n");

		}
		Uint32 colorkey4 = SDL_MapRGB( Surf_Ghost3->format, 255, 255, 255 );
		SDL_SetColorKey(Surf_Ghost3, SDL_SRCCOLORKEY, colorkey4);

		if((Surf_Ghost4 = SDL_LoadBMP("ghost.bmp")) == NULL) {

			printf("error while loading BMP\n");

		}
		Uint32 colorkey5 = SDL_MapRGB( Surf_Ghost4->format, 255, 255, 255 );
		SDL_SetColorKey(Surf_Ghost4, SDL_SRCCOLORKEY, colorkey5);


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
			goToPathMuffin();
			walkinGhostdown();
			finallyReleaseTheGhost(&ghost1);
			initFields();
			doGraphics();

			SDL_Flip(Surf_Display);
		}
	}
