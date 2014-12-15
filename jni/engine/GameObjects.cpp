/*
 * GameObjects.cpp
 *
 *  ������ ��� �������� � ��������������� ������� ��������.
 *
 *  ���� ��������: 04.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */


#include "GameObjects.h"

#include "Geo.h"

void ObjectOffscreenRecycle(GameObject* object)
{
	// ������ � �������
	object->Recycle();
}

void ObjectOffscreenReappear(GameObject* object)
{
	// ��������� � ��������������� �������
	object->Reappear();
}

void FreeGameObject(GameObject* object)
{
	// ���������� ��� �������� �� ������
	delete object;
}

/*** GameObject class ***/

void GameObject::SetMass(float mass)
{
	if (mass > .0)
	{
		this->mass = mass;
		this->invMass = 1.0 / mass;
		// ������� �����, ������� ������ �������
		CalcInertia();
	} else
	{
		this->mass = .0;
		this->invMass = .0;
		this->inertia = .0;
	}
}

void GameObject::Contact(GameObject* target, const Vector2f& normal)
{
	if (inertia > .0 && target->inertia > .0)
	{	// ������������ ����������, ���� ����� � ������� ����� �������� ������
		Poly<Vector2f>* p2 = target->drawPoly;
		Vector2f* v = drawPoly->vertices;
		Vector2f* end = &v[drawPoly->vertexCount];
		while (v < end)
		{
			// ���� ������������� ������� ������� �������� � ���� ����� �������
			if (p2->Contains(*v))
				// ������� � ���������� ��������
				ObjectsContact(this, target, *v, normal).Solve();
			v++;
		}
	}
}

void GameObject::CalcInertia()
{	// ������� ������� ������� ���� ������ http://www.physicsforums.com/showthread.php?t=25293&page=2&pp=15
	float denom = .0;
	float numer = .0;
	if (mass > .0 && shape != NULL)
	{
		Vector2f* v = shape->vertices;
		int cnt = shape->vertexCount;
		for (int i = cnt - 1, j = 0; j < cnt; i = j++)
		{
			Vector2f& p1 = v[i];
			Vector2f& p2 = v[j];
			float a = fabs(p2.Cross(p1));
			float b = p1.Dot(p1) + p1.Dot(p2) + p2.Dot(p2);

			denom += a * b;
			numer += a;
		}
		inertia = (mass / 6.0) * (denom / numer);
	} else
		inertia = .0;
}

void GameObject::SetShape(ShapeF* shape, const Vector2f* scale, const Vector2f* direction)
{
	// ������� �� ��������� 100%
	this->scale = Vector2f(1.0, 1.0);

	if (shape != NULL)
	{
		ShapeF* s = NULL;
		// ���� ����� ���� ������� ��� �� ������������� �������
		if (direction != NULL || (scale != NULL && (scale->x != 1.0 || scale->y != 1.0)))
		{
			// ������ ����� ������ �����
			s = this->shape = new ShapeF();

			// �������� ������ �� ���������
			s->Assign(shape);

			if (scale)
			{
				// ������������
				s->Scale(*scale);
				this->scale = *scale;
			}

			if (direction)
				// ������ ���� �������
				s->Rotate(*direction);
		} else
			this->shape = shape;

		// ������� ������ ����� �������, ���� ��� ������
		delete internalShape;
		internalShape = s;

		// ������������� ���������� ������ ��������������
		activePoly->SetVertexCount(shape->vertexCount);
		drawPoly->SetVertexCount(shape->vertexCount);

		// ����� ���� �������
		SetAngle(this->angle);

		// ��������� ���������� ��� ��������� �� ������
		UpdateDrawPoly();

		// ������� ������ �������
		CalcInertia();
	} else
	{
		// ����� �� ������
		delete internalShape;
		activePoly->SetVertexCount(0);
		drawPoly->SetVertexCount(0);
		this->shape = internalShape = NULL;
		UpdateDrawPoly();
		inertia = .0;
	}
}

void GameObject::UpdateDrawPoly()
{
	if (shape != NULL)
	{
		// ��������� �������������� ��� ������ �� �����
		activeRect->MoveTo(drawRect, coord);
		// ��������� �������������� ��� ����������� ������
		activePoly->MoveTo(drawPoly, coord);
		// ����������� ������ ����� �������
		shapeBounds = GetPolyBounds<float>(drawPoly);
	} else
	{
		// ���� ��� �����, �� ������� - ���� �����
		shapeBounds.left = shapeBounds.right = coord.x;
		shapeBounds.top = shapeBounds.bottom = coord.y;
	}
}

