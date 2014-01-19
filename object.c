#include "object.h"
#include "field.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "keys.h"

object_s muffin;
object_s fork1;
object_s fork2;
object_s fork3;
object_s fork4;
field_s map[16][12];

cookie_s *cookieList;


int isMapWall(int wallX, int wallY){

	if((map[wallX][wallY].type == FIELD_WALL)){
		return 1;
	}
	else return 0;
}

int isMapPath(int wallX, int wallY){

	if((map[wallX][wallY].type == FIELD_PATH)){
		return 1;
	}
	else return 0;
}

int drawCookieX(int amount){

	srand(time(NULL));

	float posX = rand()%15;

	/*if(map[cookiePosX][cookiePosY].type == FIELD_WALL){
                cookiePosX = (rand%15);
                cookiePosY = (rand%11);
        }
        else{
                cookiePosX = cookiePosX*TILE_SIZE;
                cookiePosY = cookiePosY*TILE_SIZE;
        }*/
	return posX;
}

/* - 4 funkcje: 2 wertykalnie, 2 horyzontalnie
 */

int isMuffinOnCookieH_R(object_s *muffin, float *cookieX, float *cookieY, int size){
	if(muffin->posX < *cookieX && muffin->posY == *cookieY && abs(muffin->posX - *cookieX) > 0.70*size && abs(muffin->posX - *cookieX) < size){
		return 1;
	}
	else return 0;
}

int isMuffinOnCookieH_L(object_s *muffin, float *cookieX, float *cookieY, int size){
	if(muffin->posX > *cookieX && muffin->posY == *cookieY && abs(muffin->posX - *cookieX) > 0.70*size && abs(muffin->posX - *cookieX) < size){
		return 1;
	}
	else return 0;
}


int isMuffinOnCookieV_D(object_s *muffin, float *cookieX, float *cookieY, int size){
	if(muffin->posY < *cookieY && muffin->posX == *cookieX && abs(muffin->posY - *cookieY) > 0.70*size && abs(muffin->posY - *cookieY) < size){
		return 1;
	}
	else return 0;
}

int isMuffinOnCookieV_U(object_s *muffin, float *cookieX, float *cookieY, int size){
	if(muffin->posY > *cookieY && muffin->posX == *cookieX && abs(muffin->posY - *cookieY) > 0.70*size && abs(muffin->posY - *cookieY) < size){
		return 1;
	}
	else return 0;
}

int drawCookieY(int amount){
	srand(time(NULL));

	float posY = rand()%15;
	return posY;
}
void drawCookiePosition(float *cookieX, float *cookieY){
	srand(time(NULL));
	int cookieXint = rand()%11;
	int cookieYint = rand()%15;

	if(map[cookieXint][cookieYint].type == FIELD_WALL){
		while(map[cookieXint][cookieYint].type == FIELD_WALL){
			cookieXint = rand()%11;
			cookieYint = rand()%15;
		}
	}

	*cookieX = cookieXint*TILE_SIZE;
	*cookieY = cookieYint*TILE_SIZE;
	//printf("%d - cookieX, %d - cookieY, %d - fieldType\n", cookieX, cookieY, map[cookieX/TILE_SIZE][cookieY/TILE_SIZE].type);
}

void setCookiePosition(cookie_s *cookieList, float *cookiePosX, float *cookiePosY){ // once i a while it doesn't appear !
	cookie_s *wsk;
	wsk = cookieList;

	drawCookiePosition(&(*cookiePosX), &(*cookiePosY));
	while(wsk->next != NULL){
		if(wsk->posX == *cookiePosX && wsk->posY == *cookiePosY){
			drawCookiePosition(&(*cookiePosX), &(*cookiePosY));
		}
		wsk = wsk->next;
	}

}

void insertCookie(cookie_s *cookieList, int cookie_amount){

	cookie_s *wsk, *new;
	wsk = cookieList;
	while(wsk->next != NULL){
		wsk = wsk->next;
	}

	new = malloc(sizeof(cookie_s));

	new->next = NULL;
	wsk->next = new;
	cookie_amount++;

	//drawCookiePosition(&(new->posX), &(new->posY));
	setCookiePosition(cookieList,&(new->posX), &(new->posY));


}

int isMuffinOnCookie(object_s *muffin, float *cookieX, float *cookieY, int size){

	if(isMuffinOnCookieH_R(muffin, cookieX, cookieY, size)){
		return 1;
	}
	if(isMuffinOnCookieH_L(muffin, cookieX, cookieY, size)){
		return 1;
	}
	if(isMuffinOnCookieV_D(muffin, cookieX, cookieY, size)){
		return 1;
	}
	if(isMuffinOnCookieV_U(muffin, cookieX, cookieY, size)){
		return 1;
	}
	else return 0;
}

