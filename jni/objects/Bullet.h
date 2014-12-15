/*
 * Bullet.h
 *
 *  Игровой объект "пуля".
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "engine/GameObjects.h"

class Bullet: public GameObject
{
	public:
		// время полёта
		float flightCounter;

		// шаг затухания
		float fadeStep;

		// взрывается ли
		bool blowupState;

		Bullet(): GameObject(), flightCounter(0.0), fadeStep(1.0), blowupState(false) {};

		virtual void Move(double latency)
		{
			if (!blowupState)
				// если не взрывается, то двигать
				GameObject::Move(latency);

			if (flightCounter <= 0.0)
			{
				// если время вышло, затухать
				float a = (alpha -= fadeStep * latency);
				if (a <= 0.0)
					// если затухла, удалить
					Recycle();
			} else
				flightCounter -= latency;
		};
};

#endif /* BULLET_H_ */
