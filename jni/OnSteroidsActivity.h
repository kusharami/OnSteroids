/*
 * OnSteroidsActivity.h
 *
 *  Activity ����.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
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
		// �������
		GameObjects* objects;

		// ������� ��������������
		RectBuffer* rectBuffer;

		// �����
		ShapesBuffer* numBuffer;

		// ������ ����
		uint32_t oldScore;

		// ����� ����
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

		// ���������
		int state;

		// ��������
		Bacteria* bacteria;

		// ����
		TouchControllerObject* wheel;

		// ������ "�������"
		TouchControllerObject* btn1;

		// ������ "�������"
		TouchControllerObject* btn2;

		// �������� �������
		Pill* SpawnPillRandom();

	protected:
		// ���-�� ������ �� ������
		int pillCount;
		// ����� �� ���������� ���������
		float appearTime;

		// ��������
		ShapeF** numbers; // �����
		ShapeF* shipShape; // ��������
		ShapeF* pillShape; // ������
		ShapeF* predShape; // ������� ���������
		ShapeF* pwhiteShape; // ����� ���������
		ShapeF* wheelShape; // ����
		ShapeF* photonShape; // �����
		ShapeF* atkbtnShape; // ������ "�������"
		ShapeF* accbtnShape; // ������ "�������"

		// ���� � ������ ������ �� �����
		void ScoreUpdate();

		// �������� ���������
		virtual void Update(double latency);

		// ������� �� �� �����
		virtual void Render();

		// ������� ������ ������
		virtual int GetDrawWidth();

		// �������������
		virtual void TouchEvent(TouchEventRec *event);

		// ����������� �� ������������� � ������ ������
		volatile bool touchEventsBusy;
		// ������� ������ �������������
		List* touchEvents;
		// ����������� ������ �������������
		List* touchEventsBackup;

		// �������� �������
		Bacteria* SpawnBacteria(float direction, float x, float y);

		// ���� �������
		Wheel* SpawnWheel(float direction, float x, float y);

		// ������ �������
		TouchControllerObject* SpawnButton(float x, float y, ShapeF* shape, TouchControllerObject* newObj);

		// �������������
		void Initialize();
};

// ���������� ���������
class ShootController: public TriggerController<ShooterObject, &ShooterObject::shooting> {};
// ���������� ���������
class MotorController: public TriggerController<MotorObject, &MotorObject::accelerating> {};

// ������ "�������"
class ShootButton: public TriggerButton<ShootController> {};
// ������ "�������"
class AccelButton: public TriggerButton<MotorController> {};

#endif /* ONSTEROIDSACTIVITY_H_ */
