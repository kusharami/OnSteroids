/*
 * TriggerController.h
 *
 *  ������ ������ ������������� ��� �������� �������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */


#ifndef TRIGGERCONTROLLER_H_
#define TRIGGERCONTROLLER_H_

#include "GameObjectController.h"

template <typename TriggerObjectClass, bool TriggerObjectClass::*trigger>
class TriggerController: public GameObjectController
{
	public:
		// ��������������� ������
		TriggerObjectClass* triggerObject;

		TriggerController() : GameObjectController(), triggerObject(NULL) {};

		// ����� ��� ��������
		virtual void TakeControlUnder(GameObject* object)
		{
			GameObjectController::TakeControlUnder(object);
			triggerObject = dynamic_cast<TriggerObjectClass*> (object);
		};

		// ���������������
		virtual void Control(double latency)
		{
			if (triggerObject)
				triggerObject->*trigger = true;
		};

		// ���������
		virtual void Stop(double latency)
		{
			if (triggerObject)
				triggerObject->*trigger = false;
		};
};

#endif /* TRIGGERCONTROLLER_H_ */
