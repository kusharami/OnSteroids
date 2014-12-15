/*
 * Pill.h
 *
 *  Игровой объект "пилюля".
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef PILL_H_
#define PILL_H_

#include "engine/GameObjects.h"

#define HALF_ID 2
#define PILL_ID 3


// половинка пилюли
class HalfPill: public GameObject
{
	public:
		// указатель на счётчик пилюль
		int* countPtr;

		HalfPill() : GameObject(), countPtr(NULL) { id = HALF_ID; };

		virtual void Recycle()
		{
			// удаляя объект, уменьшаем счётчик
			if (countPtr != NULL)
				(*countPtr)--;

			GameObject::Recycle();
		}

};

// целая пилюля
class Pill: public HalfPill
{
	public:
		// картинка красной половинки
		ShapeF* halfRed;
		// картинка белой половинки
		ShapeF* halfWhite;
		Pill() : HalfPill(), halfRed(NULL), halfWhite(NULL) { id = PILL_ID; };

		void SplitUp()
		{
			// удалить большую
			Recycle();

			// создать половинки
			HalfPill* p1 = new HalfPill();
			HalfPill* p2 = new HalfPill();

			// посчитать координаты
			float hy = shape->origin.y / 2.0;
			p1->coord = Vector2f(0, hy).Rotate(angleVector) + coord;
			p2->coord = Vector2f(0, -hy).Rotate(angleVector) + coord;

			// инициировать данные
			p1->countPtr = p2->countPtr = countPtr;
			p1->rotateMode = p2->rotateMode = ROTATE_ENDLESS;
			p1->rotationSpeed = p2->rotationSpeed = 1.0;
			p1->direction = p2->direction = direction;
			p1->motion = p2->motion = motion;
			p1->restitution = p2->restitution = restitution;
			p1->offscreenEvent = p2->offscreenEvent = offscreenEvent;
			p1->priority = p2->priority = priority;

			// повернуть
			p1->SetAngle(angle);
			p2->SetAngle(angle);

			// установить картинки
			p1->SetShape(halfRed, &scale, NULL);
			p2->SetShape(halfWhite, &scale, NULL);

			// установить половинную массу
			float hmass = mass / 2.0;
			p1->SetMass(hmass);
			p2->SetMass(hmass);

			// Плюс две к счётчику пилюль
			*countPtr += 2;

			// добавить к объектам
			owner->Add(p1);
			owner->Add(p2);
		}

};

#endif /* PILL_H_ */
