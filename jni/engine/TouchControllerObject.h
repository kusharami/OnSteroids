/*
 * TouchControllerObject.h
 *
 *  Игровой объект для взаимодействия с прикосновениями пользователя.
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
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
		// Контроллер объекта
		GameObjectController* controller;

		// список прикосновений
		List* touchEvents;

		TouchControllerObject() : GameObject(), TouchController(), controller(NULL),
				touchEvents(NULL) { flags |= GOF_GHOST; };

		// взять объект под контроль
		void TakeControl(GameObject* object)
		{
			controller->TakeControlUnder(object);
		}

		// если нажать, GameObjectController::Control
		virtual void TouchMove(double latency, float x, float y)
		{
			TouchController::TouchMove(latency, x, y);
			controller->Control(latency);
		}

		// если отпустить, вызывет GameObjectController::Stop
		virtual void TouchRelease(double latency, float x, float y)
		{
			TouchController::TouchRelease(latency, x, y);
			controller->Stop(latency);
		}

		// Обновить состояние объекта
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
						// Новое прикосновение
						TouchPointer* ptr = &event->pointers[event->pointerIndex];
						if (touchID < 0)
							TouchDown(latency, ptr->id, ptr->x, ptr->y);
						break;
					}
					case ACTION_UP:
					case ACTION_POINTER_UP:
					case ACTION_CANCEL:
					{
						// Отпустили пальчик
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
						// двигаем пальчиками
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
										// идентификатор найден
										touchNotFound = false;
								} else
								if (touchID == pid)
								{
									// идентификатор найден
									touchNotFound = false;
									TouchMove(latency, ptr->x, ptr->y);
								}
							}
							ptr++;
						}
						if (touchID > 0 && touchNotFound)
							// если не нашли прикосновение с определённым идентификатор, отпускаем этот объект
							TouchRelease(latency, touchX, touchY);
						break;
					}
				}
			}
			// обновляем дальше
			GameObject::Update(latency);
		};
};


#endif /* TOUCHCONTROLLEROBJECT_H_ */
