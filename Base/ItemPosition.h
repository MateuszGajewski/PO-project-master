#pragma once


class ItemPosition final
{
public:
	const int width, height, dx, dy, wsp_x, wsp_y;

public:
	ItemPosition(const int width, const int height, const int dx, const int dy, const int wsp_x, const int wsp_y ) :
		width(width), height(height), dx(dx), dy(dy), wsp_x(wsp_x), wsp_y(wsp_y)
	{}
	~ItemPosition() {}

	bool clicked(const int x, const int y) const {
		return x >= dx && x < dx + width &&
			y >= dy && y < dy + height;
	}

	bool arrowed(const int x, const int y) const {
		return x == wsp_x && y == wsp_y;

	}
};
