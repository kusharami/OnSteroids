/*
 * FlyingShooter.h
 *
 *  ������� ������ "������� �������".
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef FLYINGSHOOTER_H_
#define FLYINGSHOOTER_H_

#include "engine/GameObjects.h"
#include "Bullet.h"
#include "ShooterObject.h"
#include "MotorObject.h"

class FlyingShooter: virtual public ShooterObject, virtual public MotorObject
{
	public:
		// �������� ����
		ShapeF* bulletShape;

		// �������� ����
		float bulletSpeed;

		// ����� ����� ����
		float bulletTime;

		// ����� ��������� ����
		float bulletFadeStep;

		// ����� ���� �� ������� ������
		OffscreenEvent* bulletOffscreenEvent;

		FlyingShooter() : ShooterObject(), MotorObject(), bulletShape(NULL), bulletSpeed(0.0),
				bulletTime(0.0), bulletFadeStep(1.0), bulletOffscreenEvent(NULL) {};

		// ������� ������ ����
		virtual GameObject* CreateProjectile()
		{
			return new Bullet();
		}

		// �������
		virtual void Shoot(GameObject* object)
		{
			ShooterObject::Shoot(object);

			Bullet* bullet = dynamic_cast<Bullet*> (object);
			if (bullet != NULL)
			{
				// �������������� ����� ����
				bullet->offscreenEvent = bulletOffscreenEvent;
				bullet->flightCounter = bulletTime;
				bullet->fadeStep = bulletFadeStep;
				bullet->updateDirection = true;
				bullet->SetAngle(angle);
				bullet->SetSpeed(bulletSpeed);
				bullet->SetShape(bulletShape, &scale, NULL);
			}
		}
};


#endif /* FLYINGSHOOTER_H_ */
