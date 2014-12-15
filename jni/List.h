/*
 * List.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

class List;

typedef struct list_item
{
	List* parent;
	union
	{
		void* object;
		int value;
	};
	int index;
	bool independent;
	struct list_item* next;
	struct list_item* prev;
} ListItem;

typedef void FreeObjectProc(void* object);
typedef bool CompareObjectsProc(void*, void*);

class List
{
	public:
		int count;
		ListItem* root;
		ListItem* last;

		virtual ~List();
		List();
		List(FreeObjectProc* freeProc);
		List(FreeObjectProc* freeProc, CompareObjectsProc* compareProc);

		void SetCount(int count);

		ListItem* Add();
		ListItem* Get(int index);
		ListItem* GetSlow(int index);

		void Remove(ListItem* item);
		void Remove(void* object);
		void Remove(int index);
		ListItem* FindObject(void* object);

		void (*FreeObject) (void* object);
		bool (*CompareObjects) (void* object1, void* object2);

		virtual void Clear();
		void Unite();

	protected:
		ListItem* empty;
		ListItem* items;
		bool united;
};

#endif /* LIST_H_ */
