/*
 * List.h
 *
 *  ����� ��� ������ �� �������� �������� ��������.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

class List;

// ������� ������
typedef struct list_item
{
	// �������� ��������
	List* owner;
	union
	{
		// ������ ��������
		void* object;
		// ���� ��������
		int value;
	};
	// ����� ��������
	int index;

	// ��������� �� ���������� �������
	struct list_item* prev;

	// ��������� �� ��������� �������
	struct list_item* next;
} ListItem;

// ��������� ������� ������������ �������
typedef void FreeObjectProc(void* object);

// ��������� ������� ��������� ��������
typedef bool CompareObjectsProc(void*, void*);

class List
{
	public:
		// ���-�� ���������
		int count;

		// ������ �������
		ListItem* root;

		// ��������� �������
		ListItem* last;

		// ��������� � ������ ������
		bool addBackwards;

		virtual ~List();
		List();
		List(FreeObjectProc* freeProc);
		List(FreeObjectProc* freeProc, CompareObjectsProc* compareProc);

		// ���������� ���-��
		void SetCount(int count);

		// �������� �������
		ListItem* Add();

		// �������� ������� �� ������� index
		ListItem* Get(int index);

		// �������� ������� �� ������� index ��� �������� ����������� �������
		ListItem* GetSlow(int index);

		// ������� �������
		void Remove(ListItem* item);

		// ������� �������, ���������� ����������� ������
		void Remove(void* object);

		// ������� ������� � ����������� �������
		void Remove(int index);

		// ����� �������, ���������� ����������� ������
		ListItem* FindObject(void* object);

		// ��������� �� ������� ����������� �������
		void (*FreeObject) (void* object);

		// ��������� �� ������� ��������� ���� ��������
		bool (*CompareObjects) (void* object1, void* object2);

		// ��������� ������� ������
		void Absorb(List* list);

		// ������� ��� ��������
		virtual void Clear();

	private:
		// ���������� ������ ���������
		ListItem** items;

		// ������ �� ������
		bool united;

		// ���������� � ������������
		void Init(FreeObjectProc* freeProc, CompareObjectsProc* compareProc);

		// ������� ���������� ������ ���������
		void Unite();
};

#endif /* LIST_H_ */