int IsMuffinOnFork(object_s *muffin, object_s *fork, int size){

	if(isMuffinOnCookieH_R(muffin, &(fork->posX), &(fork->posY), size)){
		return 1;
	}
	if(isMuffinOnCookieH_L(muffin, &(fork->posX), &(fork->posY), size)){
		return 1;
	}
	if(isMuffinOnCookieV_D(muffin, &(fork->posX), &(fork->posY), size)){
		return 1;
	}
	if(isMuffinOnCookieV_U(muffin, &(fork->posX), &(fork->posY), size)){
		return 1;
	}
	else return 0;

}

void deleteCookie(cookie_s *cookieList,object_s *muffin, int size){

	cookie_s *wsk = cookieList;
	while(wsk->next != NULL){
		if(isMuffinOnCookie(muffin, &(wsk->next->posX), &(wsk->next->posY), size)){
			cookie_s *deleted = wsk->next;
			wsk->next = deleted->next;
			free(deleted);
		}
		else{
			wsk = wsk->next;
		}
	}
}

void deleteFirstCookie(cookie_s **cookieList, object_s *muffin, int size){
	cookie_s *deleteFirst = *cookieList;
	if(deleteFirst != NULL){
		if(isMuffinOnCookie(muffin, &(deleteFirst->posX), &(deleteFirst->posY), size)){
			*cookieList = deleteFirst->next;
			free(deleteFirst);

		}
	}
}
/*
void deleteCookie2(cookie_s *cookieList, object_s *muffin, int size){
        cookie_s *wsk = cookieList;
        while(wsk->next != NULL){
                if(){
                        deleteCookie(cookieList, muffin, &(wsk->posX), &(wsk->posY), size);
                }
        }
}
 */
 void addCookies(cookie_s *cookieList,int *cookie_delay, int cookie_amount)
 {
	 if(*cookie_delay%400 == 0){
		 insertCookie(cookieList, cookie_amount);
		 cookie_delay = 0;
	 }


	 cookie_amount++;

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
 void changeKeys(object_s *obj, char *accelTab){

	 if((accelTab[accD] == 1) && (obj->accelerationX > 0) && obj->accelerationY == 0){
		 accelTab[accA] = 0;
		 accelTab[accW] = 0;
		 accelTab[accS] = 0;
	 }

	 else if(accelTab[accA] == 1 && obj->accelerationX < 0 && obj->accelerationY == 0){
		 accelTab[accD] = 0;
		 accelTab[accW] = 0;
		 accelTab[accS] = 0;
	 }
	 else if(accelTab[accW] == 1 && obj->accelerationY < 0 && obj->accelerationX == 0){
		 accelTab[accA] = 0;
		 accelTab[accS] = 0;
		 accelTab[accD] = 0;
	 }

	 else if(accelTab[accS] == 1 && obj->accelerationY > 0 && obj->accelerationX == 0){
		 accelTab[accA] = 0;
		 accelTab[accW] = 0;
		 accelTab[accD] = 0;
	 }
 }

 //
 /**
  * ************************************************************************************
  */
  void ObjectOverlapWallH_R(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	 if((obj->posX < mapX) && (obj->posY == mapY) && (abs(obj->posX - mapX) > 0.80*size) && (abs(obj->posX - mapX) < size)){
		 //                printf("WallH_R\n");
		 //        printf("OBJ %f - posX, %f - posY, %f - accX, %f - accY\n", muffin.posX, muffin.posY, muffin.accelerationX, muffin.accelerationY );
		 //        printf("FIELD: %d - map-i, %d - map-j, %d - map[i][j].x, %d - map[i][j].y,\n", mapX/size, mapY/size, mapX, mapY);

		 if(isMapWall(mapX/size, mapY/size)){
			 //printf("WallH_R\n");
			 obj->posX = mapX - size;
			 obj->accelerationX = 0;
			 //accelVertical(obj, accelTab);

		 }
	 }
  }

  void ObjectOverlapWallH_L(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(obj->posX > mapX && obj->posY == mapY && abs(obj->posX - mapX) > 0.80*size && abs(obj->posX - mapX) < size){
		  //         printf("WallH_L\n");
		  if(isMapWall(mapX/size, mapY/size)){
			  //printf("H_Ll\n");
			  obj->posX = mapX + size;
			  obj->accelerationX = 0;
			  //accelVertical(obj, accelTab);
		  }
	  }
  }
  //printf("WallV_D %d -map i , %d - mapj \n", mapX/size, mapY/size);
  void ObjectOverlapWallV_D(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(obj->posY < mapY && obj->posX == mapX && abs(obj->posY - mapY) > 0.80*size && abs(obj->posY - mapY) < size){

		  if(isMapWall(mapX/size, mapY/size)){
			  //        printf("V_D\n");
			  obj->posY = mapY - size;
			  obj->accelerationY = 0;
			  //accelHorizontal(obj, accelTab);
		  }
	  }
  }

  void ObjectOverlapWallV_U(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(obj->posY > mapY && obj->posX == mapX && abs(obj->posY - mapY) > 0.80*size && abs(obj->posY - mapY) < size){
		  //        printf("WallV_U\n");
		  if(isMapWall(mapX/size, mapY/size)){
			  //        printf("WallV_U\n");
			  obj->posY = mapY + size;
			  //accelHorizontal(obj, accelTab);
			  obj->accelerationY = 0;

		  }
	  }
  }

  void ObjectOverlapWallH1of2R_U_D(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(obj->posX < mapX && abs(obj->posY - mapY) < size/2 && abs(obj->posY - mapY) != 0 && mapX != obj->posX && abs(obj->posX - mapX) > size/2 && abs(obj->posX - mapX) < size ){
		  //printf("2rud %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
		  if(isMapWall(mapX/size, mapY/size)){
			  //        printf("H1of2R_U_D\n");
			  obj->posX = mapX - size;
			  accelVertical(obj, accelTab);
			  //obj->accelerationX = 0;
		  }

	  }
  }

  void ObjectOverlapWallH1of2L_U_D(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(obj->posX > mapX && abs(obj->posY - mapY) < size/2 && abs(obj->posY - mapY) != 0 && mapX != obj->posX && abs(obj->posX - mapX) > size/2 && abs(obj->posX - mapX < size)){
		  //printf("H1of2L_U_D\n");
		  if(isMapWall(mapX/size, mapY/size)){
			  //                printf("H1of2L_U_D\n");
			  obj->posX = mapX + size;
			  accelVertical(obj, accelTab);
			  //obj->accelerationX = 0;
		  }
	  }
  }

  void ObjectOverlapWallH_Half(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(abs(obj->posX - mapX) > size/2 && abs(obj->posX - mapX) < size && abs(obj->posY - mapY) == size/2){
		  //                printf("wallH_half %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
		  if(isMapWall(mapX/size, mapY/size)){
			  //                        printf("WALL!_Half\n");
			  obj->posX = mapX - size;
			  obj->accelerationX = 0;

			  //        accelVertical(obj, accelTab);
		  }

	  }
  }

  ///roznice miedzy tymi dwoma
  void ObjectOverlapWallV1of2U_L_R(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(abs(obj->posX - mapX) < abs(obj->posY - mapY) && obj->posY > mapY && obj->posX != mapX && abs(obj->posY - mapY) > size/2 && abs(obj->posY - mapY) < size && abs(obj->posX - mapX) < size/2){

		  if(isMapWall(mapX/size, mapY/size)){
			  //        printf("V-1of2-U %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
			  //                printf("U-1of2");
			  obj->posY = mapY + size;
			  //accelHorizontal(obj, accelTab);
			  obj->accelerationY = 0;
		  }
		  //        accelHorizontal(obj, accelTab);
	  }
  }

  void ObjectOverlapWallV1of2D_R_L(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(abs(obj->posX - mapX) < abs(obj->posY - mapY) && abs(obj->posY - mapY) > size/2 && abs(obj->posY - mapY) < size && abs(obj->posX - mapX) < size/2){
		  //        printf("D-1of2");
		  if(isMapWall(mapX/size, mapY/size)){
			  //        printf("V-1of2-D %d - map i, %d - map j, %f- muff.posXm %f - muff.posY, %d - mapX, %d - mapY \n", mapX/size, mapY/size, obj->posX, obj->posY, mapX, mapY);
			  //                printf("WallV1of2D_R_L\n");
			  obj->posY = mapY - size;
			  //        accelHorizontal(obj, accelTab);
			  obj->accelerationY = 0;
		  }
		  //accelHorizontal(obj, accelTab);
	  }
  }

  void ObjectOverlapWallV_Half(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(abs(obj->posY - mapY) > size/2 && abs(obj->posY - mapY) < size && abs(obj->posX - mapX) == size/2){
		  //        printf("WallV_Half\n");
		  if(isMapWall(mapX/size, mapY/size)){
			  //                printf("WallV_Half\n");

			  obj->posX += 1;

		  }
	  }
  }


  ///skrecanie

  void ObjectTurnsRight(object_s *obj, int mapX, int mapY, int size, char *accelTab){
	  if(accelTab[accW] == 1 || accelTab[accS] == 1){
		  if(accelTab[accD] == 1){
			  if(obj->posX == mapX){

				  obj->accelerationY = 0;
				  obj->accelerationX = OBJECT_ACC;
			  }
		  }
	  }
  }

  //////////
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



  void goToPathx(int wallX, int wallY, object_s *obj, char *accelTab){

	  if(obj->accelerationX <= 0){
		  obj->posX= map[wallX][wallY].x + TILE_SIZE;
		  if(accelTab[accS] == 1){
			  muffin.accelerationX = 0;
			  muffin.accelerationY = OBJECT_ACC;
			  //        accelTab[accS] = 0;
		  }
		  else if(accelTab[accW] == 1){
			  muffin.accelerationX = 0;
			  muffin.accelerationY = -OBJECT_ACC;
			  //                accelTab[accW] = 0;
		  }

	  }

	  else if(obj->accelerationX >= 0){
		  obj->posX = map[wallX][wallY].x - TILE_SIZE;
		  if(accelTab[accS] == 1){
			  muffin.accelerationX = 0;
			  muffin.accelerationY = OBJECT_ACC;
			  //        accelTab[accS] = 0;
		  }
		  else if(accelTab[accW] == 1){
			  muffin.accelerationX = 0;
			  muffin.accelerationY = -OBJECT_ACC;
			  //        accelTab[accW] = 0;
		  }
	  }
  }

  void goToPathy(int wallX, int wallY, object_s *obj, char *accelTab){
	  if(obj->accelerationY <= 0){
		  obj->posY = map[wallX][wallY].y + TILE_SIZE;
		  if(accelTab[accD] == 1){
			  muffin.accelerationX = OBJECT_ACC;
			  muffin.accelerationY = 0;
			  //        accelTab[accD] = 0;
		  }
		  else if(accelTab[accA] == 1){
			  muffin.accelerationX = -OBJECT_ACC;
			  muffin.accelerationY = 0;
			  //        accelTab[accA] = 0;
		  }

	  }

	  else if(obj->accelerationY >= 0){
		  obj->posY = map[wallX][wallY].y - TILE_SIZE;
		  if(accelTab[accD] == 1){
			  muffin.accelerationX = OBJECT_ACC;
			  muffin.accelerationY = 0;
			  //        accelTab[accD] = 0;
		  }
		  else if(accelTab[accA] == 1){
			  muffin.accelerationX = -OBJECT_ACC;
			  muffin.accelerationY = 0;
			  //        accelTab[accA] = 0;
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

		  if ( (x1 <= x2) && ( abs(x1 - x2) < size) ) {

			  return 1;
		  }
		  else if ( (x1 <= x2) && ( abs(x1 - x2) == 0 ) ) {

			  return 1;
		  }
		  else if ( (x2 <= x1) && ( abs(x1 - x2) < size) ) {
			  return 1;
		  }
		  else if (((x2 <= x1) && (abs(x1 - x2) == 0)) ){
			  return 1;
		  }
		  return 0;
	  }

	  else if (x1 == x2){

		  if ( (y1 <= y2) && ( abs(y1 - y2) < size) ) {

			  return 1;
		  }
		  else if ( (y1 <= y2) && ( abs(y1 - y2) == 0 ) ) {

			  return 1;
		  }
		  else if ( (y2 <= y1) && ( abs(y1 - y2) < size) ) {
			  return 1;
		  }
		  else if (((y2 <= y1) && (abs(y1 - y2) == 0)) ){
			  return 1;
		  }
		  return 0;
	  }
	  else return 0;



  }

  void changeReleaseCounter( object_s *fork1, object_s *fork2){
	  if(fork1->release_counter%11 == 0){
		  fork1->release_counter = 1;
		  fork2->release_counter = 1;
	  }
  }

  int shouldReleasefork(object_s *fork){
	  if(fork->release_moment == 1){
		  return 1;
	  }
	  else return 0;
  }

  int shouldforkWalkOnPath(object_s *fork){
	  if(fork->release_moment == 0){
		  return 1;
	  }
	  else return 0;
  }

  void forkToPathx(int wallX, int wallY, object_s *fork){
	  if(fork->accelerationX < 0){
		  fork->posX = map[wallX][wallY].y + TILE_SIZE;
		  fork->accelerationX = 0;
		  fork->accelerationY = 0.5;
	  }
	  else if(fork->accelerationX > 0){
		  fork->posY = map[wallX][wallY].y - TILE_SIZE;
		  fork->accelerationX = 0;
		  fork->accelerationY = -0.5;
	  }
  }

  void forkToPathy(int wallX, int wallY, object_s *fork){
	  if(fork->accelerationY < 0){
		  fork->posY = map[wallX][wallY].y + TILE_SIZE;
		  fork->accelerationY = 0;
		  fork->accelerationX = 0.5;
	  }
	  else if(fork->accelerationY > 0){
		  fork->posY = map[wallX][wallY].y - TILE_SIZE;
		  fork->accelerationY = 0;
		  fork->accelerationX = -0.5;
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

