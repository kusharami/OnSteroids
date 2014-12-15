/*
 * Bacteria.h
 *
 *  ������� ������ "��������".
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef BACTERIA_H_
#define BACTERIA_H_

#include "objects/FlyingShooter.h"
#include "game/Photon.h"

#define BACTERIA_ID 1

class Bacteria;

// ��������� ������� �������������� � ���������
typedef void BactUpdateProc(Bacteria* sender);

class Bacteria: public FlyingShooter
{
	public:
		// ��� ���������
		float deathStep;

		// ����� ������ �� �����������
		float shieldTime;

		// ������������ ������� �����������
		float shieldRate;

		// ��������
		float health;

		// �� ��������� ��������
		BactUpdateProc* onUpdateHealth;

		// �����
		BactUpdateProc* onDeath;

		// ���������� ��������
		KillPillProc* onKillPill;

		Bacteria() : FlyingShooter(), health(100), onUpdateHealth(NULL), onDeath(NULL),
				shieldTime(0), shieldRate(0), deathStep(0), onKillPill(NULL) { id = BACTERIA_ID; };

		// ����� �����
		virtual GameObject* CreateProjectile()
		{
			return new Photon();
		}

		virtual void Update(double latency)
		{
			if (health <= 0)
			{
				// ���� ����, ��������
				alpha -= deathStep * latency;
				if (alpha <= 0)
				{
					if (onDeath != NULL)
						// ������������ ����
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
					// ����� ������ �� ����������� �����
					// ��������������� ����
					red = 1.0;
					green = 1.0;
					blue = 1.0;
					alpha = 1.0;
				}
			}
		}

		void DamageInflict(float damage)
		{
			// ���� �� �������� ������ �� �����������
			if (shieldTime <= 0)
			{
				// ���� �������
				red = 1.0;
				green = .0;
				blue = .0;
				alpha = 1.0;
				shieldTime = shieldRate;
				// ������� �����������
				health -= damage;
				SetHealth(this->health);
			}
		}

		void SetHealth(float health)
		{
			if (health < 0)
			{
				// ������
				health = 0;
				flags |= GOF_GHOST;
			}
			this->health = health;
			if (onUpdateHealth)
				// �������� ����������
				onUpdateHealth(this);
		}

		virtual void Contact(GameObject* target, const Vector2f& normal)
		{
			GameObject::Contact(target, normal);

			// ��� ������������ ������� ����������� ������������� ����� ������
			DamageInflict(target->mass);
		}

		// �������
		virtual void Shoot(GameObject* object)
		{
			FlyingShooter::Shoot(object);

			Photon* photon = dynamic_cast<Photon*> (object);
			photon->onKillPill = onKillPill;

			// ��������� ����
			switch (Random() & 3)
			{
				default:
				case 0:
					// �����
					object->red = 1.0;
					object->green = 1.0;
					object->blue = 1.0;
					break;
				case 1:
					// �������
					object->red = 1.0;
					object->green = 0.0;
					object->blue = 1.0;
					break;
				case 2:
					// ������
					object->red = 0.0;
					object->green = 1.0;
					object->blue = 0.0;
					break;
				case 3:
					// �����
					object->red = 1.0;
					object->green = 1.0;
					object->blue = 0.0;
					break;
			}
		};
};

#endif /* BACTERIA_H_ */
