/*
 * List.cpp
 *
 *  Класс для работы со связными списками объектов.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#include "List.h"

// сравнивалка по умолчанию
static bool CompareValues(void* obj1, void* obj2)
{
	return (obj1 == obj2);
}

List::~List()
{
	Clear();
}

List::List(FreeObjectProc* freeProc)
{
	Init(freeProc, &CompareValues);
}

List::List()
{
	Init(NULL, &CompareValues);
}

List::List(FreeObjectProc* freeProc, CompareObjectsProc* compareProc)
{
	Init(freeProc, compareProc);
}

void List::Absorb(List* list)
{
	// удалить все элементы
	Clear();

	// установить ссылки на элементы
	root = list->root;
	last = list->last;
	count = list->count;

	// убрать ссылки на элементы из источника
	list->root = list->last = NULL;
	list->count = 0;
}

void List::Init(FreeObjectProc* freeProc, CompareObjectsProc* compareProc)
{
	addBackwards = false;
	FreeObject = freeProc;
	CompareObjects = compareProc;
	root = NULL;
	last = NULL;
	items = NULL;
	count = 0;
	united = false;
}

void List::SetCount(int count)
{
	if (count >= this->count)
	{
		// если новое кол-во больше старого, добавляем элементы
		for (int i = this->count; i < count; i++)
			Add();
	} else
	{
		// если меньше - удаляем элементы
		ListItem* item = last;
		for (int i = this->count; --i >= count; )
		{
			ListItem* prev = item->prev;
			Remove(item);
			item = prev;
		}
	}
}

ListItem* List::Add()
{
	ListItem* item = new ListItem;
	item->owner = this;
	item->object = NULL;

	if (addBackwards)
	{
		// добавить в начало списка
		item->next = root;
		item->prev = NULL;

		if (last == NULL)
			last = item;
		else
			root->prev = item;

		root = item;
	} else
	{
		// добавить в конец списка
		item->index = count;

		item->prev = last;
		item->next = NULL;

		if (root == NULL)
			root = item;
		else
			last->next = item;

		last = item;
	}

	count++;
	// внутренний буффер не готов
	united = false;
	return item;
}

ListItem* List::Get(int index)
{
	if (index >= 0 && index < count)
	{
		// подготовить буффер элементов
		Unite();
		// возвратить элемент
		return items[index];
	}
	return NULL;
}

ListItem* List::GetSlow(int index)
{
	if (index >= 0 && index < count)
	{
		ListItem* item = this->root;
		int i = 0;
		// поиск элемента в списке
		while (item != NULL)
		{
			if (index == i++)
				return item;
			item = item->next;
		}
	}
	return NULL;
}

void List::Remove(ListItem* item)
{
	if (item->owner == this)
	{
		count--;
		united = false;

		ListItem* prev = item->prev;
		if (item == root)
		{
			// если удаляем самый первый элемент
			if (root == last)
				last == NULL;
			root = item->next;
			if (root != NULL)
				root->prev = NULL;
		} else
		{
			// любой другой
			ListItem* next = item->next;
			prev->next = next;
			if (next == NULL)
				last = prev;
			else
				next->prev = prev;
		}
		item->index = -1;

		if (FreeObject != NULL)
			// уничтожение объекта
			FreeObject(item->object);

		delete item;
	}
}

void List::Remove(void* object)
{
	Remove(FindObject(object));
}

void List::Remove(int index)
{
	Remove(Get(index));
}

ListItem* List::FindObject(void* object)
{
	// сравниваем каждый объект с искомым
	ListItem* item = this->root;
	while (item != NULL)
	{
		if (CompareObjects(item->object, object))
			return item;
		item = item->next;
	}
	return NULL;
}

void List::Clear()
{
	count = 0;
	ListItem* item = this->last;
	this->root = NULL;
	this->last = NULL;
	// удаляем объекты с конца
	while (item != NULL)
	{
		ListItem* prev = item->prev;

		if (FreeObject != NULL)
			// уничтожение объекта
			FreeObject(item->object);

		delete item;

		item = prev;
	}

	delete[] items;
}

void List::Unite()
{
	if (!united)
	{
		// если буффер не готов, заполняем
		united = true;
		if (count == 0)
		{
			// количество 0, удаляем
			delete[] this->items;
			this->items = NULL;
			return;
		}
		ListItem** items = new ListItem*[count];
		ListItem** uitem = items;
		ListItem* item = this->root;
		int i = 0;
		while (item != NULL)
		{
			*(uitem++) = item;
			item->index = i++;
			item = item->next;
		}

		delete[] this->items;
		this->items = items;
	}
}

