#include "MenuPositioner.h"
#include "margins.h"



MenuPositioner::MenuPositioner(const MenuData& data, int l) :
	data(data),
	
	minWidth(0), minHeight(0)
{
	LINES = l;
}


MenuPositioner::~MenuPositioner()
{}


void MenuPositioner::setPosition()
{
	int currentDx = 0, currentDy, itemWidth, optionWidth, maxOptionWidth, wsp_x, wsp_y;
	wsp_x = 0;
	wsp_y = 0;
	for (auto &item : data.getItems()) {
		wsp_y = 0;
		itemWidth = item.getLabel().size() + TOP_MARGINS;
		position.emplace(&item, ItemPosition(itemWidth, 1, currentDx, LINES-1, wsp_x, wsp_y));

		maxOptionWidth = 0;
		for (auto &option : item.getItems()) {
			optionWidth = option.getLabel().size() + OPTION_MARGINS;
			if (optionWidth > maxOptionWidth) maxOptionWidth = optionWidth;
		}
		wsp_y = 1;
		currentDy = LINES-2;
		for (auto &option : item.getItems()) {
			position.emplace(&option, ItemPosition(maxOptionWidth, 1, currentDx, currentDy--, wsp_x, wsp_y++));
		}
		currentDx += itemWidth;
		wsp_x += 1;

		if (currentDx + maxOptionWidth > minWidth) minWidth = currentDx + maxOptionWidth;
		if (currentDy > minHeight) minHeight = currentDy;
	}
	if (currentDx > minWidth) minWidth = currentDx;
}


const Item* MenuPositioner::itemAt(const int x, const int y) const
{
	for (auto &item : data.getItems())
		if (position.at(&item).clicked(x, y)) return &item;
	auto selected = data.getSelected();
	if (selected != nullptr) {
		for (auto &item : selected->getItems())
			if (position.at(&item).clicked(x, y))
				return &item;
	}
	return nullptr;
}
const Item* MenuPositioner::itemAtkey(const int x, const int y) const
{
	for (auto &item : data.getItems())
		if (position.at(&item).arrowed(x, y)) return &item;
	auto selected = data.getSelected();
	if (selected != nullptr) {
		for (auto &item : selected->getItems())
			if (position.at(&item).arrowed(x, y))
				return &item;
	}
	return nullptr;
}
