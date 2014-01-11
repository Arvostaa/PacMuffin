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
#include "keys.h"

int release_pos = 999;
int acceleration_draw = 0;
int cookie_amount = 1;

float newPosX;
float newPosY;

#define SCREEN_W 800
#define SCREEN_H 600

object_s muffin;
object_s ghost1;
object_s ghost2;
object_s ghost3;
object_s ghost4;
field_s map[16][12];

cookie_s *cookie1;

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
SDL_Surface* Surf_cookie1;

char KeysOn[4] = {0,0,0,0};
char accel[5] = {0, 0, 0, 0, 0};

char running = 1;


void OnKeyDowndd(SDLKey sym) {

	switch(sym) {

	case (SDLK_d): {

		KeysOn[0]=1;
		accel[accD] = 1;
		accel[accA] = 0;
		muffin.accelerationX = OBJECT_ACC;
		//	muffin.accelerationY = 0;
		break;

	}
	case (SDLK_a): {
		KeysOn[1] = 1;
		accel[accA] = 1;
		accel[accD] = 0;
		muffin.accelerationX = -OBJECT_ACC;
		//	muffin.accelerationY = 0;
		break;
	}
	case (SDLK_w): {
		KeysOn[2]= 1;
		accel[accW] = 1;
		accel[accS] = 0;
		muffin.accelerationY = -OBJECT_ACC;
		//	muffin.accelerationX = 0;
		break;
	}
	case (SDLK_s): {
		KeysOn[3] = 1;
		accel[accS] = 1;
		accel[accW] = 0;
		muffin.accelerationY = OBJECT_ACC;
		//	muffin.accelerationX = 0;
		break;
	}
	default: {}
	}
}

void OnKeyUp(SDLKey sym) {
	switch(sym) {

	case (SDLK_d): {
		KeysOn[0]=0;

		break;

	}
	case (SDLK_a): {
		KeysOn[1] = 0;
		break;
	}
	case (SDLK_w): {
		KeysOn[2]= 0;
		break;
	}
	case (SDLK_s): {
		KeysOn[3] = 0;
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
		OnKeyUp(Event->key.keysym.sym);
		break;
	}
	case(SDL_MOUSEBUTTONDOWN): {
		break;
	}
	}
}

void printObjectProp(object_s *object){
	printf("OBJ %f - posX, %f - posY, %f - accX, %f - accY\n", object->posX, object->posY, object->accelerationX, object->accelerationY );
}

