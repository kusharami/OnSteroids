/*
 * InputListener.h
 *
 *  Базовый класс для обработки ввода.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef INPUTLISTENER_H_
#define INPUTLISTENER_H_

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_CANCEL 3
#define ACTION_POINTER_DOWN 5
#define ACTION_POINTER_UP 6

// кнопочки
typedef struct
{
	// идентификатор устройства ввода
	int32_t 	deficeId;
	// источник ввода
	int32_t 	source;
	// действие
	uint32_t	action;
	// нажатая клавиша
	int32_t		keyCode;
	// символ
	uint32_t 	unicodeChar;
	// время нажатия
	int64_t 	downTime;
	// время события
	int64_t 	eventTime;
	// системные клавиши Alt, Ctrl, Caps Lock и так далее
	uint32_t 	metaState;
} KeyEventRec;

// указатель прикосновения
typedef struct
{
	// идентификатор прикосновения
	int32_t		id;
	// координаты
	float		x;
	float		y;
} TouchPointer;

// данные прикосновения
typedef struct
{
	// действие
	uint32_t	action;
	// действие без лишней информации
	uint32_t	actionMasked;
	// номер указателя прикосновения
	uint32_t	pointerIndex;
	// количество указателей прикосновения
	uint32_t	pointerCount;
	// указатели прикосновения
	TouchPointer pointers[10];
} TouchEventRec;

class InputListener
{
	public:
		// нажата кнопочка
		virtual void KeyEvent(KeyEventRec *event) {};
		// прикосновение
		virtual void TouchEvent(TouchEventRec *event) {};

		virtual ~InputListener() {};
};

#endif /* INPUTLISTENER_H_ */
