/*
 * field.h
 *
 *  Created on: Dec 7, 2013
 *      Author: arvostaa
 */

#ifndef FIELD_H_
#define FIELD_H_

typedef enum FIELD_TYPE {

	FIELD_WALL = 0,
	FIELD_HIGHLIGHT = 1,
	FIELD_PATH = 2,
	FIELD_GATE = 3

}fieldType_e;

typedef struct FIELD_STRUCT {

fieldType_e type;
int x, y;
int highlighted;

} field_s;




#endif /* FIELD_H_ */
