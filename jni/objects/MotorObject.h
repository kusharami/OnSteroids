/*
 * MotorObject.h
 *
 *  ������� ������ � ����������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */


#ifndef MOTOROBJECT_H_
#define MOTOROBJECT_H_

#include "engine/GameObjects.h"

class MotorObject: virtual public GameObject
{
	public:
		// ������������ ��������
		float maxSpeed;

		// ���������
		float accelerationStep;

		// ����������
		float brakingStep;

		// ����������� ��
		bool uncontrolled;

		// ���������� ��
		bool accelerating;

		// ����� �� � ����������
		bool canBrake;

		MotorObject() : GameObject(), accelerating(false), maxSpeed(0.0), uncontrolled(false),
				accelerationStep(0.0), brakingStep(0.0), canBrake(false)  {};

		// ����� �������������
		virtual void BeforeContact()
		{
			// �������� ����������� ����� ��������
			if (rotateMode == ROTATE_NO)
				rotationSpeed = .0;

			rotateMode = ROTATE_ENDLESS;
			uncontrolled = true;
		}

		// ����� ������������
		virtual void AfterContact()
		{
			// �������� ��������
			this->speed = motion.Length();
			// ��������� �����������
			direction = DirectionVector<float>(atan2(motion.y, motion.x));
		}

		virtual void RotateTo(float finalAngle, float speed)
		{
			// ���� ��������� ��� �������� �������� � ���������� ��������
			if (!uncontrolled || fabs(rotationSpeed) <= fabs(speed))
				// ������ �������
				GameObject::RotateTo(finalAngle, speed);
		}

		virtual void Move(double latency)
		{
			float maxSpeed = this->maxSpeed;

			if (uncontrolled)
			{
				// �������������

				float step = brakingStep * latency;

				// ������� �������� �� ����������
				if (speed > maxSpeed)
					SetSpeed(speed - step);

				if (rotateMode == ROTATE_ENDLESS)
				{
					// ������� �������� ��������
					bool noRotate;
					if (rotationSpeed < 0)
						noRotate = ((rotationSpeed += step) >= .0);
					else
						noRotate = ((rotationSpeed -= step) <= .0);

					if (noRotate)
						rotateMode = ROTATE_NO;
				}

				if (speed <= maxSpeed && rotateMode != ROTATE_ENDLESS)
					// ���� �� � ���������� ��������, ��������������� ����������
					uncontrolled = false;
			} else
			{
				if (accelerating)
				{
					// ���� ���������� � ������ ����� ����������� ��� �������� �� ������������
					if (oldAngle != angle || speed < maxSpeed)
					{
						// ������ ������ �������� � ������� ��������� � ���� �����������
						motion += DirectionVectorD<float>(angle) * (accelerationStep * latency);

						// ����� ��������
						float speed = motion.Length();
						if (speed > maxSpeed)
						{
							// ���� ������ ������������, ������������
							direction = DirectionVector<float>(atan2(motion.y, motion.x));
							motion = direction * maxSpeed;
							this->speed = maxSpeed;
						} else
							this->speed = speed;
					}
					// ����� ���������
					canBrake = true;
				} else
				{
					if (canBrake)
					{
						canBrake = false;
						if (speed < maxSpeed)
							// ������������ �����������
							direction = DirectionVector<float>(atan2(motion.y, motion.x));
					}
					if (speed > 0)
						// ������� ��������
						SetSpeed(speed - brakingStep * latency);
					else
						return;
				}
			}
			// ���������� ������
			GameObject::Move(latency);
		};
};


#endif /* MOTOROBJECT_H_ */
