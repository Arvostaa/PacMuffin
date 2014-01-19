/*
============================================================================
Name : PacMUFFIN
Author :AnnSko
Version :0.1
============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "field.h"
#include "object.h"
#include "keys.h"

#include <time.h>

int release_pos = 999;
int acceleration_draw = 0;
int cookie_amount = 1;

int cookie_delay = 1;
float newPosX;
float newPosY;

#define SCREEN_W 800
#define SCREEN_H 600

object_s muffin;
object_s fork1;
object_s fork2;
object_s fork3;
object_s fork4;
field_s map[16][12];

cookie_s *cookieList;

SDL_Surface* Surf_Display;
SDL_Surface* Surf_Muffin;
SDL_Surface* Surf_DeadMuffin;
SDL_Surface* Surf_Path;
SDL_Surface* Surf_Wall;
SDL_Surface* Surf_Gate;
SDL_Surface* Surf_Highlight;
SDL_Surface* Surf_fork;
//SDL_Surface* Surf_fork2;
//SDL_Surface* Surf_fork3;
//SDL_Surface* Surf_fork4; //zmien jak dla walla

SDL_Surface* Surf_cookieList;

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
		//        muffin.accelerationY = 0;
		break;

	}
	case (SDLK_a): {
		KeysOn[1] = 1;
		accel[accA] = 1;
		accel[accD] = 0;

		muffin.accelerationX = -OBJECT_ACC;
		//        muffin.accelerationY = 0;
		break;
	}
	case (SDLK_w): {
		KeysOn[2]= 1;

		accel[accW] = 1;
		accel[accS] = 0;
		muffin.accelerationY = -OBJECT_ACC;
		//        muffin.accelerationX = 0;
		break;
	}
	case (SDLK_s): {
		KeysOn[3] = 1;

		accel[accS] = 1;
		accel[accW] = 0;
		muffin.accelerationY = OBJECT_ACC;
		//        muffin.accelerationX = 0;
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

void accelerationDraw(object_s *fork){
	if(acceleration_draw%2 == 0){
		fork->accelerationX = 0,25;
	}
	else fork->accelerationX = -OBJECT_ACC;
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


			changeKeys(&muffin, accel);

		}
	}
}

int isforkInGate(int wallX, int wallY){

	if(map[wallX][wallY].y == 3*TILE_SIZE){
		return 1;
	}
	else return 0;
}

void moveforkInCage( object_s *fork){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if(fork->release_counter%11 == 0){
				release_pos = 3*TILE_SIZE;
				fork->release_counter = 1;
			}
			if (isTileOnTile(fork->posX, fork->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if(isObjectOnPath(i, j)){
					goToPathy(i, j, fork, accel);
					fork->accelerationY = -(fork->accelerationY);
					fork->release_counter++;
				}
			}
		}
	}
}

void finallyReleaseThefork(object_s *fork1){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile(fork1->posX, fork1->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				if(isforkInGate(i,j)){
					if(release_pos == 3*TILE_SIZE){
						release_pos = 999;
						acceleration_draw++;
						accelerationDraw(fork1); //losowany accX
						fork1->posY = j*TILE_SIZE;
						fork1->accelerationY = 0;
						fork1->release_moment = 0;
						break;
					}
				}
			}
		}
	}
}

void moveforkOutOfCage(object_s *fork){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			if (isTileOnTile( fork->posX, fork->posY, map[i][j].x, map[i][j].y, TILE_SIZE)){
				//                printf(" %d - i, %d - j,%d - map[i][j].x, %d - map[i][j].y, %f - forkX, %f - forkY\n ",i,j,map[i][j].x, map[i][j].y, fork->posX, fork->posY);
				if(isObjectOnGate(i,j)){
					fork->posX = map[i][j].x;
					//forkToPathy(i,j, fork);
					fork->accelerationX = 0;
					fork->accelerationY = 0,25;
					fork->on_path_moment = 1;
					fork->release_moment = 1;
					break;
				}
			}
		}
	}
}

void forksLogic(){

	moveforkInCage(&fork1);
	finallyReleaseThefork(&fork1);
	if(fork1.release_moment == 0){
		moveforkOutOfCage(&fork1);
	}
	moveforkInCage(&fork2);
	if(shouldReleasefork(&fork2)){
		finallyReleaseThefork(&fork2);
	}
	if(fork1.release_moment == 1){
		moveforkOutOfCage(&fork2);
	}

	moveforkInCage(&fork3);
	if(shouldReleasefork(&fork3)){
		finallyReleaseThefork(&fork3);
	}
	moveforkInCage(&fork4);
	if(shouldReleasefork(&fork4)){
		finallyReleaseThefork(&fork4);
	}
	//printf("%d %d %d %d\n", fork1.release_counter, fork2.release_counter, fork3.release_counter, fork4.release_counter);
	fork1.posY += fork1.speed*fork1.accelerationY;
	fork2.posY += fork1.speed*fork2.accelerationY;
	fork3.posY += fork1.speed*fork3.accelerationY;
	fork4.posY += fork1.speed*fork4.accelerationY;

	fork1.posX += fork1.speed*fork1.accelerationX;
	fork2.posX += fork1.speed*fork2.accelerationX;
	fork3.posX += fork1.speed*fork3.accelerationX;
	fork4.posX += fork1.speed*fork4.accelerationX;
}

void doLogic() {
	/*
        newPosX = muffin.posX + muffin.speed*muffin.accelerationX;
        newPosY = muffin.posY + muffin.speed*muffin.accelerationY;


	 */
	muffin.posX += muffin.speed*muffin.accelerationX;

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

	printf("%f - accX, %f -accY, %d - accD, %d -accA, %d - accW, %d - accS\n", muffin.accelerationX, muffin.accelerationY, accel[accD], accel[accA], accel[accW], accel[accS]);
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

