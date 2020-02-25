#include "NormalMode.h"

int poped = 0;
int x = 0;
int y = 0;

NormalMode::NormalMode(Input &input, BaseTool &tool, Menu &menu, const std::function<void()> &editionHandler) :
	Mode(input),
	tool(tool),
	menu(menu),
	editionHandler(editionHandler)
{}


NormalMode::~NormalMode()
{}


void NormalMode::operator()()
{
	auto &inputKey = input.getKey();

	try {
		auto &key = dynamic_cast<const ResizeKey&>(inputKey);
		handle(key);
		return;
	}
	catch (std::bad_cast&) {}



	try {
		auto &key = dynamic_cast<const KeyboardKey&>(inputKey);
		if (poped == 0 && key.getCode() == 27) { menu.renderTop2(x, y); poped = 1; }
		else if (key.getCode() == 27) { menu.renderTop(); menu.hideOptions(); poped = 0; x = 0; y=0; }
		handle(key);
		return;
	}
	catch (std::bad_cast&) {}
}

void NormalMode::handle(const ResizeKey &key)
{
	throw Resize();
}





void NormalMode::handle(const KeyboardKey &key)
{
	if (poped == 1) {
		if (key.getCode() == 261) {
			x++; if (menu.renderTop2(x, y) == 0) {
				x--;  menu.renderTop2(x, y);
			}
		}
		else if (key.getCode() == 260 && x > 0) { x--; menu.renderTop2(x, y); }
		if (key.getCode() == 259 && menu.getSelected() != nullptr) { y++; if (menu.renderOptions(x, y) == 0) { y--; menu.renderOptions(x, y); } }
		else if (key.getCode() == 258 && y > 0) {
			y--; if (y != 0) { menu.renderOptions(x, y); }
			else { menu.renderOptions(x, y); menu.renderTop2(x, y); }
		}
		else if (key.getCode() == 13) {
			auto item = menu.itemAtkey(x, y);


			if (item == nullptr) {
				menu.select(nullptr);
				tool.setEntry("KEY", key.getRepresentation());
				if (editionHandler)
					editionHandler();
			}
			else if (auto topItem = dynamic_cast<const TopItem*>(item)) {
				menu.select(topItem);
			}
			else if (auto optionItem = dynamic_cast<const OptionItem*>(item)) {
				menu.select(nullptr);
				menu.hideOptions(); poped = 1; y = 0; menu.renderTop2(x, y);
				if (optionItem->getParam().empty())
					optionItem->handle();
				else throw optionItem;
			}
		}




		else {
				tool.setEntry("KEY", key.getRepresentation());
				if (editionHandler)
					editionHandler();
			}
		}
	
}