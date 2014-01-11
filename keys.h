#ifndef KEYS_H_
#define KEYS_H_

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

#include "field.h"
#include "object.h"
#include <stdlib.h>

typedef enum KeyOn{
	rightD ,
	leftA ,
	upW,
	downS,
}Key;

typedef enum ACC_counter{
	accD,
	accA,
	accW,
	accS,
	acc0

}acc_enum;



//void OnKeyDowndd(SDLKey sym);


#endif /* KEYS_H_ */
