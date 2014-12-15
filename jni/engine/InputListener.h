/*
 * InputListener.h
 *
 *  ������� ����� ��� ��������� �����.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef INPUTLISTENER_H_
#define INPUTLISTENER_H_

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_CANCEL 3
#define ACTION_POINTER_DOWN 5
#define ACTION_POINTER_UP 6

// ��������
typedef struct
{
	// ������������� ���������� �����
	int32_t 	deficeId;
	// �������� �����
	int32_t 	source;
	// ��������
	uint32_t	action;
	// ������� �������
	int32_t		keyCode;
	// ������
	uint32_t 	unicodeChar;
	// ����� �������
	int64_t 	downTime;
	// ����� �������
	int64_t 	eventTime;
	// ��������� ������� Alt, Ctrl, Caps Lock � ��� �����
	uint32_t 	metaState;
} KeyEventRec;

// ��������� �������������
typedef struct
{
	// ������������� �������������
	int32_t		id;
	// ����������
	float		x;
	float		y;
} TouchPointer;

// ������ �������������
typedef struct
{
	// ��������
	uint32_t	action;
	// �������� ��� ������ ����������
	uint32_t	actionMasked;
	// ����� ��������� �������������
	uint32_t	pointerIndex;
	// ���������� ���������� �������������
	uint32_t	pointerCount;
	// ��������� �������������
	TouchPointer pointers[10];
} TouchEventRec;

class InputListener
{
	public:
		// ������ ��������
		virtual void KeyEvent(KeyEventRec *event) {};
		// �������������
		virtual void TouchEvent(TouchEventRec *event) {};

		virtual ~InputListener() {};
};

#endif /* INPUTLISTENER_H_ */
