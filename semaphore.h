#pragma once
#include <windows.h>
class semaphore
{
	HANDLE ghSemaphore;
public:
	semaphore(LONG lInitialCount, LONG lMaximumCount)
		:ghSemaphore{ CreateSemaphore(nullptr, lInitialCount, lMaximumCount, nullptr) }
	{}
	semaphore(LONG lInitialCount)
		:ghSemaphore{ CreateSemaphore(nullptr, lInitialCount, lInitialCount, nullptr) }
	{}

	void acquire()
	{
		DWORD err = WaitForSingleObject(ghSemaphore, INFINITE);
		if (err != WAIT_OBJECT_0) [[unlikely]]
		{
			throw GetLastError();
		}
	}
	void release(LONG n = 1)
	{
		if (!ReleaseSemaphore(
			ghSemaphore,  // handle to semaphore
			n,            // increase count
			NULL))       // not interested in previous count
			[[unlikely]]
		{
			throw GetLastError();
		}
	}
};