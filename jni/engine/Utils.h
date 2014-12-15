/*
 * Utils.h
 *
 *  ‘ункции.  онстанты.
 *
 *  ƒата создани€: 04.09.2013
 *  ƒата изменени€: 16.09.2013
 *
 *  јвтор: „ерданцева јлександра
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <cmath>

// константа дл€ перевода градусов в радианы
#define RADIAN ((1.0 / 180.0) * M_PI)

// константа дл€ перевода радиан в градусы
#define DEGREE ((1.0 / M_PI) * 180.0)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// максимальное из двух значений
template <typename T>
static inline T Max(T a, T b)
{
	return MAX(a, b);
}

// минимальное из двух значений
template <typename T>
static inline T Min(T a, T b)
{
	return MIN(a, b);
}

// возвращает случайное число
uint32_t Random();

// возвращает случайное число в промежутке от 0 до count - 1
static inline uint32_t Random(uint32_t count)
{
	return Random() % count;
}

// переменные дл€ генератора случайных чисел
extern uint32_t rmz, rmw;

// изменить эти переменные
static inline void Randomize(uint32_t mz, uint32_t mw)
{
	rmz = mz;
	rmw = mw;
}

#endif /* UTILS_H_ */
