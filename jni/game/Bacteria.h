/*
 * Bacteria.h
 *
 *  Игровой объект "бактерия".
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef BACTERIA_H_
#define BACTERIA_H_

#include "objects/FlyingShooter.h"
#include "game/Photon.h"

#define BACTERIA_ID 1

class Bacteria;

// описатель функции взаимодействия с бактерией
typedef void BactUpdateProc(Bacteria* sender);

class Bacteria: public FlyingShooter
{
	public:
		// шаг затухания
		float deathStep;

		// время защиты от повреждения
		float shieldTime;

		// максимальная частота повреждений
		float shieldRate;

		// здоровье
		float health;

		// на изменении здоровья
		BactUpdateProc* onUpdateHealth;

		// убили
		BactUpdateProc* onDeath;

		// уничтожили пилюльку
		KillPillProc* onKillPill;

		Bacteria() : FlyingShooter(), health(100), onUpdateHealth(NULL), onDeath(NULL),
				shieldTime(0), shieldRate(0), deathStep(0), onKillPill(NULL) { id = BACTERIA_ID; };

		// Новый фотон
		virtual GameObject* CreateProjectile()
		{
			return new Photon();
		}

		virtual void Update(double latency)
		{
			if (health <= 0)
			{
				// если умер, затухаем
				alpha -= deathStep * latency;
				if (alpha <= 0)
				{
					if (onDeath != NULL)
						// окончательно умер
						onDeath(this);
					else
						Recycle();
				}
			} else
			{
				FlyingShooter::Update(latency);
				if (shieldTime > 0)
					shieldTime -= latency;

				if (shieldTime <= 0)
				{
					// время защиты от повреждения вышло
					// восстанавливаем цвет
					red = 1.0;
					green = 1.0;
					blue = 1.0;
					alpha = 1.0;
				}
			}
		}

		void DamageInflict(float damage)
		{
			// если не включена защита от повреждений
			if (shieldTime <= 0)
			{
				// цвет красный
				red = 1.0;
				green = .0;
				blue = .0;
				alpha = 1.0;
				shieldTime = shieldRate;
				// наносим повреждение
				health -= damage;
				SetHealth(this->health);
			}
		}

		void SetHealth(float health)
		{
			if (health < 0)
			{
				// умерли
				health = 0;
				flags |= GOF_GHOST;
			}
			this->health = health;
			if (onUpdateHealth)
				// здоровье изменилось
				onUpdateHealth(this);
		}

		virtual void Contact(GameObject* target, const Vector2f& normal)
		{
			GameObject::Contact(target, normal);

			// при столкновении наносим повреждения эквивалентное массе пилюли
			DamageInflict(target->mass);
		}

		// Выстрел
		virtual void Shoot(GameObject* object)
		{
			FlyingShooter::Shoot(object);

			Photon* photon = dynamic_cast<Photon*> (object);
			photon->onKillPill = onKillPill;

			// случайный цвет
			switch (Random() & 3)
			{
				default:
				case 0:
					// белый
					object->red = 1.0;
					object->green = 1.0;
					object->blue = 1.0;
					break;
				case 1:
					// розовый
					object->red = 1.0;
					object->green = 0.0;
					object->blue = 1.0;
					break;
				case 2:
					// зелёный
					object->red = 0.0;
					object->green = 1.0;
					object->blue = 0.0;
					break;
				case 3:
					// жёлтый
					object->red = 1.0;
					object->green = 1.0;
					object->blue = 0.0;
					break;
			}
		};
};

#endif /* BACTERIA_H_ */
