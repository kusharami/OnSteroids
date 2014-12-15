/*
 * TouchController.h
 *
 *  Класс взаимодействия с прикосновениями пользователя.
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef TOUCHCONTROLLER_H_
#define TOUCHCONTROLLER_H_

class TouchController
{
	public:
		// идентификатор прикосновения
		int touchID;
		// координаты прикосновения
		float touchX, touchY;

		// границы прикосновения
		Rect<float> touchRect;

		TouchController() : touchID(-1), touchX(0.0), touchY(0.0), touchRect() {};

		void SetBounds(float x, float y, float width, float height)
		{
			touchRect.left = x;
			touchRect.top = y;
			touchRect.right = x + width;
			touchRect.bottom = y + height;
		};

		// нажать
		virtual void TouchDown(double latency, int touchID, float x, float y)
		{
			if (touchRect.Contains(x, y))
			{
				// если точка внутри границ
				this->touchID = touchID;
				TouchMove(latency, x, y);
			}
		};

		// двигать
		virtual void TouchMove(double latency, float x, float y)
		{
			this->touchX = x;
			this->touchY = y;
		}

		// отпустить
		virtual void TouchRelease(double latency, float x, float y)
		{
			this->touchID = -1;
			this->touchX = x;
			this->touchY = y;
		};

		virtual ~TouchController() {};
};


#endif /* TOUCHCONTROLLER_H_ */
