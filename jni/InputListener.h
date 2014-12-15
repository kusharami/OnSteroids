/*
 * InputListener.h
 *
 *  Created on: 02.09.2013
 *      Author: Natalie
 */

#ifndef INPUTLISTENER_H_
#define INPUTLISTENER_H_

typedef struct
{
	int32_t 	deficeId;
	int32_t 	source;
	uint32_t 	unicodeChar;
	int64_t 	downTime;
	int64_t 	eventTime;
	uint32_t 	metaState;
} KeyEvent;

typedef struct
{
	uint32_t	id;
	float		x;
	float		y;
} TouchPointer;

typedef struct
{
	uint32_t	action;
	uint32_t	actionMask;
	uint32_t	actionIndex;
	uint32_t	pointerCount;
	TouchPointer pointers[10];
} MotionEvent;

class InputListener
{
	public:
		virtual bool KeyDown(int keyCode, KeyEvent *event) { return false; };
		virtual bool KeyUp(int keyCode, KeyEvent *event) { return false; };
		virtual bool TouchEvent(MotionEvent *event) { return false; };
		virtual ~InputListener() {};
};

#endif /* INPUTLISTENER_H_ */
