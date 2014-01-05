/*
 * object.h
 *
 *  Created on: Dec 7, 2013
 *      Author: arvostaa
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#define TILE_SIZE 50

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

void changeReleaseCounter( object_s *ghost1, object_s *ghost2);
int shouldReleaseGhost(object_s *ghost);
void ghostToPathx(int wallX, int wallY, object_s *ghost);
void ghostToPathy(int wallX, int wallY, object_s *ghost);
int isObjectOnTile (int objX, int objY, int tileX, int tileY);
int isObjectOnTheEdgeOfMapField(object_s *object, int tileX, int tileY, int size);
int isTileOnTile(int x1, int y1, int x2, int y2, int size);
int isObjectOverlapMapField(object_s *object, int tileX, int tileY, int size);
void changeReleaseCounter( object_s *ghost1, object_s *ghost2);
int shouldGhostWalkOnPath(object_s *ghost);
int isObjectOnWall(int wallX, int wallY);
void goToPathx(int wallX, int wallY, object_s *obj);
void goToPathy(int wallX, int wallY, object_s *obj);
int isObjectOnGate(int wallX, int wallY);
int isObjectOnPath(int wallX, int wallY);
int Over_Edge(object_s *object, int tileX, int tileY, int size);



#endif /* OBJECT_H_ */
