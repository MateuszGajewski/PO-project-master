#include "Menu.h"
#include "margins.h"
#include "colors.h"



Menu::Menu(WINDOW *const &menuWindow, WINDOW *const &toolWindow) :
	menuWindow(menuWindow), toolWindow(toolWindow), 
	optionsWindow(nullptr),
	data(),
	positioner(data, LINES)

{}


Menu::~Menu()
{
	if (optionsWindow)
		delwin(optionsWindow);
}


void Menu::addItem(const ParsedBind &bind, const std::function<void()> handler)
{
	try {
		auto &item = data.getByLabel(bind.top);
		item.addItem(OptionItem(bind.option, bind.param, bind.entry, handler));
	}
	catch (MenuData::UndefinedItem&) {
		TopItem item(bind.top);
		item.addItem(OptionItem(bind.option, bind.param, bind.entry, handler));
		data.addItem(item);
	}
}


void Menu::renderTop2()
{
	
	if (COLS < positioner.getMinWidth() ||
		LINES < positioner.getMinHeight())
		throw TooSmallTerminal();

	//short current = MENU_ODD_PAIR, next = MENU_EVEN_PAIR;
	for (auto &item : data.getItems()) {
		//wattron(menuWindow, COLOR_PAIR(current));
		mvwprintw(
			menuWindow,
			0, positioner.getPosition(item).dx,
			"%s", (
				std::string(TOP_MARGIN_LEFT, ' ') +
				item.getLabel() +
				std::string(TOP_MARGIN_RIGHT, ' ')
			).c_str()
		);
		//wattroff(menuWindow, COLOR_PAIR(current));
		//std::swap(current, next);
	}
	//wattron(menuWindow, COLOR_PAIR(current));
	wprintw(menuWindow, std::string(COLS - getcurx(menuWindow), ' ').c_str());
	//wattroff(menuWindow, COLOR_PAIR(current));
	wrefresh(menuWindow);
}

int Menu::renderTop2(int x, int y)
{
	int found = 0;
	if (COLS < positioner.getMinWidth() ||
		LINES < positioner.getMinHeight())
		throw TooSmallTerminal();

	short current = MENU_ODD_PAIR, next = MENU_EVEN_PAIR;
	for (auto& item : data.getItems()) {
		//wattron(menuWindow, COLOR_PAIR(current));
		if (positioner.getPosition(item).wsp_x==x && positioner.getPosition(item).wsp_y == y){
			found = 1;
			wattron(menuWindow, COLOR_PAIR(current));
			mvwprintw(
				menuWindow,
				0, positioner.getPosition(item).dx,
				"%s", (
					std::string(TOP_MARGIN_LEFT, ' ') +
					item.getLabel() +
					std::string(TOP_MARGIN_RIGHT, ' ')
					).c_str()
			);
			wattroff(menuWindow, COLOR_PAIR(current));
		}
		else {
			mvwprintw(
				menuWindow,
				0, positioner.getPosition(item).dx,
				"%s", (
					std::string(TOP_MARGIN_LEFT, ' ') +
					item.getLabel() +
					std::string(TOP_MARGIN_RIGHT, ' ')
					).c_str()
			);
		}
		
		//wattroff(menuWindow, COLOR_PAIR(current));
		//std::swap(current, next);
	}
	//wattron(menuWindow, COLOR_PAIR(current));
	wprintw(menuWindow, std::string(COLS - getcurx(menuWindow), ' ').c_str());
	//wattroff(menuWindow, COLOR_PAIR(current));
	wrefresh(menuWindow);
	return found;
}

void Menu::renderTop()
{
	wclear(menuWindow);
	wrefresh(menuWindow);
	
}
void Menu::renderOptions()
{
	renderTop2();
	if (optionsWindow != nullptr)
		hideOptions();

	auto selected = getSelected();
	if (selected == nullptr || selected->getItems().size() == 0) {
		hideOptions();
		return;
	}

	auto &firstOption = selected->getItems().front();
	auto &firstOptionPosition = positioner.getPosition(firstOption);
	int width = firstOptionPosition.width,
		height = selected->getItems().size(),
		dx = firstOptionPosition.dx,
		dy = firstOptionPosition.dy;
	int yy = 0;
	for (auto& item : selected->getItems()) { yy++; }
	optionsWindow = newwin(height, width, LINES-yy-1, dx);

	//short current = SUBMENU_ODD_PAIR, next = SUBMENU_EVEN_PAIR;
	for (auto &item : selected->getItems()) {
		auto &position = positioner.getPosition(item);
		int leftMargin = OPTION_MARGIN_LEFT,
			rightMargin = position.width - leftMargin - item.getLabel().size();

	//	wattron(optionsWindow, COLOR_PAIR(current));
		mvwprintw(
			optionsWindow,
			position.dy - (LINES - yy - 1)  , position.dx - dx,
			"%s", (
				std::string(leftMargin, ' ') +
				item.getLabel() +
				std::string(rightMargin, ' ')
			).c_str()
		);
		//wattroff(optionsWindow, COLOR_PAIR(current));
	//	std::swap(current, next);
	}

	wrefresh(optionsWindow);
}

int Menu::renderOptions(int x, int y)
{
	int found = 0;
	renderTop2();
	if (optionsWindow != nullptr)
		hideOptions();

	auto selected = getSelected();
	if (selected == nullptr || selected->getItems().size() == 0) {
		hideOptions();
	}

	auto& firstOption = selected->getItems().front();
	auto& firstOptionPosition = positioner.getPosition(firstOption);
	int width = firstOptionPosition.width,
		height = selected->getItems().size(),
		dx = firstOptionPosition.dx,
		dy = firstOptionPosition.dy;
	int yy = 0;
	for (auto& item : selected->getItems()) { yy++; }
	optionsWindow = newwin(height, width, LINES - yy - 1, dx);

	short current = SUBMENU_ODD_PAIR, next = SUBMENU_EVEN_PAIR;
	for (auto& item : selected->getItems()) {
		auto& position = positioner.getPosition(item);
		int leftMargin = OPTION_MARGIN_LEFT,
			rightMargin = position.width - leftMargin - item.getLabel().size();
		short current = MENU_ODD_PAIR, next = MENU_EVEN_PAIR;

		//	wattron(optionsWindow, COLOR_PAIR(current));
		if (positioner.getPosition(item).wsp_x == x && positioner.getPosition(item).wsp_y == y) {
			found = 1;
			wattron(optionsWindow, COLOR_PAIR(current));
			mvwprintw(
				optionsWindow,
				position.dy - (LINES - yy - 1), position.dx - dx,
				"%s", (
					std::string(leftMargin, ' ') +
					item.getLabel() +
					std::string(rightMargin, ' ')
					).c_str()
			);
			wattroff(optionsWindow, COLOR_PAIR(current));
		}
		else {
			mvwprintw(
				optionsWindow,
				position.dy - (LINES - yy - 1), position.dx - dx,
				"%s", (
					std::string(leftMargin, ' ') +
					item.getLabel() +
					std::string(rightMargin, ' ')
					).c_str()
			);
		}
		//wattroff(optionsWindow, COLOR_PAIR(current));
	//	std::swap(current, next);
	}

	wrefresh(optionsWindow);
	return found;
}

void Menu::hideOptions()
{
	if (optionsWindow == nullptr)
		return;

	wclear(optionsWindow);
	wrefresh(optionsWindow);
	touchwin(toolWindow);
	wrefresh(toolWindow);
	delwin(optionsWindow);
	optionsWindow = nullptr;
}