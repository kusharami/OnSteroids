/*
 * TriggerButton.h
 *
 *  ������ ������ ������-�������������  ��� �������� �������.
 *
 *  ���� ��������: 10.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef TRIGGERBUTTON_H_
#define TRIGGERBUTTON_H_

#include "TouchControllerObject.h"

template <typename ControllerClass>
class TriggerButton: public TouchControllerObject, public ControllerClass
{
	public:
		// ���� �������
		int touchColor;
		// �������� ����
		int untouchColor;
		TriggerButton() : TouchControllerObject(), ControllerClass(),
				untouchColor(0xFFFFFFFF),
				touchColor(0xFFFF0000) { controller = this; };

		// ������������� ������
		virtual void Control(double latency)
		{
			if (touchRect.Contains(touchX, touchY))
			{
				// ���� ����� ������ ������
				// ������ ����
				SetColor(touchColor);
				// ���������
				ControllerClass::Control(latency);
			} else
				// ����� ��
				Stop(latency);
		};

		// ��������� ������
		virtual void Stop(double latency)
		{
			ControllerClass::Stop(latency);
			// ������ ���� �� ��������
			SetColor(untouchColor);
		}
};


#endif /* TRIGGERBUTTON_H_ */
