/*
 * MotorObject.h
 *
 *  »гровой объект с двигателем.
 *
 *  ƒата создани€: 10.09.2013
 *  ƒата изменени€: 16.09.2013
 *
 *  јвтор: „ерданцева јлександра
 */


#ifndef MOTOROBJECT_H_
#define MOTOROBJECT_H_

#include "engine/GameObjects.h"

class MotorObject: virtual public GameObject
{
	public:
		// максимальна€ скорость
		float maxSpeed;

		// ускорение
		float accelerationStep;

		// торможение
		float brakingStep;

		// неуправл€ем ли
		bool uncontrolled;

		// ускор€етс€ ли
		bool accelerating;

		// готов ли к торможению
		bool canBrake;

		MotorObject() : GameObject(), accelerating(false), maxSpeed(0.0), uncontrolled(false),
				accelerationStep(0.0), brakingStep(0.0), canBrake(false)  {};

		// перед столкновением
		virtual void BeforeContact()
		{
			// включает бесконечный режим вращени€
			if (rotateMode == ROTATE_NO)
				rotationSpeed = .0;

			rotateMode = ROTATE_ENDLESS;
			uncontrolled = true;
		}

		// после столкновени€
		virtual void AfterContact()
		{
			// измерить скорость
			this->speed = motion.Length();
			// вычислить направление
			direction = DirectionVector<float>(atan2(motion.y, motion.x));
		}

		virtual void RotateTo(float finalAngle, float speed)
		{
			// если управл€ем или скорость вращени€ в допустимых пределах
			if (!uncontrolled || fabs(rotationSpeed) <= fabs(speed))
				// начать поворот
				GameObject::RotateTo(finalAngle, speed);
		}

		virtual void Move(double latency)
		{
			float maxSpeed = this->maxSpeed;

			if (uncontrolled)
			{
				// неуправл€емый

				float step = brakingStep * latency;

				// снижаем скорость до нормальной
				if (speed > maxSpeed)
					SetSpeed(speed - step);

				if (rotateMode == ROTATE_ENDLESS)
				{
					// снижаем скорость вращени€
					bool noRotate;
					if (rotationSpeed < 0)
						noRotate = ((rotationSpeed += step) >= .0);
					else
						noRotate = ((rotationSpeed -= step) <= .0);

					if (noRotate)
						rotateMode = ROTATE_NO;
				}

				if (speed <= maxSpeed && rotateMode != ROTATE_ENDLESS)
					// если всЄ в допустимых пределах, восстанавливаем управление
					uncontrolled = false;
			} else
			{
				if (accelerating)
				{
					// если ускор€етс€ и задано новое направление или скорость не максимальна€
					if (oldAngle != angle || speed < maxSpeed)
					{
						// мен€ем вектор движени€ с помощью ускорени€ и угла направлени€
						motion += DirectionVectorD<float>(angle) * (accelerationStep * latency);

						// нова€ скорость
						float speed = motion.Length();
						if (speed > maxSpeed)
						{
							// если больше максимальной, корректируем
							direction = DirectionVector<float>(atan2(motion.y, motion.x));
							motion = direction * maxSpeed;
							this->speed = maxSpeed;
						} else
							this->speed = speed;
					}
					// можно тормозить
					canBrake = true;
				} else
				{
					if (canBrake)
					{
						canBrake = false;
						if (speed < maxSpeed)
							// корректируем направление
							direction = DirectionVector<float>(atan2(motion.y, motion.x));
					}
					if (speed > 0)
						// снижаем скорость
						SetSpeed(speed - brakingStep * latency);
					else
						return;
				}
			}
			// перемещаем объект
			GameObject::Move(latency);
		};
};


#endif /* MOTOROBJECT_H_ */
