#pragma once

#ifndef __CONTINUOS_LIST__
#define __CONTINUOS_LIST__

template <typename T>
struct contListNav
{
	contListNav<T>* prev = nullptr;
	contListNav<T>* next = nullptr;
};

template<typename T>
struct contList
{
	contListNav<T>* head = nullptr;

	static void add(contListNav<T>* p)
	{

	}
};

#endif
