/*
 * GameObjectController.h
 *
 *  ������� ����� ��� ���������� ������� ��������.
 *
 *  ���� ��������: 16.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */


#ifndef GAMEOBJECTCONTROLLER_H_
#define GAMEOBJECTCONTROLLER_H_

#include "GameObjects.h"

class GameObjectController
{
	public:
		GameObject* underControlObject;

		GameObjectController() : underControlObject(NULL) {};

		virtual void TakeControlUnder(GameObject* object)
		{
			underControlObject = object;
		};

		virtual void Control(double latency) {};

		virtual void Stop(double latency) {}

		virtual ~GameObjectController() {};
};


#endif /* GAMEOBJECTCONTROLLER_H_ */
