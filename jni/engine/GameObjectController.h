/*
 * GameObjectController.h
 *
 *  Базовый класс для управления игровым объектом.
 *
 *  Дата создания: 16.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
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
