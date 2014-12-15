/*
 * Wheel.h
 *
 *  ������� ������-���������� "����".
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
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
				// �������� �������� �������
				GameObject::rotationSpeed = object->rotationSpeed;
		}

		virtual void Control(double latency)
		{
			// ������� �� ����������� � ����� �������������
			RotateTo(touchX, touchY, 0);
			destinationAngle = this->finalAngle;
			WheelController::Control(latency);
		};
};


#endif /* WHEEL_H_ */