void initForks(object_s *fork){


}
void displaySurfaceCookies(cookie_s *cookie){
	cookie_s *wsk = cookie;
	while(wsk!= NULL){
		displaySurface(Surf_Display, Surf_cookieList, wsk->posX, wsk->posY);
		wsk = wsk->next;
	}
}

void displaySurfaceMuffin(object_s *muffin, object_s *fork1, object_s *fork2, object_s *fork3, object_s *fork4){

	if(IsMuffinOnFork(muffin, fork1, TILE_SIZE) ||IsMuffinOnFork(muffin, fork2, TILE_SIZE)){
		displaySurface(Surf_Display, Surf_DeadMuffin, muffin->posX, muffin->posY);

	}
	else if(IsMuffinOnFork(muffin, fork3, TILE_SIZE) ||IsMuffinOnFork(muffin, fork4, TILE_SIZE)){
		displaySurface(Surf_Display, Surf_DeadMuffin, muffin->posX, muffin->posY);

	}


	else displaySurface(Surf_Display, Surf_Muffin, muffin->posX, muffin->posY);

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


	displaySurfaceMuffin(&muffin, &fork1, &fork2, &fork3, &fork4);
	displaySurface(Surf_Display, Surf_fork, fork1.posX, fork1.posY);
	displaySurface(Surf_Display, Surf_fork, fork2.posX, fork2.posY);
	displaySurface(Surf_Display, Surf_fork, fork3.posX, fork3.posY);
	displaySurface(Surf_Display, Surf_fork, fork4.posX, fork4.posY);
	displaySurfaceCookies(cookieList);

}



