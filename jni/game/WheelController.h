/*
 * WheelController.h
 *
 *  ������� ����������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef WHEELCONTROLLER_H_
#define WHEELCONTROLLER_H_

#include "engine/GameObjectController.h"

class WheelController: public GameObjectController
{
	public:
		// ����������� ���� �����������
		float destinationAngle;

		// �������� ��������
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
				// ������� �������������� ������
				underControlObject->RotateTo(destinationAngle, rotationSpeed);
		};
};

#endif /* WHEELCONTROLLER_H_ */
