/*
 * object.h
 *
 *  Created on: Dec 7, 2013
 *      Author: arvostaa
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#define TILE_SIZE 50
#define MAP_W 16
#define MAP_H 12
#define OBJECT_ACC 0.15

#include <time.h>
#include <stdio.h>
#include "field.h"

typedef struct OBJECT_STRUCT {

	float posX;
	float posY;
	float accelerationX;
	float accelerationY;
	int speed;
	int size;
	int release_counter;
	int release_moment;
	int on_path_moment;

} object_s;

typedef struct cookie{
	struct cookie *next;

	int posX;
	int posY;

}cookie_s;

void insertCookie(cookie_s *cookieList, int cookie_amount);
void drawCookiePosition(int *cookieX, int *cookieY);
void addCookies(cookie_s *cookieList,int *cookie_delay, int cookie_amount);

void setCookiePosition(cookie_s *cookieList, int *cookiePosX, int *cookiePosY);
void addCookies(cookie_s *cookieList, int *cookie_delay, int cookie_amount);
void changeReleaseCounter( object_s *ghost1, object_s *ghost2);
int shouldReleaseGhost(object_s *ghost);
void goToPathx(int wallX, int wallY, object_s *obj, char *accelTab);
void ghostToPathy(int wallX, int wallY, object_s *ghost);
int isObjectOnTile (int objX, int objY, int tileX, int tileY);
int isObjectOnTheEdgeOfMapField(object_s *object, int tileX, int tileY, int size);
int isTileOnTile(int x1, int y1, int x2, int y2, int size);
int isObjectOverlapMapField(object_s *object, int tileX, int tileY, int size);
void changeReleaseCounter( object_s *ghost1, object_s *ghost2);
int shouldGhostWalkOnPath(object_s *ghost);
int isMapWall(int wallX, int wallY);
int isMapPath(int wallX, int wallY);
void goToPathx(int wallX, int wallY, object_s *obj, char *accelTab);
void goToPathy(int wallX, int wallY, object_s *obj,  char *accelTab);
int isObjectOnGate(int wallX, int wallY);
int isObjectOnPath(int wallX, int wallY);
int Over_Edge(object_s *object, int tileX, int tileY, int size);

void changeKeys(object_s *obj, char *accelTab);
void accelVertical(object_s *obj, char *accelTab);
void accelHorizontal(object_s *obj, char *accelTab);
void ObjectOverlapWallH_R(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallH_L(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallV_D(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallV_U(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallH1of2R_U_D(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallH1of2L_U_D(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallH_Half(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallV1of2U_L_R(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallV1of2D_R_L(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOverlapWallV_Half(object_s *obj, int mapX, int mapY, int size, char *accelTab);

void ObjectOverlapPathH_R_D(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOnCornerH_R_U(object_s *obj, int mapX, int mapY, int size, char *accelTab);
void ObjectOnCornerV_L_U(object_s *obj, int mapX, int mapY, int size, char *accelTab);

void ObjectTurnsRight(object_s *obj, int mapX, int mapY, int size, char *accelTab);


void directObjectToPath(object_s *obj, int mapX, int mapY, int size, char *accelTab);



#endif /* OBJECT_H_ */