int main(void) {

	muffin.size = 50;
	muffin.speed = 6;
	muffin.posX = 50;
	muffin.posY = 50;

	fork1.posX = 300;
	fork1.posY = 100;
	fork2.posX = 350;
	fork2.posY = 100;
	fork3.posX = 400;
	fork3.posY = 100;
	fork4.posX = 450;
	fork4.posY = 100;

	fork1.accelerationX = 0;
	fork1.accelerationY = OBJECT_ACC;
	fork1.speed = 4;
	fork2.accelerationX = 0;
	fork2.accelerationY = OBJECT_ACC;
	fork3.accelerationX = 0;
	fork3.accelerationY = OBJECT_ACC;
	fork4.accelerationX = 0;
	fork4.accelerationY = OBJECT_ACC;

	fork1.release_counter = 1;
	fork2.release_counter = 1;
	fork3.release_counter = 1;
	fork4.release_counter = 1;

	fork1.release_moment = 1;
	fork2.release_moment = 1;
	fork3.release_moment = 1;
	fork4.release_moment = 1;

	initFields();

	cookieList = malloc(sizeof(cookie_s));

	cookieList->next = NULL;
	cookieList->posX = 100;
	cookieList->posY = 500;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}
	SDL_EnableKeyRepeat(20,1);

	if((Surf_Display = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return 0;
	}


	if(IsMuffinOnFork(&muffin, &fork1, TILE_SIZE)){
		printf("vsdaGhfds\n\n\n");
		if((Surf_Muffin = SDL_LoadBMP("images/dead_piggy.bmp")) == NULL) {

			printf("error while loading BMP\n");

		}

	}


	if((Surf_Muffin = SDL_LoadBMP("images/dead_piggy.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}




	if((Surf_Muffin = SDL_LoadBMP("images/piggy.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey = SDL_MapRGB( Surf_Muffin->format, 0, 0, 0 );
	SDL_SetColorKey(Surf_Muffin, SDL_SRCCOLORKEY, colorkey);



	if((Surf_Wall = SDL_LoadBMP("images/wall.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}
	if((Surf_Gate = SDL_LoadBMP("images/gate.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}

	if((Surf_fork = SDL_LoadBMP("images/fork.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey2 = SDL_MapRGB( Surf_fork->format, 255, 255, 255 );
	SDL_SetColorKey(Surf_fork, SDL_SRCCOLORKEY, colorkey2);

	/* if((Surf_fork2 = SDL_LoadBMP("images/fork.bmp")) == NULL) {

                printf("error while loading BMP\n");

        }
        Uint32 colorkey3 = SDL_MapRGB( Surf_fork2->format, 255, 255, 255 );
        SDL_SetColorKey(Surf_fork2, SDL_SRCCOLORKEY, colorkey3);

        if((Surf_fork3 = SDL_LoadBMP("images/fork.bmp")) == NULL) {

                printf("error while loading BMP\n");

        }
        Uint32 colorkey4 = SDL_MapRGB( Surf_fork3->format, 255, 255, 255 );
        SDL_SetColorKey(Surf_fork3, SDL_SRCCOLORKEY, colorkey4);

        if((Surf_fork4 = SDL_LoadBMP("images/fork.bmp")) == NULL) {

                printf("error while loading BMP\n");

        }
        Uint32 colorkey5 = SDL_MapRGB( Surf_fork4->format, 255, 255, 255 );
        SDL_SetColorKey(Surf_fork4, SDL_SRCCOLORKEY, colorkey5);

	 */
	if((Surf_Path = SDL_LoadBMP("images/path.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}

	if((Surf_Highlight = SDL_LoadBMP("images/highlight.bmp")) == NULL) {
		printf("error while loading BMP\n");
	}

	if((Surf_cookieList = SDL_LoadBMP("images/cookie.bmp")) == NULL) {

		printf("error while loading BMP\n");

	}
	Uint32 colorkey9 = SDL_MapRGB( Surf_cookieList->format, 255, 255, 255 ); ///sth wrong :<
	SDL_SetColorKey(Surf_cookieList, SDL_SRCCOLORKEY, colorkey9);

	SDL_Event Event;

	while(running) {

		while(SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}

		SDL_FillRect(Surf_Display, NULL, 12852252);

		doLogic();
		forksLogic();
		doGraphics();

		if(IsMuffinOnFork(&muffin, &fork1, TILE_SIZE)){
			muffin.accelerationX = 0;
			muffin.accelerationY = 0;
		}

		if(cookie_amount <= 4){
			deleteCookie(cookieList, &muffin, TILE_SIZE);
			deleteFirstCookie(&cookieList, &muffin, TILE_SIZE);
			addCookies(cookieList, &cookie_delay, cookie_amount);
			cookie_delay++;

		}
		//r        printf("%d\n", cookie_delay);
		//drawCookiePosition(0,0);
		//        printObjectProp(&m;uffin);
		//        printFieldProp(5,3);
		//        printFieldProp(4,3);
		//        printFieldProp(1,3);
		//        printer();
		SDL_Flip(Surf_Display);
	}
}
