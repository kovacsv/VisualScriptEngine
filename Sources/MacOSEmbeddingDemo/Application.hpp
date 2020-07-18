#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "MAS_NodeEditorNSViewControl.hpp"

class Application
{
public:
	Application ();

	void Run ();

private:
	MAS::NodeEditorNSViewControl editorControl;
};

#endif
