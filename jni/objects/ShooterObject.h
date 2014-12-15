/*
 * ShooterObject.h
 *
 *  Игровой объект с пушкой.
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef SHOOTEROBJECT_H_
#define SHOOTEROBJECT_H_

#include "engine/GameObjects.h"

class ShooterObject: virtual public GameObject
{
	public:
		// частота перезарядки
		double reloadRate;

		// время перезарядки
		double reloadCounter;

		// стреляет ли
		bool shooting;

		// координаты пушки
		Vector2f barrelOrigin;

		// координаты пушки с учётом вращения
		Vector2f shootPoint;

		ShooterObject() : GameObject(), reloadRate(0.0), reloadCounter(0.0), shooting(false),
				barrelOrigin(), shootPoint() {};

		// виртуальный метод создания объекта
		virtual GameObject* CreateProjectile() = 0;

		// Выстрел
		virtual void Shoot(GameObject* object)
		{
			// координаты снаряда
			object->coord = coord + shootPoint;
			// приоритет
			object->priority = priority;
		};

		virtual void DoSetAngle(float angle)
		{
			GameObject::DoSetAngle(angle);
			// вращаем координаты пушки
			shootPoint = barrelOrigin.Rotate(angleVector);
		}

		virtual void Update(double latency)
		{
			if (shooting)
			{
				// если стреляет
				reloadCounter -= latency;
				// ждём перезарядки
				if (reloadCounter <= 0.0)
				{
					reloadCounter = reloadRate;
					// создаём снаряд
					GameObject* projectile = CreateProjectile();
					if (projectile != NULL)
					{
						// стреляем
						Shoot(projectile);
						owner->Add(projectile);
					}
				}
			} else
				reloadCounter = 0.0;

			GameObject::Update(latency);
		}

		// установить координаты пушки
		void SetBarrelOrigin(float x, float y)
		{
			shootPoint = (barrelOrigin = Vector2f(x, y)).Rotate(angleVector);
		}
};

#endif /* SHOOTEROBJECT_H_ */
