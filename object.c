#include "object.h"
#include "field.h"
#include <stdlib.h>
#include "keys.h"

object_s muffin;
object_s ghost1;
object_s ghost2;
object_s ghost3;
object_s ghost4;
field_s map[16][12];

cookie_s *cookie1;

void insertCookie(cookie_s *cookie, int amount){
	cookie_s *wsk, *new;
	wsk = cookie;
	while(wsk->next != NULL){
		wsk = wsk->next;
	}
	new = malloc(sizeof(cookie_s));
	new->cookie_counter = amount;
	new->next = NULL;
	wsk->next = new;
}


// POJEDYŃCZE FUNKCJE-KURDUPLE, KTÓRE ZŁOŻĄ SIĘ NA FUNKCJĘ WIĘKSZĄ
//1 - na przyciski

void accelVertical(object_s *obj, char *accelTab){
	if(accelTab[accS] == 1){
		obj->accelerationX = 0;
		obj->accelerationY = OBJECT_ACC;
	}
	if(accelTab[accW] == 1){
		obj->accelerationX = 0;
		obj->accelerationY = -OBJECT_ACC;
	}
}

void accelHorizontal(object_s *obj, char *accelTab){
	if(accelTab[accD] == 1){
		obj->accelerationX = OBJECT_ACC;
		obj->accelerationY = 0;
	}
	if(accelTab[accA] == 1){
		obj->accelerationX = -OBJECT_ACC;
		obj->accelerationY = 0;
	}
}
///
int isMapWall(int wallX, int wallY){

	if((map[wallX][wallY].type == FIELD_WALL)){
		return 1;
	}
	else return 0;
}
//
/**
 * ************************************************************************************
 */
void ObjectOverlapWallH_R(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if((obj->posX < mapX) && (obj->posY == mapY) && (abs(obj->posX - mapX) > 0.80*size) && (abs(obj->posX - mapX) < size)){
//		printf("WallH_R\n");
	//	printf("OBJ %f - posX, %f - posY, %f - accX, %f - accY\n", muffin.posX, muffin.posY, muffin.accelerationX, muffin.accelerationY );
	//	printf("FIELD: %d - map-i, %d - map-j, %d - map[i][j].x, %d - map[i][j].y,\n", mapX/size, mapY/size, mapX, mapY);

		if(isMapWall(mapX/size, mapY/size)){
			printf("WallH_R\n");
			obj->posX = mapX - size;
			obj->accelerationX = 0;

		}
	}
}

void ObjectOverlapWallH_L(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(obj->posX > mapX && obj->posY == mapY && abs(obj->posX - mapX) > 0.80*size && abs(obj->posX - mapX) < size){
//	 printf("WallH_L\n");
		if(isMapWall(mapX/size, mapY/size)){
			printf("H_Ll\n");
			obj->posX = mapX + size;
			obj->accelerationX = 0;

		}
	}
}
//printf("WallV_D %d -map i , %d - mapj \n", mapX/size, mapY/size);
void ObjectOverlapWallV_D(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(obj->posY < mapY && obj->posX == mapX && abs(obj->posY - mapY) > 0.80*size && abs(obj->posY - mapY) < size){

		if(isMapWall(mapX/size, mapY/size)){
		printf("V_D\n");
			obj->posY = mapY - size;
			obj->accelerationY = 0;
		}
	}
}

void ObjectOverlapWallV_U(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(obj->posY > mapY && obj->posX == mapX && abs(obj->posY - mapY) > 0.80*size && abs(obj->posY - mapY) < size){
	//	printf("WallV_U\n");
		if(isMapWall(mapX/size, mapY/size)){
		printf("WallV_U\n");
			obj->posY = mapY + size;
			obj->accelerationY = 0;

		}
	}
}

void ObjectOverlapWallH1of2R_U_D(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(obj->posX < mapX && abs(obj->posY - mapY) < size/2 && abs(obj->posY - mapY) != 0 && mapX != obj->posX && abs(obj->posX - mapX) > size/2 && abs(obj->posX - mapX) < size ){
		//printf("2rud   %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
		if(isMapWall(mapX/size, mapY/size)){
		printf("H1of2R_U_D\n");
			obj->posX = mapX - size;
		accelVertical(obj, accelTab);
		}

	}
}

void ObjectOverlapWallH1of2L_U_D(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(obj->posX > mapX && abs(obj->posY - mapY) < size/2 && abs(obj->posY - mapY) != 0 && mapX != obj->posX && abs(obj->posX - mapX) > size/2 && abs(obj->posX - mapX < size)){
		//printf("H1of2L_U_D\n");
		if(isMapWall(mapX/size, mapY/size)){
			printf("H1of2L_U_D\n");
			obj->posX = mapX + size;
			accelVertical(obj, accelTab);
		}
	}
}

void ObjectOverlapWallH_Half(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(abs(obj->posX - mapX) > size/2 && abs(obj->posX - mapX) < size && abs(obj->posY - mapY) == size/2){
//		printf("wallH_half   %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
		if(isMapWall(mapX/size, mapY/size)){
//			printf("WALL!_Half\n");
			obj->posX = mapX - size;
		//	accelHorizontal(obj, accelTab);
		}

	}
}


void ObjectOverlapWallV1of2U_L_R(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(abs(obj->posX - mapX) < abs(obj->posY - mapY) && obj->posY > mapY && obj->posX != mapX && abs(obj->posY - mapY) > size/2 && abs(obj->posY - mapY) < size && abs(obj->posX - mapX) < size/2){

		if(isMapWall(mapX/size, mapY/size)){
		//	printf("V-1of2-U   %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
			printf("U-1of2");
			obj->posY = mapY + size;
			obj->accelerationY = 0;
		}
	//	accelHorizontal(obj, accelTab);
	}
}

