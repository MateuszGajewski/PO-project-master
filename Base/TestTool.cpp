#include "TestTool.h"

#include <stdexcept>



TestTool::TestTool()
{}


TestTool::~TestTool()
{}


std::string TestTool::getEntry(const std::string &key) const
{
	try {
		return entries.at(key);
	}
	catch (std::out_of_range&) {
		throw BaseTool::UndefinedEntry();
	}
}


void TestTool::setEntry(const std::string &key, const std::string &value)
{
	entries[key] = value;
}


void TestTool::assignWindow(WINDOW *windowptr)
{
	window = windowptr;
}


void TestTool::edition()
{
	if (getEntry("KEY").size() == 1) {
		waddch(window, getEntry("KEY")[0]);
	}
	wrefresh(window);
}


void TestTool::fill()
{
	int height, width;
	getmaxyx(window, height, width);
	wmove(window, 0, 0);
	for (int i = 0; i < height * width; ++i) {
		waddstr(window, (std::to_string(i) + " ").c_str());
		waddch(window, i);
	}
	wrefresh(window);
}


void TestTool::unsave()
{
	setEntry("IS_SAVED", "NO");
}


void TestTool::param()
{
	wprintw(window, "otoooo %s", getEntry("param").c_str());
}


void TestTool::clear()
{
	wclear(window);
	wrefresh(window);
}