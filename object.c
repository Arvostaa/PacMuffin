#include "object.h"
#include "field.h"
#include <stdlib.h>

#define TILE_SIZE 50

object_s muffin;
object_s ghost1;
object_s ghost2;
object_s ghost3;
object_s ghost4;
field_s map[16][12];

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

int isTileOnTile2(int x1, int y1, int x2, int y2, int size){

	if (y1 == y2){

		if (   (x1 <= x2)  &&  ( abs(x1 - x2) < size/2)   )  {

			return 1;
		}

		else if ( (x2 <= x1) &&  ( abs(x1 - x2) < size/2)  ) {
			return 1;
		}

		return 0;
	}

	else if (x1 == x2){

		if (   (y1 <= y2)  &&  ( abs(y1 - y2) < size/2)   )  {

			return 1;
		}

		else if ( (y2 <= y1) &&  ( abs(y1 - y2) < size/2)  ) {
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

int isObjectOnWall(int wallX, int wallY){

	if((map[wallX][wallY].type == FIELD_WALL)){
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

int isObjectOnGate(int wallX, int wallY){

	if(map[wallX][wallY].type == FIELD_GATE){
		return 1;
	}
	return 0;
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

