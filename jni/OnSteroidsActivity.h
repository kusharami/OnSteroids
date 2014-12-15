/*
 * OnSteroidsActivity.h
 *
 *  Activity игры.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef ONSTEROIDSACTIVITY_H_
#define ONSTEROIDSACTIVITY_H_

#include <jni.h>
#include "AndroidMain.h"

#include "engine/GLActivity.h"
#include "engine/ShapesBuffer.h"
#include "engine/List.h"
#include "engine/GameObjects.h"
#include "engine/RectBuffer.h"

#include "engine/TouchControllerObject.h"
#include "engine/TriggerController.h"
#include "engine/TriggerButton.h"

#include "objects/ShooterObject.h"
#include "objects/MotorObject.h"

#include "game/Pill.h"
#include "game/Bacteria.h"
#include "game/Wheel.h"

#define STATE_ERROR -1
#define STATE_INITIALIZE 0
#define STATE_GAMESTART 1
#define STATE_RUN 2

class OnSteroidsActivity: public GLActivity
{
	public:
		// объекты
		GameObjects* objects;

		// цветные прямоугольники
		RectBuffer* rectBuffer;

		// цифры
		ShapesBuffer* numBuffer;

		// старый счёт
		uint32_t oldScore;

		// новый счёт
		uint32_t score;

		OnSteroidsActivity(JNIEnv* env, jobject jActivity);

		virtual ~OnSteroidsActivity()
		{
			delete numBuffer;
			if (numbers)
			{
				for (int i = 0; i < 10; i++)
					delete numbers[i];
				delete[] numbers;
			}
			delete touchEvents;
			delete touchEventsBackup;
			delete rectBuffer;
			delete objects;
			delete pillShape;
			delete predShape;
			delete pwhiteShape;
			delete shipShape;
			delete wheelShape;
			delete photonShape;
			delete atkbtnShape;
			delete accbtnShape;
		};

		// состояние
		int state;

		// бактерия
		Bacteria* bacteria;

		// руль
		TouchControllerObject* wheel;

		// кнопка "выстрел"
		TouchControllerObject* btn1;

		// кнопка "поехали"
		TouchControllerObject* btn2;

		// Пилюлька появись
		Pill* SpawnPillRandom();

	protected:
		// кол-во пилюль на экране
		int pillCount;
		// время до следующего появления
		float appearTime;

		// картинки
		ShapeF** numbers; // цифры
		ShapeF* shipShape; // бактерия
		ShapeF* pillShape; // пилюля
		ShapeF* predShape; // красная половинка
		ShapeF* pwhiteShape; // белая половинка
		ShapeF* wheelShape; // руль
		ShapeF* photonShape; // фотон
		ShapeF* atkbtnShape; // кнопка "выстрел"
		ShapeF* accbtnShape; // кнопка "поехали"

		// Счёт в буффер вывода на экран
		void ScoreUpdate();

		// обновить состояние
		virtual void Update(double latency);

		// вывести всё на экран
		virtual void Render();

		// рабочая ширина экрана
		virtual int GetDrawWidth();

		// прикосновение
		virtual void TouchEvent(TouchEventRec *event);

		// обновляются ли прикосновения в данный момент
		volatile bool touchEventsBusy;
		// рабочий список прикосновений
		List* touchEvents;
		// обновляемый список прикосновений
		List* touchEventsBackup;

		// Бактерия появись
		Bacteria* SpawnBacteria(float direction, float x, float y);

		// Руль появись
		Wheel* SpawnWheel(float direction, float x, float y);

		// Кнопка появись
		TouchControllerObject* SpawnButton(float x, float y, ShapeF* shape, TouchControllerObject* newObj);

		// Инициализация
		void Initialize();
};

// Контроллер выстрелов
class ShootController: public TriggerController<ShooterObject, &ShooterObject::shooting> {};
// Контроллер двигателя
class MotorController: public TriggerController<MotorObject, &MotorObject::accelerating> {};

// Кнопка "выстрел"
class ShootButton: public TriggerButton<ShootController> {};
// Кнопка "поехали"
class AccelButton: public TriggerButton<MotorController> {};

#endif /* ONSTEROIDSACTIVITY_H_ */