void printFieldProp(int i, int j){
	printf("FIELD: %d - map-i, %d - map-j, %d - map[i][j].x, %d - map[i][j].y, %d - fieldType\n", i, j, map[i][j].x, map[i][j].y, map[i][j].type);
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

void accelerationDraw(object_s *ghost){
	if(acceleration_draw%2 == 0){
		ghost->accelerationX = 0,25;
	}
	else ghost->accelerationX = -OBJECT_ACC;
}
/*
void goToPathMuffin(){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){

			if (isTileOnTile(muffin.posX, muffin.posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if (isMapWall(i,j)){
					goToPathx(i, j, &muffin, accel);
					goToPathy(i, j, &muffin, accel);
				}
			}
		}
	}
}
*/
void goToPathMuffin2(){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			ObjectOverlapWallH_R(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallH_L(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallV_D(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallV_U(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallH1of2R_U_D(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallH1of2L_U_D(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallV1of2U_L_R(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
			ObjectOverlapWallV1of2D_R_L(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);
		//printf("V-1of2-U   %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);	ObjectOverlapWallV1of2D_R_L(&muffin, map[i][j].x, map[i][j].y, TILE_SIZE, accel);


		}
	}
}


int isGhostInGate(int wallX, int wallY){

	if(map[wallX][wallY].y == 3*TILE_SIZE){
		return 1;
	}
	else return 0;
}

void moveGhostInCage( object_s *ghost){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if(ghost->release_counter%11 == 0){
				release_pos = 3*TILE_SIZE;
				ghost->release_counter = 1;
			}
			if (isTileOnTile(ghost->posX, ghost->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if(isObjectOnPath(i, j)){
					goToPathy(i, j, ghost, accel);
					ghost->accelerationY = -(ghost->accelerationY);
					ghost->release_counter++;
				}
			}
		}
	}
}

void finallyReleaseTheGhost(object_s *ghost1){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile(ghost1->posX, ghost1->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if(isGhostInGate(i,j)){
					if(release_pos == 3*TILE_SIZE){
						release_pos = 999;
						acceleration_draw++;
						accelerationDraw(ghost1);     //losowany accX
						ghost1->posY = j*TILE_SIZE;
						ghost1->accelerationY = 0;
						ghost1->release_moment = 0;
						break;
					}
				}
			}
		}
	}
}

void moveGhostOutOfCage(object_s *ghost){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile( ghost->posX, ghost->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				//		printf(" %d - i, %d - j,%d - map[i][j].x, %d - map[i][j].y,  %f  - ghostX, %f - ghostY\n ",i,j,map[i][j].x, map[i][j].y, ghost->posX, ghost->posY);
				if(isObjectOnGate(i,j)){
					ghost->posX = map[i][j].x;
					//ghostToPathy(i,j, ghost);
					ghost->accelerationX = 0;
					ghost->accelerationY = 0,25;
					ghost->on_path_moment = 1;
					ghost->release_moment = 1;
					break;
				}
			}
		}
	}
}

void GhostsLogic(){

	moveGhostInCage(&ghost1);
	finallyReleaseTheGhost(&ghost1);
	if(ghost1.release_moment == 0){
		moveGhostOutOfCage(&ghost1);
	}
	moveGhostInCage(&ghost2);
	if(shouldReleaseGhost(&ghost2)){
		finallyReleaseTheGhost(&ghost2);
	}
	if(ghost1.release_moment == 1){
		moveGhostOutOfCage(&ghost2);
	}

	moveGhostInCage(&ghost3);
	if(shouldReleaseGhost(&ghost3)){
		finallyReleaseTheGhost(&ghost3);
	}
	moveGhostInCage(&ghost4);
	if(shouldReleaseGhost(&ghost4)){
		finallyReleaseTheGhost(&ghost4);
	}
	//printf("%d  %d  %d  %d\n", ghost1.release_counter, ghost2.release_counter, ghost3.release_counter, ghost4.release_counter);
	ghost1.posY += ghost1.speed*ghost1.accelerationY;
	ghost2.posY += ghost1.speed*ghost2.accelerationY;
	ghost3.posY += ghost1.speed*ghost3.accelerationY;
	ghost4.posY += ghost1.speed*ghost4.accelerationY;

	ghost1.posX += ghost1.speed*ghost1.accelerationX;
	ghost2.posX += ghost1.speed*ghost2.accelerationX;
	ghost3.posX += ghost1.speed*ghost3.accelerationX;
	ghost4.posX += ghost1.speed*ghost4.accelerationX;
}
/*
void ChangeDirection(object_s *object){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if((object->accelerationX == 0) && (object->accelerationY > 0)){
				if(isTileOnTile(object->posX, object->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){

					if(abs(object->posY - map[i][j].y + TILE_SIZE < 2*TILE_SIZE)){
						printf("mniejszy niz 2tilesize\n");
						if(KeysOn[leftA] == 1 || KeysOn[rightD] == 1) {
							printf("KeyD/A is on!\n");
							{
								if(newPosY > map[i][j].y){

									object->posY = map[i][j].y;
									object->posX += object->accelerationY*object->speed;
								}
							}
						}

					}
				}
			}

		}
	}
}
*/
void doLogic() {
/*
	newPosX = muffin.posX + muffin.speed*muffin.accelerationX;
	newPosY = muffin.posY + muffin.speed*muffin.accelerationY;

	//ChangeDirection(&muffin);

	muffin.posX += muffin.speed*muffin.accelerationX;
*/


	/*
	 * if(GRANICA_PRZEKROCZONA){
	 * if(PRZYCISK_HORYZONTALNY){
	 * if(MOZLIWE PRZEJSCIE HORYZONTALNE){
	 * wyrownaj Y do srodka;
	 * pozostala wartosc do X;
	 * }
	 * }
	 * }
	 *
	 * OBLICZENIA GRANICY WERTYKALNEJ
	 * GRANICA PRZEKROCZONA?
	 * TAK: {SPRAWDZ PRZYCISK HORYZONTALNY
	 * NIE: muffin.posy += muffin.speed*muffin.accelerationY;
	 */

	if((muffin.posX < 0))
	{
		muffin.posX = 0;
	}
	else if ( muffin.posX > (SCREEN_W - muffin.size))
	{
		muffin.posX = SCREEN_W - muffin.size;
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
	goToPathMuffin2();
	//goToPathMuffin2y();

	//goToPathMuffin2x();

	//printf("%f - accX, %f -accY, %d - accD, %d -accA, %d - accW, %d - accS\n", muffin.accelerationX, muffin.accelerationY, accel[accD], accel[accA], accel[accW], accel[accS]);
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
		map[1][k].y = k*TILE_SIZE;
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
	map[4][9].y = 9*TILE_SIZE;

	map[5][3].type = FIELD_GATE;
	map[5][3].x = 5*TILE_SIZE;
	map[5][3].y = 3*TILE_SIZE;

	map[10][3].type = FIELD_GATE;
	map[10][3].x = 10*TILE_SIZE;
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
	displaySurface(Surf_Display, Surf_cookie1, cookie1->posX, cookie1->posY);

}

int main(void) {

	muffin.size = 50;
	muffin.speed = 2;
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

	ghost1.accelerationX = 0;
	ghost1.accelerationY = OBJECT_ACC;
	ghost1.speed = 4;
	ghost2.accelerationX = 0;
	ghost2.accelerationY = OBJECT_ACC;
	ghost3.accelerationX = 0;
	ghost3.accelerationY = OBJECT_ACC;
	ghost4.accelerationX = 0;
	ghost4.accelerationY = OBJECT_ACC;

	ghost1.release_counter = 1;
	ghost2.release_counter = 1;
	ghost3.release_counter = 1;
	ghost4.release_counter = 1;

	ghost1.release_moment = 1;
	ghost2.release_moment = 1;
	ghost3.release_moment = 1;
	ghost4.release_moment = 1;

	cookie1->posX = 100;
	cookie1->posY = 400;
	initFields();

	cookie1 = malloc(sizeof(cookie_s));

	cookie1->next = NULL;
	cookie1->cookie_counter = cookie_amount;


	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}
	SDL_EnableKeyRepeat(20,1);

	if((Surf_Display = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return 0;
	}

	if((Surf_Pig = SDL_LoadBMP("images/piggy.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
//	Uint32 colorkey = SDL_MapRGB( Surf_Pig->format, 0, 0, 0 );
//	SDL_SetColorKey(Surf_Pig, SDL_SRCCOLORKEY, colorkey);



	if((Surf_Wall = SDL_LoadBMP("images/wall.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}
	if((Surf_Gate = SDL_LoadBMP("images/gate.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}

	if((Surf_Ghost1 = SDL_LoadBMP("images/ghost.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey2 = SDL_MapRGB( Surf_Ghost1->format, 255, 255, 255 );
	SDL_SetColorKey(Surf_Ghost1, SDL_SRCCOLORKEY, colorkey2);

	if((Surf_Ghost2 = SDL_LoadBMP("images/ghost.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey3 = SDL_MapRGB( Surf_Ghost2->format, 255, 255, 255 );
	SDL_SetColorKey(Surf_Ghost2, SDL_SRCCOLORKEY, colorkey3);

	if((Surf_Ghost3 = SDL_LoadBMP("images/ghost.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey4 = SDL_MapRGB( Surf_Ghost3->format, 255, 255, 255 );
	SDL_SetColorKey(Surf_Ghost3, SDL_SRCCOLORKEY, colorkey4);

	if((Surf_Ghost4 = SDL_LoadBMP("images/ghost.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey5 = SDL_MapRGB( Surf_Ghost4->format, 255, 255, 255 );
	SDL_SetColorKey(Surf_Ghost4, SDL_SRCCOLORKEY, colorkey5);


	if((Surf_Path = SDL_LoadBMP("images/path.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}

	if((Surf_Highlight = SDL_LoadBMP("images/highlight.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}

	if((Surf_cookie1 = SDL_LoadBMP("images/cookie.bmp")) == NULL) {

				printf("error while loading BMP\n");

			}
		Uint32 colorkey9 = SDL_MapRGB( Surf_cookie1->format, 255, 255, 255 ); ///sth wrong :<
		SDL_SetColorKey(Surf_cookie1, SDL_SRCCOLORKEY, colorkey9);

	SDL_Event Event;

	while(running) {

		while(SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}

		SDL_FillRect(Surf_Display, NULL, 12852252);

		doLogic();
		GhostsLogic();
		doGraphics();
		//	printObjectProp(&muffin);
		//	printFieldProp(5,3);
		//	printFieldProp(4,3);
		//	printFieldProp(1,3);
		//	printer();
		SDL_Flip(Surf_Display);
	}
}
