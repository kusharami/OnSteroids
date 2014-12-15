/*
 * Utils.h
 *
 *  �������. ���������.
 *
 *  ���� ��������: 04.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <cmath>

// ��������� ��� �������� �������� � �������
#define RADIAN ((1.0 / 180.0) * M_PI)

// ��������� ��� �������� ������ � �������
#define DEGREE ((1.0 / M_PI) * 180.0)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// ������������ �� ���� ��������
template <typename T>
static inline T Max(T a, T b)
{
	return MAX(a, b);
}

// ����������� �� ���� ��������
template <typename T>
static inline T Min(T a, T b)
{
	return MIN(a, b);
}

// ���������� ��������� �����
uint32_t Random();

// ���������� ��������� ����� � ���������� �� 0 �� count - 1
static inline uint32_t Random(uint32_t count)
{
	return Random() % count;
}

// ���������� ��� ���������� ��������� �����
extern uint32_t rmz, rmw;

// �������� ��� ����������
static inline void Randomize(uint32_t mz, uint32_t mw)
{
	rmz = mz;
	rmw = mw;
}

#endif /* UTILS_H_ */
