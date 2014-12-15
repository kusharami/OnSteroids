/*
 * TouchControllerObject.h
 *
 *  ������� ������ ��� �������������� � ��������������� ������������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef TOUCHCONTROLLEROBJECT_H_
#define TOUCHCONTROLLEROBJECT_H_

#include "GameObjects.h"
#include "GameObjectController.h"
#include "TouchController.h"
#include "InputListener.h"

class TouchControllerObject: virtual public GameObject, public TouchController
{
	public:
		// ���������� �������
		GameObjectController* controller;

		// ������ �������������
		List* touchEvents;

		TouchControllerObject() : GameObject(), TouchController(), controller(NULL),
				touchEvents(NULL) { flags |= GOF_GHOST; };

		// ����� ������ ��� ��������
		void TakeControl(GameObject* object)
		{
			controller->TakeControlUnder(object);
		}

		// ���� ������, GameObjectController::Control
		virtual void TouchMove(double latency, float x, float y)
		{
			TouchController::TouchMove(latency, x, y);
			controller->Control(latency);
		}

		// ���� ���������, ������� GameObjectController::Stop
		virtual void TouchRelease(double latency, float x, float y)
		{
			TouchController::TouchRelease(latency, x, y);
			controller->Stop(latency);
		}

		// �������� ��������� �������
		virtual void Update(double latency)
		{
			ListItem* item = touchEvents->root;
			while (item != NULL)
			{
				TouchEventRec* event = (TouchEventRec*) item->object;
				ListItem* it = item;
				item = item->next;
				switch (event->actionMasked)
				{
					case ACTION_DOWN:
					case ACTION_POINTER_DOWN:
					{
						// ����� �������������
						TouchPointer* ptr = &event->pointers[event->pointerIndex];
						if (touchID < 0)
							TouchDown(latency, ptr->id, ptr->x, ptr->y);
						break;
					}
					case ACTION_UP:
					case ACTION_POINTER_UP:
					case ACTION_CANCEL:
					{
						// ��������� �������
						if (touchID >= 0)
						{
							TouchPointer* ptr = &event->pointers[event->pointerIndex];
							if (touchID == ptr->id)
								TouchRelease(latency, ptr->x, ptr->y);
						}
						break;
					}
					case ACTION_MOVE:
					{
						// ������� ����������
						TouchPointer* ptr = &event->pointers[0];
						TouchPointer* end = &event->pointers[event->pointerCount];
						bool touchNotFound = true;
						while (ptr < end)
						{
							int pid = ptr->id;
							if (pid >= 0)
							{
								if (touchID < 0)
								{
									TouchDown(latency, pid, ptr->x, ptr->y);
									if (touchID == pid)
										// ������������� ������
										touchNotFound = false;
								} else
								if (touchID == pid)
								{
									// ������������� ������
									touchNotFound = false;
									TouchMove(latency, ptr->x, ptr->y);
								}
							}
							ptr++;
						}
						if (touchID > 0 && touchNotFound)
							// ���� �� ����� ������������� � ����������� �������������, ��������� ���� ������
							TouchRelease(latency, touchX, touchY);
						break;
					}
				}
			}
			// ��������� ������
			GameObject::Update(latency);
		};
};


#endif /* TOUCHCONTROLLEROBJECT_H_ */
