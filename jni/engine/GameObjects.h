/*
 * GameObjects.h
 *
 *  Классы для хранения и вззаимодействия игровых объектов.
 *
 *  Дата создания: 04.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef GAMEOBJECTS_H_
#define GAMEOBJECTS_H_

#include <GLES/gl.h>
#include "GLScreen.h"
#include "Shape.h"
#include "ShapesBuffer.h"
#include "List.h"
#include "Vector2.h"
#include "Utils.h"
#include "Rect.h"

class GameObject;

typedef void OffscreenEvent(GameObject* object);

void FreeGameObject(GameObject* object);

// удалить объект, если он вышел за границы экрана
void ObjectOffscreenRecycle(GameObject* object);

// объект появляется с другой стороны экрана, если он вышел за границы
void ObjectOffscreenReappear(GameObject* object);

// не вращать объект
#define ROTATE_NO 0

// всегда вращать
#define ROTATE_ENDLESS 1

// вращать определённое время
#define ROTATE_GRADUAL 2

// повернуть сразу
#define ROTATE_INSTANT 3

class GameObjects: public List
{
	public:
		// Экран, владеющий объектами
		GLScreen* owner;

		// область видимости объектов
		Rect<float> clientRect;

		// корзина для объектов
		List* recycleBin;

		// буффер для вывода объектов на экран
		ShapesBufferEx* shapes;

		GameObjects(GLScreen* owner, Texture* texture, int maxObjects, int maxPriorities, bool colors);
		virtual ~GameObjects();

		// добавить объект
		GameObject* Add(GameObject* object);

		// обновить состояние всех объектов
		// latency - количество времени в миллисекундах, которое прошло с момента предыдущего вызова этого метода
		virtual void Update(double latency);

		// поместить объект в корзину
		void ObjectRecycle(GameObject* object);

		// вывод объетов на экран
		void Draw();

	private:
		// очистить корзину
		void RecycleBinClear();
};

// Флаги объекта

// видимый
#define GOF_VISIBLE 		(1 << 0)

// активный
#define GOF_ACTIVE 			(1 << 1)

// призрак (контакты не обрабатываются)
#define GOF_GHOST 			(1 << 2)

// *** //
class GameObject
{
	public:
		// Список, владеющий этим объектом
		GameObjects* owner;
		// Элемент списка, владеющий этим объектом
		ListItem* container;

		// буффер для вывода на экран
		ShapesBufferEx* shapes;

		// приоритет объекта на экране (ниже - впереди, выше - позади)
		int priority;

		// флаги объекта
		unsigned int flags;

		// идентификатор объекта, задаётся пользователем
		int id;

		// границы видимости объекта
		Rect<float> shapeBounds;

		// форма объекта
		ShapeF* shape;

		// масса объекта и её инвертированное значение
		float mass, invMass;

		// момент инерции
		float inertia;

		// трение
		float friction;

		// отталкивание
		float restitution;

		// максимальная скорость вращения
		float maxRotationSpeed;

		// текущая скорость вращения
		float rotationSpeed;

		// счётчик вращения. используется при rotateMode = ROTATE_GRADUAL
		float rotateCounter;

		// режим вращения: ROTATE_NO, ROTATE_ENDLESS, ROTATE_GRADUAL, ROTATE_INSTANT
		int rotateMode;

		// конечный угол наклона для ROTATE_GRADUAL
		float finalAngle;

		// угол наклона объекта в градусах. его текущее и старое значение
		float angle, oldAngle;

		// обновлять направление движения объекта при изменении угла наклона
		bool updateDirection;

		// вектор наклона
		Vector2f angleVector;

		// вектор направления
		Vector2f direction;

		// вектор движения
		Vector2f motion;

		// координаты объекта
		Vector2f coord;

		// масштаб объекта
		Vector2f scale;

		// цвет объекта и уровень его прозрачности
		GLfloat red, green, blue, alpha;

		// скорость объекта
		float speed;

		// событие при выходе за границы экрана
		OffscreenEvent* offscreenEvent;

		// прямоугольник для вывода формы на экран
		Poly<Vector2f>* activeRect;
		// прямоугольник для вывода формы на экран, смещённый по координатам coord
		Poly<Vector2f>* drawRect;

		// описание многоугольной формы объекта
		Poly<Vector2f>* activePoly;

		// описание многоугольной формы объекта, смещённый по координатам coord
		Poly<Vector2f>* drawPoly;

		GameObject() : owner(NULL), container(NULL), shapes(NULL), priority(0), id(0), speed(0), oldAngle(0),
				flags(GOF_ACTIVE | GOF_VISIBLE), offscreenEvent(NULL), internalShape(NULL), shape(NULL),
				shapeBounds(), red(1.0), green(1.0), blue(1.0), alpha(1.0), mass(0), inertia(0), friction(0),
				updateDirection(false), direction(), motion(), coord(), rotationSpeed(0), rotateCounter(0),
				rotateMode(ROTATE_NO), finalAngle(0), angle(0), angleVector(), restitution(0), invMass(0),
				maxRotationSpeed(1.0)
		{
			activeRect = new Poly<Vector2f>();
			activePoly = new Poly<Vector2f>();
			drawRect = new Poly<Vector2f>();
			drawPoly = new Poly<Vector2f>();
			activeRect->SetVertexCount(4);
			drawRect->SetVertexCount(4);
		};

		// Задать массу объекта
		void SetMass(float mass);

		// Задать цвет объекта в формате ARGB
		void SetColor(uint32_t color);

		// Задать форму объекта в определённом масштабе
		void SetShape(ShapeF* shape, const Vector2f& scale)
		{
			SetShape(shape, &scale, NULL);
		};

		// Задать форму объекта в определённом масштабе, с определённым вектором наклона
		void SetShape(ShapeF* shape, const Vector2f* scale, const Vector2f* direction);

		// Установка владельцев объекта
		void SetContainer(ListItem* container)
		{
			this->container = container;
			container->object = this;
			owner = (GameObjects*) container->owner;
			shapes = owner->shapes;
		}

		// Задать угол наклона в градусах и выполнить все действия связанные с этим
		void SetAngle(float angle);

		// Задать угол наклона по направлению к точке (x, y)
		void SetAngleTo(float x, float y);

		// Задать угол наклона в градусах при следующем вызове метода Rotate
		virtual void DirectTo(float angle);

		// Задать угол наклона по направлению к точке (x, y) при следующем вызове метода Rotate
		void DirectTo(float x, float y);

		// Постепенно повернуться с определённой скоростью
		virtual void RotateTo(float finalAngle, float speed);
		// Постепенно повернуться к точке (x, y) с определённой скоростью
		void RotateTo(float x, float y, float speed);

		// Задать скорость перемещения объекта
		void SetSpeed(float speed);

		// Виртуальный метод, который вызывается перед контактом с другим объектом
		virtual void BeforeContact() {};
		// Виртуальный метод, который вызывается после контакта с другим объектом
		virtual void AfterContact() {};

		// Непосредственное соприкосновение двух объектов
		virtual void Contact(GameObject* target, const Vector2f& normal);

		// Обновить состояние объекта.
		virtual void Update(double latency);

		// Отправить объект в буффер вывода на экран
		void PutToBuffer();

		// Отправить объект в корзину
		virtual void Recycle();

		// Появиться с противоположной стороны экрана
		void Reappear();

		// Обновить описание многоугольной формы объекта
		void UpdateActivePoly();

		// Обновить описание многоугольной формы объекта, смещённого по координатам coord
		void UpdateDrawPoly();

		// Применить импульс к объекту
		void ApplyImpulse(const Vector2f& j, const Vector2f& r);

		virtual ~GameObject();

	protected:
		// Вращение объекта
		virtual void Rotate(double latency);
		// Перемещение объекта
		virtual void Move(double latency);

		// Посчитать момент инерции
		virtual void CalcInertia();

		// Задать угол наклона объекта в градусах
		virtual void DoSetAngle(float angle);

	private:
		// масштабированная форма объекта
		ShapeF* internalShape;
};

// Класс с данными столкновения двух объектов
class ObjectsContact
{
	public:
		GameObject* obj1;
		GameObject* obj2;
		// точка столкновения
		Vector2f coord;
		// нормаль
		Vector2f normal;
		// Вектор направления от первого объекта к точке столкновения
		Vector2f dir1;
		// Вектор направления от второго объекта к точке столкновения
		Vector2f dir2;

		ObjectsContact(GameObject* obj1, GameObject* obj2, const Vector2f& coord, const Vector2f& normal);

		// Посчитать и применить импульсы к объектам
		void Solve();
};

#endif /* GAMEOBJECTS_H_ */
