/*
 * ShooterObject.h
 *
 *  ������� ������ � ������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef SHOOTEROBJECT_H_
#define SHOOTEROBJECT_H_

#include "engine/GameObjects.h"

class ShooterObject: virtual public GameObject
{
	public:
		// ������� �����������
		double reloadRate;

		// ����� �����������
		double reloadCounter;

		// �������� ��
		bool shooting;

		// ���������� �����
		Vector2f barrelOrigin;

		// ���������� ����� � ������ ��������
		Vector2f shootPoint;

		ShooterObject() : GameObject(), reloadRate(0.0), reloadCounter(0.0), shooting(false),
				barrelOrigin(), shootPoint() {};

		// ����������� ����� �������� �������
		virtual GameObject* CreateProjectile() = 0;

		// �������
		virtual void Shoot(GameObject* object)
		{
			// ���������� �������
			object->coord = coord + shootPoint;
			// ���������
			object->priority = priority;
		};

		virtual void DoSetAngle(float angle)
		{
			GameObject::DoSetAngle(angle);
			// ������� ���������� �����
			shootPoint = barrelOrigin.Rotate(angleVector);
		}

		virtual void Update(double latency)
		{
			if (shooting)
			{
				// ���� ��������
				reloadCounter -= latency;
				// ��� �����������
				if (reloadCounter <= 0.0)
				{
					reloadCounter = reloadRate;
					// ������ ������
					GameObject* projectile = CreateProjectile();
					if (projectile != NULL)
					{
						// ��������
						Shoot(projectile);
						owner->Add(projectile);
					}
				}
			} else
				reloadCounter = 0.0;

			GameObject::Update(latency);
		}

		// ���������� ���������� �����
		void SetBarrelOrigin(float x, float y)
		{
			shootPoint = (barrelOrigin = Vector2f(x, y)).Rotate(angleVector);
		}
};

#endif /* SHOOTEROBJECT_H_ */
