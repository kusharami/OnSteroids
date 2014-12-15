/*
 * Wheel.h
 *
 *  Игровой объект-контроллер "руль".
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef WHEEL_H_
#define WHEEL_H_

#include "engine/TouchControllerObject.h"
#include "game/WheelController.h"

class Wheel: public TouchControllerObject, public WheelController
{
	public:
		Wheel() : TouchControllerObject(), WheelController() { controller = this; };

		virtual void TakeControlUnder(GameObject* object)
		{
			WheelController::TakeControlUnder(object);
			if (object)
				// скорость вращения объекта
				GameObject::rotationSpeed = object->rotationSpeed;
		}

		virtual void Control(double latency)
		{
			// вращаем по направлению к ночке прикосновения
			RotateTo(touchX, touchY, 0);
			destinationAngle = this->finalAngle;
			WheelController::Control(latency);
		};
};


#endif /* WHEEL_H_ */
