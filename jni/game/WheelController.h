/*
 * WheelController.h
 *
 *  Рулевое управление.
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef WHEELCONTROLLER_H_
#define WHEELCONTROLLER_H_

#include "engine/GameObjectController.h"

class WheelController: public GameObjectController
{
	public:
		// назначенный угол направления
		float destinationAngle;

		// скорость вращения
		float rotationSpeed;

		WheelController() : GameObjectController(), destinationAngle(0.0), rotationSpeed(0.0) {};

		virtual void TakeControlUnder(GameObject* object)
		{
			GameObjectController::TakeControlUnder(object);
			if (object)
				rotationSpeed = object->rotationSpeed;
		}

		virtual void Control(double latency)
		{
			if (underControlObject)
				// вращаем контроллируемы объект
				underControlObject->RotateTo(destinationAngle, rotationSpeed);
		};
};

#endif /* WHEELCONTROLLER_H_ */
