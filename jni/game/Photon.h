/*
 * Photon.h
 *
 *  ������� ������ "�����".
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef PHOTON_H_
#define PHOTON_H_

#include "engine/GameObjects.h"
#include "objects/Bullet.h"
#include "game/Pill.h"

#define PHOTON_ID 4

// ��������� ������� ����������� ������
typedef void KillPillProc(GameObject* pill);

class Photon: public Bullet
{
	public:
		// ��������� �� ������� ����������� ������
		KillPillProc* onKillPill;

		Photon(): Bullet(), onKillPill(NULL) { id = PHOTON_ID; };

		virtual void Contact(GameObject* target, const Vector2f& normal)
		{
			// ���� ��� �� ���������� � �� ������� �� 25%. � ���� - ������
			if (!blowupState && alpha > .25 && (target->id == PILL_ID || target->id == HALF_ID))
			{
				// ���������� �� �����, �������� �������� � 3 ����
				float sx = scale.x * 3;
				SetShape(shape, scale * 3);

				// ������ �� ������������� � ���������
				flags |= GOF_GHOST;

				// �������������
				SetSpeed(0);

				// ��������� ������
				alpha = 1;
				// ����������
				blowupState = true;
				flightCounter = 0;

				if (onKillPill)
					// ������� ����������� ������
					onKillPill(target);

				if (target->id == HALF_ID)
					// ������� ������
					target->Recycle();
				else
					// ����� ������ �� ���
					((Pill*) target)->SplitUp();
			}
		}
};


#endif /* PHOTON_H_ */