void GameObject::Update(double latency)
{
	if (flags & GOF_ACTIVE)
	{
		// �������
		Rotate(latency);
		// �����������
		Move(latency);

		// ���������� ������
		UpdateDrawPoly();

		if (offscreenEvent != NULL)
		{
			const Rect<float>& rect = owner->clientRect;
			if (shapeBounds.left >= rect.right
			||	shapeBounds.top >= rect.bottom
			||	shapeBounds.right <= rect.left
			||	shapeBounds.bottom <= rect.top)
				// ������ �� �������� ������
				offscreenEvent(this);
		}
	} else
	if ((flags & GOF_VISIBLE) && shape != NULL)
		// ������ ���������, �� �����
		// ��������� �������������� ��� ������ �� �����
		activeRect->MoveTo(drawRect, coord);
}

void GameObject::PutToBuffer()
{
	if (flags & GOF_VISIBLE)
	{
		// ������ ��� ������ � �������� �����������
		ShapesBuffer* shapes = &this->shapes->buffers[priority];

		// �������� ������� ������� � ������
		int i = shapes->Put(shape, drawRect->vertices);

		if (shapes->hasColor)
			// ���� ����� �������� �����, �����
			shapes->SetShapeColorF(i, red, green, blue, alpha);
	}
}

GameObject::~GameObject()
{
	delete internalShape;
	delete activeRect;
	delete drawRect;
	delete activePoly;
	delete drawPoly;
}

void GameObject::DirectTo(float x, float y)
{
	DirectTo(DegreeAngle<float>(x - coord.x, y - coord.y));
}

void GameObject::DirectTo(float angle)
{
	finalAngle = angle;
	rotateMode = ROTATE_INSTANT;
}

void GameObject::RotateTo(float x, float y, float speed)
{
	RotateTo(DegreeAngle(x - coord.x, y - coord.y), speed);
}

void GameObject::RotateTo(float finalAngle, float speed)
{
	if (speed == 0.0)
		speed = rotationSpeed;

	float dir = angle;
	if (finalAngle != dir)
	{
		// ������� �������� ��� ������������ ��������
		float dif1 = finalAngle - dir;
		if (dif1 < 0)
			dif1 += 360;
		// ������� �� �������

		float dif2 = dir - finalAngle;
		if (dif2 < 0)
			dif2 += 360;
		// ������� ������ �������

		if (speed < 0)
			speed = -speed;

		if (dif1 < dif2)
		{
			// �������, ���� �� ������� �������
			rotationSpeed = speed;
			rotateCounter = dif1;
		}
		else
		{
			// �������, ���� ������ ������� �������
			rotationSpeed = -speed;
			rotateCounter = dif2;
		}
		this->finalAngle = finalAngle;
		rotateMode = ROTATE_GRADUAL;
	}
}

void GameObject::Rotate(double latency)
{
	int mode = rotateMode;
	if (mode != ROTATE_NO)
	{
		if (mode == ROTATE_INSTANT)
		{
			rotateMode = ROTATE_NO;
			// ����� ���� �������
			SetAngle(finalAngle);
		} else
		if (rotationSpeed != 0)
		{
			float step = rotationSpeed * latency;
			// ��� ��������
			float angle = step + this->angle;
			// ����� ���� �������
			if (mode == ROTATE_GRADUAL)
			{
				// ���� ����������, ������ ������
				float counter;
				if (step < 0)
					counter = (rotateCounter += step);
				else
					counter = (rotateCounter -= step);

				if (counter <= 0)
				{
					// ����� �������� �����
					angle = finalAngle;
					rotateMode = ROTATE_NO;
				}
			}

			if (angle < 0)
				angle += 360;
			else
			if (angle >= 360)
				angle -= 360;

			// ����� ����
			SetAngle(angle);
		}
	}
}

void GameObject::Recycle()
{
	// ������ ��������� � �������
	flags &= ~(GOF_ACTIVE | GOF_VISIBLE);
	owner->ObjectRecycle(this);
}

void GameObject::SetColor(uint32_t color)
{
	// ���� ������� � ������� ARGB
	red = ColorToFloat(color >> 16);
	green = ColorToFloat(color >> 8);
	blue = ColorToFloat(color);
	alpha = ColorToFloat(color >> 24);
}

void GameObject::Reappear()
{
	GameObjects* owner = this->owner;
	int mode;

	#define __GO_CHECK(axis, start, end) \
		mode = 0; \
		if (motion.axis > 0 && shapeBounds.start >= owner->clientRect.end) \
			mode++; \
		else \
		if (motion.axis < 0 && shapeBounds.end <= owner->clientRect.start) \
			mode--; \
			 \
		if (mode != 0) \
		{ \
			float size = shapeBounds.end - shapeBounds.start; \
			float axis = coord.axis - shapeBounds.start; \
			 \
			if (mode > 0) \
				coord.axis = (owner->clientRect.start - size) + axis; \
			else \
			if (mode < 0) \
				coord.axis = owner->clientRect.end + axis; \
		}

	// ������� � ����� ������� ��������� �� ��� X
	__GO_CHECK(x, left, right);
	// ������� � ����� ������� ��������� �� ��� Y
	__GO_CHECK(y, top, bottom);

	#undef __GO_CHECK
}