void ObjectOverlapWallV1of2D_R_L(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(abs(obj->posX - mapX) < abs(obj->posY - mapY) && abs(obj->posY - mapY) > size/2 && abs(obj->posY - mapY) < size && abs(obj->posX - mapX) < size/2){
printf("D-1of2");
		if(isMapWall(mapX/size, mapY/size)){
		//	printf("V-1of2-D   %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
			printf("WallV1of2D_R_L\n");
			obj->posY = mapY ;
		}
		//accelHorizontal(obj, accelTab);
	}
}

void ObjectOverlapWallV_Half(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	if(abs(obj->posY - mapY) > size/2 && abs(obj->posY - mapY) < size && abs(obj->posX - mapX) == size/2){
		printf("WallV_Half\n");
		if(isMapWall(mapX/size, mapY/size)){
			printf("WallV_Half\n");

			obj->posX += 1;

		}
	}
}

void directObjectToPath(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	int i, j;
	for(i = 0; i < MAP_W; i++){
		for(j = 0; j < MAP_H; j++){
			ObjectOverlapWallH_R(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallH_L(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallV_D(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallV_U(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallH1of2R_U_D(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallH_Half(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallV1of2U_L_R(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallV1of2D_R_L(obj, mapX, mapY, size, accelTab);
			ObjectOverlapWallV_Half(obj, mapX, mapY, size, accelTab);
		}
	}
}

//////////////////////////////////////////////DUPA DUPA I TRZY KURY //////////////////////////////////////

void goToPathx(int wallX, int wallY, object_s *obj, char *accelTab){

	if(obj->accelerationX <= 0){
		obj->posX= map[wallX][wallY].x + TILE_SIZE;
		if(accelTab[accS] == 1){
			muffin.accelerationX = 0;
			muffin.accelerationY = OBJECT_ACC;
			//	accelTab[accS] = 0;
		}
		else if(accelTab[accW] == 1){
			muffin.accelerationX = 0;
			muffin.accelerationY = -OBJECT_ACC;
			//		accelTab[accW] = 0;
		}

	}

	else if(obj->accelerationX >= 0){
		obj->posX = map[wallX][wallY].x - TILE_SIZE;
		if(accelTab[accS] == 1){
			muffin.accelerationX = 0;
			muffin.accelerationY = OBJECT_ACC;
			//	accelTab[accS] = 0;
		}
		else if(accelTab[accW] == 1){
			muffin.accelerationX = 0;
			muffin.accelerationY = -OBJECT_ACC;
			//	accelTab[accW] = 0;
		}
	}
}

void goToPathy(int wallX, int wallY, object_s *obj,  char *accelTab){
	if(obj->accelerationY <= 0){
		obj->posY = map[wallX][wallY].y + TILE_SIZE;
		if(accelTab[accD] == 1){
			muffin.accelerationX = OBJECT_ACC;
			muffin.accelerationY = 0;
			//	accelTab[accD] = 0;
		}
		else if(accelTab[accA] == 1){
			muffin.accelerationX = -OBJECT_ACC;
			muffin.accelerationY = 0;
			//	accelTab[accA] = 0;
		}

	}

	else if(obj->accelerationY >= 0){
		obj->posY = map[wallX][wallY].y - TILE_SIZE;
		if(accelTab[accD] == 1){
			muffin.accelerationX = OBJECT_ACC;
			muffin.accelerationY = 0;
			//	accelTab[accD] = 0;
		}
		else if(accelTab[accA] == 1){
			muffin.accelerationX = -OBJECT_ACC;
			muffin.accelerationY = 0;
			//	accelTab[accA] = 0;
		}
	}
}




int isObjectOnTile (int objX, int objY, int tileX, int tileY){

	if((objX == tileX*TILE_SIZE) && (objY == tileY*TILE_SIZE)){

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

void changeReleaseCounter( object_s *ghost1, object_s *ghost2){
	if(ghost1->release_counter%11 == 0){
		ghost1->release_counter = 1;
		ghost2->release_counter = 1;
	}
}

int shouldReleaseGhost(object_s *ghost){
	if(ghost->release_moment == 1){
		return 1;
	}
	else return 0;
}

int shouldGhostWalkOnPath(object_s *ghost){
	if(ghost->release_moment == 0){
		return 1;
	}
	else return 0;
}

void ghostToPathx(int wallX, int wallY, object_s *ghost){
	if(ghost->accelerationX < 0){
		ghost->posX = map[wallX][wallY].y + TILE_SIZE;
		ghost->accelerationX = 0;
		ghost->accelerationY = 0.5;
	}
	else if(ghost->accelerationX > 0){
		ghost->posY = map[wallX][wallY].y - TILE_SIZE;
		ghost->accelerationX = 0;
		ghost->accelerationY = -0.5;
	}
}

void ghostToPathy(int wallX, int wallY, object_s *ghost){
	if(ghost->accelerationY < 0){
		ghost->posY = map[wallX][wallY].y + TILE_SIZE;
		ghost->accelerationY = 0;
		ghost->accelerationX = 0.5;
	}
	else if(ghost->accelerationY > 0){
		ghost->posY = map[wallX][wallY].y - TILE_SIZE;
		ghost->accelerationY = 0;
		ghost->accelerationX = -0.5;
	}
}



int isObjectOnPath(int wallX, int wallY){

	if(map[wallX][wallY].type == FIELD_PATH){
		return 1;
	}
	else return 0;
}

int isObjectOnGate(int wallX, int wallY){

	if(map[wallX][wallY].type == FIELD_GATE){
		return 1;
	}
	return 0;
}


