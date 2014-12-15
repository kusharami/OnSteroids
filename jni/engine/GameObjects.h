/*
 * GameObjects.h
 *
 *  ������ ��� �������� � ��������������� ������� ��������.
 *
 *  ���� ��������: 04.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
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

// ������� ������, ���� �� ����� �� ������� ������
void ObjectOffscreenRecycle(GameObject* object);

// ������ ���������� � ������ ������� ������, ���� �� ����� �� �������
void ObjectOffscreenReappear(GameObject* object);

// �� ������� ������
#define ROTATE_NO 0

// ������ �������
#define ROTATE_ENDLESS 1

// ������� ����������� �����
#define ROTATE_GRADUAL 2

// ��������� �����
#define ROTATE_INSTANT 3

class GameObjects: public List
{
	public:
		// �����, ��������� ���������
		GLScreen* owner;

		// ������� ��������� ��������
		Rect<float> clientRect;

		// ������� ��� ��������
		List* recycleBin;

		// ������ ��� ������ �������� �� �����
		ShapesBufferEx* shapes;

		GameObjects(GLScreen* owner, Texture* texture, int maxObjects, int maxPriorities, bool colors);
		virtual ~GameObjects();

		// �������� ������
		GameObject* Add(GameObject* object);

		// �������� ��������� ���� ��������
		// latency - ���������� ������� � �������������, ������� ������ � ������� ����������� ������ ����� ������
		virtual void Update(double latency);

		// ��������� ������ � �������
		void ObjectRecycle(GameObject* object);

		// ����� ������� �� �����
		void Draw();

	private:
		// �������� �������
		void RecycleBinClear();
};

// ����� �������

// �������
#define GOF_VISIBLE 		(1 << 0)

// ��������
#define GOF_ACTIVE 			(1 << 1)

// ������� (�������� �� ��������������)
#define GOF_GHOST 			(1 << 2)

// *** //
class GameObject
{
	public:
		// ������, ��������� ���� ��������
		GameObjects* owner;
		// ������� ������, ��������� ���� ��������
		ListItem* container;

		// ������ ��� ������ �� �����
		ShapesBufferEx* shapes;

		// ��������� ������� �� ������ (���� - �������, ���� - ������)
		int priority;

		// ����� �������
		unsigned int flags;

		// ������������� �������, ������� �������������
		int id;

		// ������� ��������� �������
		Rect<float> shapeBounds;

		// ����� �������
		ShapeF* shape;

		// ����� ������� � � ��������������� ��������
		float mass, invMass;

		// ������ �������
		float inertia;

		// ������
		float friction;

		// ������������
		float restitution;

		// ������������ �������� ��������
		float maxRotationSpeed;

		// ������� �������� ��������
		float rotationSpeed;

		// ������� ��������. ������������ ��� rotateMode = ROTATE_GRADUAL
		float rotateCounter;

		// ����� ��������: ROTATE_NO, ROTATE_ENDLESS, ROTATE_GRADUAL, ROTATE_INSTANT
		int rotateMode;

		// �������� ���� ������� ��� ROTATE_GRADUAL
		float finalAngle;

		// ���� ������� ������� � ��������. ��� ������� � ������ ��������
		float angle, oldAngle;

		// ��������� ����������� �������� ������� ��� ��������� ���� �������
		bool updateDirection;

		// ������ �������
		Vector2f angleVector;

		// ������ �����������
		Vector2f direction;

		// ������ ��������
		Vector2f motion;

		// ���������� �������
		Vector2f coord;

		// ������� �������
		Vector2f scale;

		// ���� ������� � ������� ��� ������������
		GLfloat red, green, blue, alpha;

		// �������� �������
		float speed;

		// ������� ��� ������ �� ������� ������
		OffscreenEvent* offscreenEvent;

		// ������������� ��� ������ ����� �� �����
		Poly<Vector2f>* activeRect;
		// ������������� ��� ������ ����� �� �����, ��������� �� ����������� coord
		Poly<Vector2f>* drawRect;

		// �������� ������������� ����� �������
		Poly<Vector2f>* activePoly;

		// �������� ������������� ����� �������, ��������� �� ����������� coord
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

		// ������ ����� �������
		void SetMass(float mass);

		// ������ ���� ������� � ������� ARGB
		void SetColor(uint32_t color);

		// ������ ����� ������� � ����������� ��������
		void SetShape(ShapeF* shape, const Vector2f& scale)
		{
			SetShape(shape, &scale, NULL);
		};

		// ������ ����� ������� � ����������� ��������, � ����������� �������� �������
		void SetShape(ShapeF* shape, const Vector2f* scale, const Vector2f* direction);

		// ��������� ���������� �������
		void SetContainer(ListItem* container)
		{
			this->container = container;
			container->object = this;
			owner = (GameObjects*) container->owner;
			shapes = owner->shapes;
		}

		// ������ ���� ������� � �������� � ��������� ��� �������� ��������� � ����
		void SetAngle(float angle);

		// ������ ���� ������� �� ����������� � ����� (x, y)
		void SetAngleTo(float x, float y);

		// ������ ���� ������� � �������� ��� ��������� ������ ������ Rotate
		virtual void DirectTo(float angle);

		// ������ ���� ������� �� ����������� � ����� (x, y) ��� ��������� ������ ������ Rotate
		void DirectTo(float x, float y);

		// ���������� ����������� � ����������� ���������
		virtual void RotateTo(float finalAngle, float speed);
		// ���������� ����������� � ����� (x, y) � ����������� ���������
		void RotateTo(float x, float y, float speed);

		// ������ �������� ����������� �������
		void SetSpeed(float speed);

		// ����������� �����, ������� ���������� ����� ��������� � ������ ��������
		virtual void BeforeContact() {};
		// ����������� �����, ������� ���������� ����� �������� � ������ ��������
		virtual void AfterContact() {};

		// ���������������� ��������������� ���� ��������
		virtual void Contact(GameObject* target, const Vector2f& normal);

		// �������� ��������� �������.
		virtual void Update(double latency);

		// ��������� ������ � ������ ������ �� �����
		void PutToBuffer();

		// ��������� ������ � �������
		virtual void Recycle();

		// ��������� � ��������������� ������� ������
		void Reappear();

		// �������� �������� ������������� ����� �������
		void UpdateActivePoly();

		// �������� �������� ������������� ����� �������, ���������� �� ����������� coord
		void UpdateDrawPoly();

		// ��������� ������� � �������
		void ApplyImpulse(const Vector2f& j, const Vector2f& r);

		virtual ~GameObject();

	protected:
		// �������� �������
		virtual void Rotate(double latency);
		// ����������� �������
		virtual void Move(double latency);

		// ��������� ������ �������
		virtual void CalcInertia();

		// ������ ���� ������� ������� � ��������
		virtual void DoSetAngle(float angle);

	private:
		// ���������������� ����� �������
		ShapeF* internalShape;
};

// ����� � ������� ������������ ���� ��������
class ObjectsContact
{
	public:
		GameObject* obj1;
		GameObject* obj2;
		// ����� ������������
		Vector2f coord;
		// �������
		Vector2f normal;
		// ������ ����������� �� ������� ������� � ����� ������������
		Vector2f dir1;
		// ������ ����������� �� ������� ������� � ����� ������������
		Vector2f dir2;

		ObjectsContact(GameObject* obj1, GameObject* obj2, const Vector2f& coord, const Vector2f& normal);

		// ��������� � ��������� �������� � ��������
		void Solve();
};

#endif /* GAMEOBJECTS_H_ */
