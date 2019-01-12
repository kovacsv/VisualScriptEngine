#ifndef NE_DEBUG_HPP
#define NE_DEBUG_HPP

#ifdef DEBUG
#define DBGBREAK() DbgBreak()
#define DBGASSERT(condition) DbgAssert(condition)
#define DBGERROR(condition) DbgError(condition)
#define DBGVERIFY(condition) DbgVerify(condition)
#define DBGONLY(text) text
#else
#define DBGBREAK()
#define DBGASSERT(condition)
#define DBGERROR(condition) (condition)
#define DBGVERIFY(condition) (condition)
#define DBGONLY(text)
#endif

void DbgBreak ();
void DbgAssert (bool condition);
bool DbgError (bool condition);
bool DbgVerify (bool condition);

void EnableLeakDetection ();

#endif
