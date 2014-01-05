#ifndef KEYS_H_
#define KEYS_H_

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

#include "field.h"
#include "object.h"
#include <stdlib.h>

typedef enum KeyOn{
	rightD = 5,
	leftA ,
	upW,
	downS,
}Key;

char KeysOn[4] = {rightD, leftA, upW, downS};


//void OnKeyDowndd(SDLKey sym);


#endif /* KEYS_H_ */
