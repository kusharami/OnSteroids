/*
 * List.h
 *
 *  Класс для работы со связными списками объектов.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

class List;

// элемент списка
typedef struct list_item
{
	// владелец элемента
	List* owner;
	union
	{
		// объект элемента
		void* object;
		// иное значение
		int value;
	};
	// номер элемента
	int index;

	// указатель на предыдущий элемент
	struct list_item* prev;

	// указатель на следующий элемент
	struct list_item* next;
} ListItem;

// описатель функции освобождения объекта
typedef void FreeObjectProc(void* object);

// описатель функции сравнения объектов
typedef bool CompareObjectsProc(void*, void*);

class List
{
	public:
		// кол-во элементов
		int count;

		// первый элемент
		ListItem* root;

		// последний элемент
		ListItem* last;

		// добавлять в начало списка
		bool addBackwards;

		virtual ~List();
		List();
		List(FreeObjectProc* freeProc);
		List(FreeObjectProc* freeProc, CompareObjectsProc* compareProc);

		// установить кол-во
		void SetCount(int count);

		// добавить элемент
		ListItem* Add();

		// получить элемент за номером index
		ListItem* Get(int index);

		// получить элемент за номером index без создания внутреннего буффера
		ListItem* GetSlow(int index);

		// удалить элемент
		void Remove(ListItem* item);

		// удалить элемент, содержащий определённый объект
		void Remove(void* object);

		// удалить элемент с определённым номером
		void Remove(int index);

		// найти элемент, содердащий определённый объект
		ListItem* FindObject(void* object);

		// указатель на функцию уничтожения объекта
		void (*FreeObject) (void* object);

		// указатель на функцию сравнения двух объектов
		bool (*CompareObjects) (void* object1, void* object2);

		// поглотить похожий список
		void Absorb(List* list);

		// удалить все элементы
		virtual void Clear();

	private:
		// внутренний буффер элементов
		ListItem** items;

		// создан ли буффер
		bool united;

		// вызывается в конструкторе
		void Init(FreeObjectProc* freeProc, CompareObjectsProc* compareProc);

		// создать внутренний буффер элементов
		void Unite();
};

#endif /* LIST_H_ */
