#include "Debug.hpp"
#include <assert.h>

#if defined(DEBUG) && defined(_MSC_VER)
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
	#ifndef DBG_NEW
		#define DBG_NEW new (_NORMAL_BLOCK, __FILE__ ,__LINE__ )
		#define new DBG_NEW
	#endif
#endif

void DbgBreak ()
{
	assert (false);
}

void DbgAssert (bool condition)
{
	assert (condition);
}

bool DbgError (bool condition)
{
	assert (!condition);
	return condition;
}

bool DbgVerify (bool condition)
{
	assert (condition);
	return condition;
}

void EnableLeakDetection ()
{
#if defined(DEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
}
