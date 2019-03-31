#include "NE_Debug.hpp"
#include <assert.h>

#if defined(DEBUG) && defined(_MSC_VER)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

static void Assert (bool condition)
{
	if (!condition) {
		assert (condition);
	}
}

void DbgBreak ()
{
	Assert (false);
}

void DbgAssert (bool condition)
{
	Assert (condition);
}

bool DbgError (bool condition)
{
	Assert (!condition);
	return condition;
}

bool DbgVerify (bool condition)
{
	Assert (condition);
	return condition;
}

void EnableLeakDetection ()
{
#if defined(DEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
}
