#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <windows.h>
#include <gdiplus.h>
#include <string>

class Application
{
public:
	Application ();
	virtual ~Application ();

	void			Start ();

	virtual void	OnInit ();
	virtual void	OnIdle ();
};

#endif
