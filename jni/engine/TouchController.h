/*
 * TouchController.h
 *
 *  ����� �������������� � ��������������� ������������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef TOUCHCONTROLLER_H_
#define TOUCHCONTROLLER_H_

class TouchController
{
	public:
		// ������������� �������������
		int touchID;
		// ���������� �������������
		float touchX, touchY;

		// ������� �������������
		Rect<float> touchRect;

		TouchController() : touchID(-1), touchX(0.0), touchY(0.0), touchRect() {};

		void SetBounds(float x, float y, float width, float height)
		{
			touchRect.left = x;
			touchRect.top = y;
			touchRect.right = x + width;
			touchRect.bottom = y + height;
		};

		// ������
		virtual void TouchDown(double latency, int touchID, float x, float y)
		{
			if (touchRect.Contains(x, y))
			{
				// ���� ����� ������ ������
				this->touchID = touchID;
				TouchMove(latency, x, y);
			}
		};

		// �������
		virtual void TouchMove(double latency, float x, float y)
		{
			this->touchX = x;
			this->touchY = y;
		}

		// ���������
		virtual void TouchRelease(double latency, float x, float y)
		{
			this->touchID = -1;
			this->touchX = x;
			this->touchY = y;
		};

		virtual ~TouchController() {};
};


#endif /* TOUCHCONTROLLER_H_ */
