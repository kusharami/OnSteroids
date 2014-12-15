/*
 * OnSteroidsActivity.cpp
 *
 *  Activity ����.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#include "OnSteroidsActivity.h"

#include "engine/AssetManager.h"

// ������������� ������
static const Vector2f photonPoints[] =
{
{5.0, 0.0},
{9.0, 0.0},
{14.0, 5.0},
{14.0, 9.0},
{9.0, 14.0},
{5.0, 14.0},
{0.0, 9.0},
{0, 5.0}

};

// ������������� ������
static const Vector2f pillPoints[] =
{
{15.0, 0.0},
{30.0, 0.0},
{46.0, 16.0},
{46.0, 73.0},
{30.0, 94.0},
{15.0, 94.0},
{0.0, 73.0},
{0, 16.0}

};

// ������������� ������� ���������
static const Vector2f pillRedPoints[] =
{
{15.0, 0.0},
{30.0, 0.0},
{46.0, 16.0},
{46.0, 48.0},
{0.0, 48.0},
{0, 16.0}

};

// ������������� ����� ���������
static const Vector2f pillWhitePoints[] =
{
{0.0, 48.0 - 48.0},
{46.0, 48.0 - 48.0},
{46.0, 73.0 - 48.0},
{30.0, 94.0 - 48.0},
{15.0, 94.0 - 48.0},
{0.0, 73.0 - 48.0}

};

// ������������� ��������
static const Vector2f shipPoints[] =
{
{15.0, 1.0},
{24.0, 1.0},
{24.0, 9.0},
{31.0, 21.0},
{37.0, 24.0},
{35.0, 40.0},
{32.0, 43.0},
{30.0, 48.0},
{19.0, 54.0},
{17.0, 54.0},
{11.0, 47.0},
{7.0, 45.0},
{7.0, 41.0},
{4.0, 40.0},
{0.0, 37.0},
{0.0, 35.0},
{3.0, 31.0},
{2.0, 23.0},
{5.0, 20.0},
{9.0, 20.0},
{11.0, 10.0},
{15.0, 10.0}};

void FreeTouchEvent(TouchEventRec* event)
{
	delete event;
}

int OnSteroidsActivity::GetDrawWidth()
{
	// ������� ������ ������ ������� �� �������� ������� ������ � ��������� ������
	float s = (float) viewHeight / (float) drawHeight;
	return (float) viewWidth / s;
}

OnSteroidsActivity::OnSteroidsActivity(JNIEnv* env, jobject jActivity) :
	GLActivity(env, jActivity),
	appearTime(0),
	touchEventsBusy(true),
	bacteria(NULL),
	pillCount(0),
	btn1(NULL),
	btn2(NULL),
	wheel(NULL),
	score(0),
	oldScore(~0),
	wheelShape(NULL),
	numBuffer(NULL),
	pillShape(NULL),
	objects(NULL),
	photonShape(NULL),
	pwhiteShape(NULL),
	numbers(NULL),
	atkbtnShape(NULL),
	predShape(NULL),
	shipShape(NULL),
	accbtnShape(NULL),
	rectBuffer(NULL),
	state(STATE_INITIALIZE)
{
	touchEvents = new List((FreeObjectProc*) &FreeTouchEvent);
	touchEventsBackup = new List((FreeObjectProc*) &FreeTouchEvent);
	touchEventsBusy = false;

	// ������� ������ ������
	drawHeight = 400;
	Initialize();
}

void OnSteroidsActivity::ScoreUpdate()
{
	if (score != oldScore)
	{
		// ���� ���� ���������
		oldScore = score;

		char str[11];
		// �������� ������ � �������
		snprintf(str, sizeof(str), "%u", score);

		// ������� ������ ���������
		numBuffer->Prepare();

		int len = strlen(str);
		int width = len * 40;
		// ������������ �����
		float x = (this->width - width) / 2.0;
		float y = height - 8;
		char c;
		char* s = str;
		while ((c = *s++))
		{
			// ����� ����� � ������
			numBuffer->Put(numbers[c - '0'], x, y);
			x += 40;
		}
	}
}

void OnSteroidsActivity::Initialize()
{
	// �������� �������� �� �����
	AssetFile* file = assets->Open("gfx/shapes.png");
	try
	{
		// ������ �����
		int size = file->Size();
		// ������ ��� �����
		void* buf = malloc(size);
		try
		{
			// ��������� ���� � ������
			file->Read(buf, size);

			// ��������� �������� �� ������� � ������ �� �� ��������
			Texture* texture = this->AddBitmapTexture(
				bitmapFactory->Decode(buf, size), GL_NEAREST, GL_LINEAR, true);

			// ����� ������ ���������������
			(rectBuffer = new RectBuffer())->Init(1);

			// ����� ������ �������� - 24 ��������. 2 ����������
			objects = new GameObjects(this, texture, 24, 2, true);

			// ������
			(pillShape = new ShapeF())->Set(-(46.0 / 2.0), -(95.0 / 2.0), 0, 0,
				46, 95, texture->width, texture->height, &pillPoints[0],
				sizeof(pillPoints) / sizeof(Vector2f));

			// ������� ���������
			(predShape = new ShapeF())->Set(-(46.0 / 2.0), -(48.0 / 2.0), 0, 0,
				46, 48, texture->width, texture->height, &pillRedPoints[0],
				sizeof(pillRedPoints) / sizeof(Vector2f));

			// ����� ���������
			(pwhiteShape = new ShapeF())->Set(-(46.0 / 2.0), -(47.0 / 2.0), 0,
				48, 46, 47, texture->width, texture->height,
				&pillWhitePoints[0],
				sizeof(pillWhitePoints) / sizeof(Vector2f));

			// ��������
			(shipShape = new ShapeF())->Set(-19, -29, 48, 0, 38, 55,
				texture->width, texture->height, &shipPoints[0],
				sizeof(shipPoints) / sizeof(Vector2f));

			// ����
			(wheelShape = new ShapeF())->Set(-(100 / 2), -(100 / 2 + 20), 100,
				0, 100, 120, texture->width, texture->height, NULL, 0);

			// �����
			(photonShape = new ShapeF())->Set(-(14.0 / 2.0), -(14.0 / 2.0), 60,
				60, 14, 14, texture->width, texture->height, &photonPoints[0],
				sizeof(photonPoints) / sizeof(Vector2f));

			// �������
			(atkbtnShape = new ShapeF())->Set(-(80.0 / 2.0), -(80.0 / 2.0), 204,
				0, 80, 80, texture->width, texture->height, NULL, 0);

			// �������
			(accbtnShape = new ShapeF())->Set(-(80.0 / 2.0), -(80.0 / 2.0), 288,
				0, 80, 80, texture->width, texture->height, NULL, 0);

			// �����
			ShapeF** n = numbers = new ShapeF*[10];
			for (int x = 0; x < 40 * 10; x += 40)
				(*n++ = new ShapeF())->Set(0, -40, x, 160, 40, 40,
					texture->width, texture->height, NULL, 0);

			(numBuffer = new ShapesBuffer())->Init(texture, 9, false);

			// ���� ���� - �����������
			numBuffer->SetColor(0xAAAAFFAA);

			state = STATE_GAMESTART;
		} catch (void* e)
		{
			state = STATE_ERROR;
		}
		free(buf);
	} catch (void* e)
	{
		state = STATE_ERROR;
	}
	delete file;
}

void OnSteroidsActivity::Update(double latency)
{
	switch (state)
	{
		case STATE_ERROR:
			Terminate();
			break;

		case STATE_GAMESTART:
		{
			objects->Clear();
			// ������� 0
			pillCount = 0;
			// ������� �������
			objects->clientRect = Rect<float>(0, 0, width, height);

			// ���� �������
			wheel = SpawnWheel(0.0, 100, height - 100);

			// ������ "�������" �������
			btn1 = SpawnButton(width - 140, height - 60, atkbtnShape,
				new ShootButton());

			// ������ "�������" �������
			btn2 = SpawnButton(width - 80, height - 140, accbtnShape,
				new AccelButton());

			// �� ��������
			score = 0;
			oldScore = ~0;
			appearTime = 0;

			// �������� �������
			bacteria = SpawnBacteria(0.0, width / 2.0, height / 2.0);

			// ����� �������� ��� ��������
			wheel->TakeControl(bacteria);
			btn1->TakeControl(bacteria);
			btn2->TakeControl(bacteria);

			state = STATE_RUN;
			latency = 0;
		}

		case STATE_RUN:
		{
			appearTime -= latency;
			if (appearTime <= .0)
			{
				// ����� ��������
				appearTime = 800;

				// �� ������ ����� �� ������ (�������� ��������� �� �����)
				if (pillCount < 6)
				{
					// �������� �������
					SpawnPillRandom();
					pillCount++;
				}
			}

			// ������� ������������� ��� ������
			while (touchEventsBusy)
				;
			touchEventsBusy = true;
			touchEvents->Absorb(touchEventsBackup);
			touchEventsBusy = false;

			// ��������� �������� ��������
			objects->Update(latency);
			// ���� ��������
			ScoreUpdate();

			break;
		}
	}
}

// ������� �� �� �����
void OnSteroidsActivity::Render()
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	objects->shapes->Draw();

	numBuffer->Draw();

	glDisable(GL_TEXTURE_2D);

	rectBuffer->Draw();
	glDisable(GL_BLEND);
}

Pill* OnSteroidsActivity::SpawnPillRandom()
{
	if (bacteria == NULL)
		// ���� �������� ���, ����� �� ����������
		return NULL;

	Pill* pill;
	objects->Add((pill = new Pill()));

	pill->countPtr = &pillCount;
	pill->halfWhite = pwhiteShape;
	pill->halfRed = predShape;
	ShapeF* shape = pillShape;

	float x, y;
	// ���������� � ����� �� ������ ������
	switch (Random() & 3)
	{
		default:
		case 0:
			x = Random(width);
			y = -shape->size.y - shape->origin.y;
			break;

		case 1:
			x = Random(width);
			y = height - shape->origin.y;
			break;

		case 2:
			x = -shape->size.x - shape->origin.x;
			y = Random(height);
			break;
		case 3:
			x = width - shape->origin.x;
			y = Random(height);
			break;
	}

	// ������������ �� ��������
	float angle = atan2(bacteria->coord.y - y, bacteria->coord.x - x);

	// �� ��������� ��������� �� 100 �� 199 �������� � �������
	float speed = (Random(100) + 100.0) / 1000.0;
	pill->direction = DirectionVector<float>(angle);
	pill->SetSpeed(speed);

	float mx = pill->motion.x;

	// �������� �������� ������� �� ����, ��� ������� �����
	if (mx < 0 || (mx == 0 && pill->motion.y < 0))
		speed = -speed;

	pill->rotateMode = ROTATE_ENDLESS;
	pill->rotationSpeed = speed;
	pill->offscreenEvent = &ObjectOffscreenReappear;

	pill->coord.x = x;
	pill->coord.y = y;

	Vector2f scale;
	// ��������� ������ �� 0.5 �� 1
	scale.x = (Random(5) + 5) * .1;
	scale.y = scale.x;
	pill->restitution = 1;
	pill->friction = 0;
	pill->SetShape(shape, &scale, NULL);

	// ����� ������� �� �������
	pill->SetMass(scale.x * 200);

	return pill;
}

static void BacteriaDeath(Bacteria* sender)
{
	// ������ ������
	OnSteroidsActivity* a = (OnSteroidsActivity*) sender->owner->owner;
	sender->Recycle();
	a->bacteria = NULL;
	a->wheel->TakeControl(NULL);
	a->btn1->TakeControl(NULL);
	a->btn2->TakeControl(NULL);

}

static void UpdateBacteriaHealth(Bacteria* sender)
{
	// �������� ��������
	RectBuffer* rb = ((OnSteroidsActivity*) sender->owner->owner)->rectBuffer;
	rb->Prepare();
	if (sender->health > 0)
		rb->Put(10, 10, sender->health / 2, 10, 0xAAFF0000);
}

static void KillPill(GameObject* pill)
{
	// ����� ������, �������� ����
	uint32_t& score = ((OnSteroidsActivity*) pill->owner->owner)->score;

	if (pill->id == PILL_ID)
		score += pill->mass / 3;
	else
		score += pill->mass;

	if (score > 999999999)
		score = 999999999;
}

Bacteria* OnSteroidsActivity::SpawnBacteria(float direction, float x, float y)
{
	Bacteria* b;
	objects->Add((b = new Bacteria()));

	// ��� ���������
	b->deathStep = 0.005;
	// ������������ ������� ����������� � �������� � ������
	b->shieldRate = 500.0;
	b->onKillPill = &KillPill;
	b->onDeath = &BacteriaDeath;
	b->onUpdateHealth = &UpdateBacteriaHealth;
	b->SetHealth(1000);
	b->coord.x = x;
	b->coord.y = y;
	b->restitution = .3;
	b->maxRotationSpeed = 500.0 / 1000.0;
	b->rotationSpeed = 300.0 / 1000.0;
	b->priority = 1;
	b->offscreenEvent = &ObjectOffscreenReappear;
	b->SetAngle(direction);
	b->SetShape(shipShape, Vector2f(1.0, 1.0));
	b->SetMass(300);

	b->SetBarrelOrigin(0, -25.0);
	b->bulletOffscreenEvent = &ObjectOffscreenReappear;
	b->bulletShape = photonShape;

	b->bulletSpeed = 400.0 / 1000.0;
	b->bulletTime = 400.0;
	b->bulletFadeStep = 0.01;
	b->reloadRate = 200.0;

	b->direction = DirectionVectorD<float>(direction);
	b->accelerationStep = 0.2 / 1000.0;
	b->maxSpeed = 200.0 / 1000.0;
	b->brakingStep = 0.1 / 1000.0;
	return b;
}

// ����
Wheel* OnSteroidsActivity::SpawnWheel(float direction, float x, float y)
{
	Wheel* wheel;
	objects->Add((wheel = new Wheel()));

	wheel->touchEvents = touchEvents;
	wheel->coord.x = x;
	wheel->coord.y = y;
	wheel->priority = 1;
	ShapeF* shape = wheelShape;
	float halfW = shape->size.x / 2.0;
	float dblW = shape->size.x * 2.0;
	wheel->SetBounds(x + shape->origin.x - halfW, y + shape->origin.x - halfW,
		dblW, dblW);
	wheel->SetAngle(direction);

	wheel->SetShape(shape, Vector2f(1.0, 1.0));
	return wheel;
}

void OnSteroidsActivity::TouchEvent(TouchEventRec* event)
{
	if (bacteria == NULL)
	{	// ���� �������� ������ ���.
		// �� ������� �� �����, ���� ���������� ������
		if (event->action == ACTION_DOWN)
			state = STATE_GAMESTART;

	} else
	{
		TouchEventRec* rec = new TouchEventRec;
		memcpy(rec, event, sizeof(TouchEventRec));
		TouchPointer* ptr = &rec->pointers[0];
		TouchPointer* end = &ptr[rec->pointerCount];
		while (ptr < end)
		{
			ptr->x /= scaleX;
			ptr->y /= scaleY;
			ptr++;
		}

		// �������� ������������� � ������
		while (touchEventsBusy)
			;
		touchEventsBusy = true;
		touchEventsBackup->Add()->object = rec;
		touchEventsBusy = false;
	}
}

// ��������
TouchControllerObject* OnSteroidsActivity::SpawnButton(float x, float y,
	ShapeF* shape, TouchControllerObject* newObj)
{
	objects->Add(newObj);

	newObj->touchEvents = touchEvents;
	newObj->coord.x = x;
	newObj->coord.y = y;
	newObj->priority = 1;

	newObj->SetBounds(x + shape->origin.x, y + shape->origin.y, shape->size.x,
		shape->size.y);
	newObj->SetShape(shape, Vector2f(1.0, 1.0));
	return newObj;
}
