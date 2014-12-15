/*
 * List.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Natalie
 */

#include "List.h"

List::~List()
{
	Clear();
}

static bool CompareValues(void* obj1, void* obj2)
{
	return (obj1 == obj2);
}

List::List(FreeObjectProc* freeProc)
{
	List(freeProc, &CompareValues);
}

List::List()
{
	List(NULL, &CompareValues);
}

List::List(FreeObjectProc* freeProc, CompareObjectsProc* compareProc)
{
	this->FreeObject = freeProc;
	this->CompareObjects = compareProc;
	root = NULL;
	last = NULL;
	items = NULL;
	count = 0;
	empty = NULL;
	united = false;
	FreeObject = NULL;
}

void List::SetCount(int count)
{
	if (count >= this->count)
	{
		for (int i = this->count; i < count; i++)
			Add();
	} else
	{
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
	ListItem* item;
	if (empty != NULL)
	{
		item = empty;
		ListItem* next = empty->next;
		ListItem* prev = empty->prev;
		if (next != NULL)
		{
			empty = next;
			next->prev = prev;
			if (prev != NULL)
				prev->next = next;
		} else
		if (prev != NULL)
		{
			empty = prev;
			prev->next = next;
			if (next != NULL)
				next->prev = prev;
		} else
			empty = NULL;
	} else
		item = new ListItem;
	item->parent = this;
	item->object = NULL;
	item->prev = last;
	item->next = NULL;
	item->index = count++;
	item->independent = true;
	if (root == NULL)
		root = item;
	else
		last->next = item;
	last = item;
	united = false;
	return item;
}

ListItem* List::Get(int index)
{
	if (index >= 0 && index < count)
	{
		Unite();
		return &(items[index]);
	}
	return NULL;
}

ListItem* List::GetSlow(int index)
{
	if (index >= 0 && index < count)
	{
		ListItem* item = this->root;
		int i = 0;

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
	if (item->parent == this)
	{
		count--;
		united = false;

		ListItem* prev = item->prev;
		if (item == root)
		{
			if (root == last)
				last == NULL;
			root = item->next;
			if (root != NULL)
				root->prev = NULL;
		} else
		{
			ListItem* next = item->next;
			prev->next = next;
			if (next == NULL)
				last = prev;
			else
				next->prev = prev;
		}
		item->index = -1;

		if (FreeObject != NULL)
			FreeObject(item->object);

		if (item->independent == false)
		{
			prev = empty;
			empty = item;
			if (prev != NULL)
			{
				item->next = NULL;
				item->prev = prev;
				prev->next = item;
			} else
			{
				item->next = NULL;
				item->prev = NULL;
			}
		} else
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
	this->empty = NULL;
	while (item != NULL)
	{
		ListItem* prev = item->prev;

		if (FreeObject != NULL)
			FreeObject(item->object);

		if (item->independent)
			delete item;

		item = prev;
	}

	delete[] items;
}

void List::Unite()
{
	if (!united)
	{
		united = true;
		if (count == 0)
		{
			delete[] this->items;
			this->items = NULL;
			return;
		}
		ListItem* items = new ListItem[count];
		ListItem* prev = NULL;
		ListItem* uitem = items;
		ListItem* item = this->root;
		int i = 0;
		while (item != NULL)
		{
			uitem->object = item->object;
			uitem->independent = false;
			uitem->index = i++;
			uitem->parent = this;
			uitem->prev = prev;
			prev = uitem;
			uitem->next = ++uitem;

			ListItem* next = item->next;

			if (item->independent)
				delete item;

			item = next;
		}

		delete[] this->items;
		this->empty = NULL;
		this->items = items;
	}
}



