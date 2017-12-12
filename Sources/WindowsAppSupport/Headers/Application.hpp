#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"

#include <windows.h>
#include <gdiplus.h>
#include <string>

class Application : public UI::Window
{
public:
	Application ();
	virtual ~Application ();
};

#endif
