/*
 * Vector2d.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

namespace Vector2d
{
	typedef struct
	{
		float x, y;
	} Point;

	typedef union
	{
			struct
			{
				Point x1y1;
				Point x2y1;
				Point x1y2;
				Point x2y2;
			};
			Point points[4];
	} Rect;
}


#endif /* VECTOR2D_H_ */
