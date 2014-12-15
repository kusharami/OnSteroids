/*
 * FlyingShooter.h
 *
 *  Игровой объект "летучий стрелок".
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
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
		// картинка пули
		ShapeF* bulletShape;

		// скорость пули
		float bulletSpeed;

		// время полёта пули
		float bulletTime;

		// время затухания пули
		float bulletFadeStep;

		// выход пули за границы экрана
		OffscreenEvent* bulletOffscreenEvent;

		FlyingShooter() : ShooterObject(), MotorObject(), bulletShape(NULL), bulletSpeed(0.0),
				bulletTime(0.0), bulletFadeStep(1.0), bulletOffscreenEvent(NULL) {};

		// создать объект пули
		virtual GameObject* CreateProjectile()
		{
			return new Bullet();
		}

		// выстрел
		virtual void Shoot(GameObject* object)
		{
			ShooterObject::Shoot(object);

			Bullet* bullet = dynamic_cast<Bullet*> (object);
			if (bullet != NULL)
			{
				// инициализируем новую пулю
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
