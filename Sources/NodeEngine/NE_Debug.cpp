#include "NE_Debug.hpp"
#include <assert.h>

#if defined(DEBUG) && defined(_MSC_VER)
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
	#ifndef DBG_NEW
		#define DBG_NEW new (_NORMAL_BLOCK, __FILE__ ,__LINE__ )
		#define new DBG_NEW
	#endif
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