void GameObject::DoSetAngle(float angle)
{
	this->angle = angle;

	// ������� ������ �������
	angleVector = DirectionVector<float>(angle * RADIAN);
}

void GameObject::SetAngle(float angle)
{
	// ��������� ������ ���� �������
	oldAngle = this->angle;

	// ������������� ����
	DoSetAngle(angle);

	if (updateDirection)
		// ������ ����������� ��������
		direction = DirectionVectorD<float>(this->angle);

	// ��������� �������������� ������ �� �����
	UpdateActivePoly();
}

void GameObject::UpdateActivePoly()
{
	if (shape != NULL)
	{
		// ��������� �������������� ��� ������ �� �����
		shape->GetDrawRect(activeRect->vertices);

		// ��������� ���� �������
		activeRect->Rotate(angleVector);

		// ��������� ��������������
		shape->CopyTo(activePoly);

		// ��������� ���� �������
		activePoly->Rotate(angleVector);
	}
}

void GameObject::SetAngleTo(float x, float y)
{
	// �������� ���� ������� �� ����������� � ����� (x, y)
	SetAngle(DegreeAngle<float>(y - coord.y, x - coord.x));
}

void GameObject::SetSpeed(float speed)
{
	if (speed > 0.0)
	{
		// ������ ������ ��������
		motion = direction * speed;
		this->speed = speed;
	} else
	{
		motion = Vector2f();
		this->speed = 0.0;
	}
}

void GameObject::Move(double latency)
{
	// ������ ���������� �������, ��������� ������ ��������
	coord.x += motion.x * latency;
	coord.y += motion.y * latency;
}

void GameObjects::ObjectRecycle(GameObject* object)
{
	// ������ � �������
	recycleBin->Add()->object = object;
}

void GameObjects::Update(double latency)
{
	if (latency > 0.0)
	{
		// ��������� ��������� ���� ��������
		ListItem* item = root;
		while (item != NULL)
		{
			GameObject* object = (GameObject*) item->object;
			object->Update(latency);
			item = item->next;
		}
	}

	// ��������������� ��������
	ListItem* item = root;
	while (item != NULL)
	{
		GameObject* obj1 = (GameObject*) item->object;
		item = item->next;

		// ����� ������ ������� � �� �������
		if ((obj1->flags & (GOF_GHOST | GOF_ACTIVE)) == GOF_ACTIVE)
		{
			// ��������� �� ������������� �� �� � ����� ��� � ����������� �� ��������� ��������
			ListItem* item2 = item;
			while (item2 != NULL)
			{
				GameObject* obj2 = (GameObject*) item2->object;
				item2 = item2->next;
				// ���� ������ ������� � �� �������
				if ((obj2->flags & (GOF_GHOST | GOF_ACTIVE)) == GOF_ACTIVE)
				{
					Edge<Vector2f> contactInfo;
					if (obj1->shapeBounds.Intersects(obj2->shapeBounds) &&
							// ���� ����� �������� ������������ �������������� ���������
					 PolygonsIntersect<Vector2f>(obj1->drawPoly, obj2->drawPoly, contactInfo))
						// ���� ����� ��������������� ������������
					{
						// �������� ����� ���������
						obj1->BeforeContact();
						obj2->BeforeContact();

						// ������������
						Vector2f& normal = contactInfo.normal;
						obj1->Contact(obj2, normal);
						obj2->Contact(obj1, normal);

						if (obj1->mass > .0 && obj2->mass > .0)
						{
							// ��������� ���������, ���� ������� ����� �����
							// ����� ����� ������ ��������� ������, ��� ����� ������
							float sum = obj1->mass + obj2->mass;
							obj1->coord += normal * (contactInfo.d * (obj2->mass / sum));
							obj2->coord -= normal * (contactInfo.d * (obj1->mass / sum));
						}

						// �������� ����� ��������
						obj1->AfterContact();
						obj2->AfterContact();

						if ((obj1->flags & GOF_ACTIVE) == 0)
							break; // ���� ������ 1 ��� �����, ��������� �� ��� ������� �� �����������
					}
				}
			}
		}
	}

	// ���������� ������� ��� ������ �� �����
	shapes->Prepare();
	// ����� �� ����� � �����. ����� ������ ������ ������� ����
	item = last;
	while (item != NULL)
	{
		GameObject* object = (GameObject*) item->object;
		object->PutToBuffer();
		item = item->prev;
	}

	// ������� ������� ��������
	RecycleBinClear();
}

