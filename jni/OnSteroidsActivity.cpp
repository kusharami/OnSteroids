/*
 * OnSteroidsActivity.cpp
 *
 *  Activity игры.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#include "OnSteroidsActivity.h"

#include "engine/AssetManager.h"

// многоугольник фотона
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

// многоугольник пилюли
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

// многоугольник красной половинки
static const Vector2f pillRedPoints[] =
{
{15.0, 0.0},
{30.0, 0.0},
{46.0, 16.0},
{46.0, 48.0},
{0.0, 48.0},
{0, 16.0}

};

// многоугольник белой половинки
static const Vector2f pillWhitePoints[] =
{
{0.0, 48.0 - 48.0},
{46.0, 48.0 - 48.0},
{46.0, 73.0 - 48.0},
{30.0, 94.0 - 48.0},
{15.0, 94.0 - 48.0},
{0.0, 73.0 - 48.0}

};

// многоугольник бактерии
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
	// рабочая ширина экрана зависит от масштаба рабочей высоты к настоящей высоте
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

	// рабочая высота экрана
	drawHeight = 400;
	Initialize();
}

void OnSteroidsActivity::ScoreUpdate()
{
	if (score != oldScore)
	{
		// если счёт обновился
		oldScore = score;

		char str[11];
		// получаем строку с цифрами
		snprintf(str, sizeof(str), "%u", score);

		// готовим буффер отрисовки
		numBuffer->Prepare();

		int len = strlen(str);
		int width = len * 40;
		// координируем вывод
		float x = (this->width - width) / 2.0;
		float y = height - 8;
		char c;
		char* s = str;
		while ((c = *s++))
		{
			// кладём цифру в буффер
			numBuffer->Put(numbers[c - '0'], x, y);
			x += 40;
		}
	}
}

void OnSteroidsActivity::Initialize()
{
	// загужаем картинку из файла
	AssetFile* file = assets->Open("gfx/shapes.png");
	try
	{
		// размер файла
		int size = file->Size();
		// буффер для файла
		void* buf = malloc(size);
		try
		{
			// загружаем файл в буффер
			file->Read(buf, size);

			// загружаем картинку из буффера и делаем из неё текстуру
			Texture* texture = this->AddBitmapTexture(
				bitmapFactory->Decode(buf, size), GL_NEAREST, GL_LINEAR, true);

			// новый буффер прямоугольников
			(rectBuffer = new RectBuffer())->Init(1);

			// новый буффер объектов - 24 максимум. 2 приоритета
			objects = new GameObjects(this, texture, 24, 2, true);

			// пилюля
			(pillShape = new ShapeF())->Set(-(46.0 / 2.0), -(95.0 / 2.0), 0, 0,
				46, 95, texture->width, texture->height, &pillPoints[0],
				sizeof(pillPoints) / sizeof(Vector2f));

			// красная половинка
			(predShape = new ShapeF())->Set(-(46.0 / 2.0), -(48.0 / 2.0), 0, 0,
				46, 48, texture->width, texture->height, &pillRedPoints[0],
				sizeof(pillRedPoints) / sizeof(Vector2f));

			// белая половинка
			(pwhiteShape = new ShapeF())->Set(-(46.0 / 2.0), -(47.0 / 2.0), 0,
				48, 46, 47, texture->width, texture->height,
				&pillWhitePoints[0],
				sizeof(pillWhitePoints) / sizeof(Vector2f));

			// бактерия
			(shipShape = new ShapeF())->Set(-19, -29, 48, 0, 38, 55,
				texture->width, texture->height, &shipPoints[0],
				sizeof(shipPoints) / sizeof(Vector2f));

			// руль
			(wheelShape = new ShapeF())->Set(-(100 / 2), -(100 / 2 + 20), 100,
				0, 100, 120, texture->width, texture->height, NULL, 0);

			// фотон
			(photonShape = new ShapeF())->Set(-(14.0 / 2.0), -(14.0 / 2.0), 60,
				60, 14, 14, texture->width, texture->height, &photonPoints[0],
				sizeof(photonPoints) / sizeof(Vector2f));

			// выстрел
			(atkbtnShape = new ShapeF())->Set(-(80.0 / 2.0), -(80.0 / 2.0), 204,
				0, 80, 80, texture->width, texture->height, NULL, 0);

			// поехали
			(accbtnShape = new ShapeF())->Set(-(80.0 / 2.0), -(80.0 / 2.0), 288,
				0, 80, 80, texture->width, texture->height, NULL, 0);

			// цифры
			ShapeF** n = numbers = new ShapeF*[10];
			for (int x = 0; x < 40 * 10; x += 40)
				(*n++ = new ShapeF())->Set(0, -40, x, 160, 40, 40,
					texture->width, texture->height, NULL, 0);

			(numBuffer = new ShapesBuffer())->Init(texture, 9, false);

			// цвет цифр - зеленоватый
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
			// пилюлек 0
			pillCount = 0;
			// рабочая область
			objects->clientRect = Rect<float>(0, 0, width, height);

			// Руль появись
			wheel = SpawnWheel(0.0, 100, height - 100);

			// Кнопка "выстрел" появись
			btn1 = SpawnButton(width - 140, height - 60, atkbtnShape,
				new ShootButton());

			// Кнопка "поехали" появись
			btn2 = SpawnButton(width - 80, height - 140, accbtnShape,
				new AccelButton());

			// всё обнуляем
			score = 0;
			oldScore = ~0;
			appearTime = 0;

			// Бактерия появись
			bacteria = SpawnBacteria(0.0, width / 2.0, height / 2.0);

			// Взять бактерию под контроль
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
				// новые пилюльки
				appearTime = 800;

				// не больше шести на экране (полвинки считаются за целые)
				if (pillCount < 6)
				{
					// Пилюлька появись
					SpawnPillRandom();
					pillCount++;
				}
			}

			// события прикосновений для работы
			while (touchEventsBusy)
				;
			touchEventsBusy = true;
			touchEvents->Absorb(touchEventsBackup);
			touchEventsBusy = false;

			// состояние объектов обновить
			objects->Update(latency);
			// счёт обновить
			ScoreUpdate();

			break;
		}
	}
}

// выводим всё на экран
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
		// если бактерии нет, новые не появляются
		return NULL;

	Pill* pill;
	objects->Add((pill = new Pill()));

	pill->countPtr = &pillCount;
	pill->halfWhite = pwhiteShape;
	pill->halfRed = predShape;
	ShapeF* shape = pillShape;

	float x, y;
	// появляется с одной из четырёх сторон
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

	// направляется на бактерию
	float angle = atan2(bacteria->coord.y - y, bacteria->coord.x - x);

	// со случайной скоростью от 100 до 199 пикселей в секунду
	float speed = (Random(100) + 100.0) / 1000.0;
	pill->direction = DirectionVector<float>(angle);
	pill->SetSpeed(speed);

	float mx = pill->motion.x;

	// скорость вращения зависит от угла, под которым летит
	if (mx < 0 || (mx == 0 && pill->motion.y < 0))
		speed = -speed;

	pill->rotateMode = ROTATE_ENDLESS;
	pill->rotationSpeed = speed;
	pill->offscreenEvent = &ObjectOffscreenReappear;

	pill->coord.x = x;
	pill->coord.y = y;

	Vector2f scale;
	// случайный размер от 0.5 до 1
	scale.x = (Random(5) + 5) * .1;
	scale.y = scale.x;
	pill->restitution = 1;
	pill->friction = 0;
	pill->SetShape(shape, &scale, NULL);

	// масса зависит от размера
	pill->SetMass(scale.x * 200);

	return pill;
}

static void BacteriaDeath(Bacteria* sender)
{
	// смерть пришла
	OnSteroidsActivity* a = (OnSteroidsActivity*) sender->owner->owner;
	sender->Recycle();
	a->bacteria = NULL;
	a->wheel->TakeControl(NULL);
	a->btn1->TakeControl(NULL);
	a->btn2->TakeControl(NULL);

}

static void UpdateBacteriaHealth(Bacteria* sender)
{
	// обновить здоровье
	RectBuffer* rb = ((OnSteroidsActivity*) sender->owner->owner)->rectBuffer;
	rb->Prepare();
	if (sender->health > 0)
		rb->Put(10, 10, sender->health / 2, 10, 0xAAFF0000);
}

static void KillPill(GameObject* pill)
{
	// убили пилюлю, получили очки
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

	// шаг затухания
	b->deathStep = 0.005;
	// максимальная частота повреждений в пикселях в секнду
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

// руль
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
	{	// если бактерии больше нет.
		// по нажатию на экран, игра начинается заново
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

		// добавить прикосновение в список
		while (touchEventsBusy)
			;
		touchEventsBusy = true;
		touchEventsBackup->Add()->object = rec;
		touchEventsBusy = false;
	}
}

// кнопочка
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
