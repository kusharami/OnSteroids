/*
 * Bullet.h
 *
 *  ������� ������ "����".
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "engine/GameObjects.h"

class Bullet: public GameObject
{
	public:
		// ����� �����
		float flightCounter;

		// ��� ���������
		float fadeStep;

		// ���������� ��
		bool blowupState;

		Bullet(): GameObject(), flightCounter(0.0), fadeStep(1.0), blowupState(false) {};

		virtual void Move(double latency)
		{
			if (!blowupState)
				// ���� �� ����������, �� �������
				GameObject::Move(latency);

			if (flightCounter <= 0.0)
			{
				// ���� ����� �����, ��������
				float a = (alpha -= fadeStep * latency);
				if (a <= 0.0)
					// ���� �������, �������
					Recycle();
			} else
				flightCounter -= latency;
		};
};

#endif /* BULLET_H_ */