GameObjects::~GameObjects()
{
	delete shapes;
	delete recycleBin;
}

GameObjects::GameObjects(GLScreen* owner, Texture* texture, int maxObjects, int maxPriorities, bool colors) :
		List((FreeObjectProc*) &FreeGameObject), owner(owner)
{
	// ������������� ������� ������ �� �����
	(shapes = new ShapesBufferEx())->Init(texture, maxPriorities, maxObjects, colors);

	// �������
	recycleBin = new List();

	// ������� ���������
	clientRect = Rect<float>(0, 0, owner->width, owner->height);
}

void GameObjects::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	shapes->Draw();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

GameObject* GameObjects::Add(GameObject* object)
{
	if (object != NULL)
		// ���������� �������
		object->SetContainer(List::Add());
	return object;
}

void GameObjects::RecycleBinClear()
{
	ListItem* item = recycleBin->root;
	while (item != NULL)
	{
		// �������� �� ������ �������, ������������ � �������
		Remove(((GameObject*) (item->object))->container);
		item = item->next;
	}
	recycleBin->Clear();
}

void GameObject::ApplyImpulse(const Vector2f& j, const Vector2f& r)
{
	// ��������� ������� � ������� ��������
	motion += j / mass;

	// �� ��������� ������� 1000 �������� � �������
	if (motion.x < -1.0)
		motion.x = -1.0;
	else
	if (motion.x > 1.0)
		motion.x = 1.0;

	if (motion.y < -1.0)
		motion.y = -1.0;
	else
	if (motion.y > 1.0)
		motion.y = 1.0;


	float w = rotationSpeed * RADIAN;

	// ��������� ������� � �������� ��������
	w += r.Cross(j) / inertia;

	// ��������� �������� ��������
	rotationSpeed = (rotationSpeed > 0)
			? Min<float>(maxRotationSpeed, w * DEGREE)
			: Max<float>(-maxRotationSpeed, w * DEGREE);
}

/** ObjectsContact class **/

ObjectsContact::ObjectsContact(GameObject* obj1, GameObject* obj2, const Vector2f& coord, const Vector2f& normal)
{
	this->obj1 = obj1;
	this->obj2 = obj2;
	this->coord = coord;
	this->normal = normal;
	dir1 = coord - obj1->coord;
	dir2 = coord - obj2->coord;
}

void ObjectsContact::Solve()
{
	// ������� ������������ ����� �� ���������

	// ������������
	float restitution = Max<float>(obj1->restitution, obj2->restitution);

	// ����� ������
	float friction = obj1->friction * obj2->friction;

	// ����� �����
	float invMass = obj1->invMass + obj2->invMass;

	// ������ ����������� � �������� ������� 1
	Vector2f v1 = obj1->motion + (dir1.Perpendicular() * (obj1->rotationSpeed * RADIAN));

	// ������ ����������� � �������� ������� 2
	Vector2f v2 = obj2->motion + (dir2.Perpendicular() * (obj2->rotationSpeed * RADIAN));

	// ������ ������������� ��������
	Vector2f relativeVelocity = v2 - v1;

	// �������� ������������� ��������
	float relativeVelocityN = relativeVelocity.Dot(normal);

	// ������������ � ������������ ���������
	float bounce = relativeVelocityN * restitution;

	// �������� ����������� ������� �������
	float dir1n = dir1.Cross(normal);
	// �������� ����������� ������� �������
	float dir2n = dir2.Cross(normal);

	// ???
	float kn = invMass + dir1n * dir1n / obj1->inertia + dir2n * dir2n / obj2->inertia;
	float nMass = 1.0 / kn;

	float jn = -(bounce + relativeVelocityN) * nMass;
	float jnAcc = Max<float>(jn, .0);

	Vector2f t = normal.Perpendicular();

	float dir1t = dir1.Cross(t);
	float dir2t = dir2.Cross(t);

	float kt = invMass + dir1t * dir1t / obj1->inertia + dir2t * dir2t / obj2->inertia;
	float tMass = 1.0 / kt;

	float jtMax = friction * jnAcc;
	float jt = -relativeVelocity.Dot(t) * tMass;
	float jtAcc = Min<float>(jtMax, Max<float>(-jtMax, jt));
	Vector2f j = (normal * jn) + (t * jt);

	// ���������� ���������
	obj1->ApplyImpulse(j.Negative(), dir1);
	obj2->ApplyImpulse(j, dir2);
}
