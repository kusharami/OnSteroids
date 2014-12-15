/*
 * Photon.h
 *
 *  Игровой объект "фотон".
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef PHOTON_H_
#define PHOTON_H_

#include "engine/GameObjects.h"
#include "objects/Bullet.h"
#include "game/Pill.h"

#define PHOTON_ID 4

// описатель функции уничтожения пилюли
typedef void KillPillProc(GameObject* pill);

class Photon: public Bullet
{
	public:
		// указатель на функцию уничтожения пилюли
		KillPillProc* onKillPill;

		Photon(): Bullet(), onKillPill(NULL) { id = PHOTON_ID; };

		virtual void Contact(GameObject* target, const Vector2f& normal)
		{
			// если ещё не взрывается и не затухла до 25%. и цель - пилюля
			if (!blowupState && alpha > .25 && (target->id == PILL_ID || target->id == HALF_ID))
			{
				// превращаем во взрыв, увеличив картинку в 3 раза
				float sx = scale.x * 3;
				SetShape(shape, scale * 3);

				// больше не соприкасается с объектами
				flags |= GOF_GHOST;

				// останавливаем
				SetSpeed(0);

				// видимость полная
				alpha = 1;
				// взрывается
				blowupState = true;
				flightCounter = 0;

				if (onKillPill)
					// событие уничтожения пилюли
					onKillPill(target);

				if (target->id == HALF_ID)
					// удаляем пилюлю
					target->Recycle();
				else
					// делим пилюлю на две
					((Pill*) target)->SplitUp();
			}
		}
};


#endif /* PHOTON_H_ */
