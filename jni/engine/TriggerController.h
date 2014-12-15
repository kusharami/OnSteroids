/*
 * TriggerController.h
 *
 *  Шаблон класса переключателя для игрового объекта.
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */


#ifndef TRIGGERCONTROLLER_H_
#define TRIGGERCONTROLLER_H_

#include "GameObjectController.h"

template <typename TriggerObjectClass, bool TriggerObjectClass::*trigger>
class TriggerController: public GameObjectController
{
	public:
		// контроллируемый объект
		TriggerObjectClass* triggerObject;

		TriggerController() : GameObjectController(), triggerObject(NULL) {};

		// взять под контроль
		virtual void TakeControlUnder(GameObject* object)
		{
			GameObjectController::TakeControlUnder(object);
			triggerObject = dynamic_cast<TriggerObjectClass*> (object);
		};

		// контроллировать
		virtual void Control(double latency)
		{
			if (triggerObject)
				triggerObject->*trigger = true;
		};

		// отпустить
		virtual void Stop(double latency)
		{
			if (triggerObject)
				triggerObject->*trigger = false;
		};
};

#endif /* TRIGGERCONTROLLER_H_ */
