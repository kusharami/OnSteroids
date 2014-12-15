/*
 * TriggerButton.h
 *
 *  Шаблон класса кнопки-переключателя  для игрового объекта.
 *
 *  Дата создания: 10.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef TRIGGERBUTTON_H_
#define TRIGGERBUTTON_H_

#include "TouchControllerObject.h"

template <typename ControllerClass>
class TriggerButton: public TouchControllerObject, public ControllerClass
{
	public:
		// цвет нажатия
		int touchColor;
		// основной цвет
		int untouchColor;
		TriggerButton() : TouchControllerObject(), ControllerClass(),
				untouchColor(0xFFFFFFFF),
				touchColor(0xFFFF0000) { controller = this; };

		// контроллируем объект
		virtual void Control(double latency)
		{
			if (touchRect.Contains(touchX, touchY))
			{
				// если палец внутри границ
				// меняем цвет
				SetColor(touchColor);
				// действуем
				ControllerClass::Control(latency);
			} else
				// иначе всё
				Stop(latency);
		};

		// отпускаем объект
		virtual void Stop(double latency)
		{
			ControllerClass::Stop(latency);
			// меняем цвет на основной
			SetColor(untouchColor);
		}
};


#endif /* TRIGGERBUTTON_H_ */
