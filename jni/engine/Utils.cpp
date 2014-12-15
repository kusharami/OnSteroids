/*
 * Utils.cpp
 *
 *  �������. ���������.
 *
 *  ���� ��������: 04.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#include "Utils.h"

uint32_t rmz = 0xF028CA76;
uint32_t rmw = 0x359B4DE1;

uint32_t Random()
{
	// ���������� ��������� ����� �� ������ George Marsaglia
	uint32_t mz = rmz;
	uint32_t mw = rmw;
	rmz = mz = (36969 * (mz & 0xFFFF)) + (mz >> 16);
	rmw = mw = (18000 * (mw & 0xFFFF)) + (mw >> 16);
	return (mz << 16) + mw;
}
