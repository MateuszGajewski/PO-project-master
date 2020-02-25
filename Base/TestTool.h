#pragma once

#include "BaseTool.h"

#include <unordered_map>


class TestTool final :
	public BaseTool
{
	std::unordered_map<std::string, std::string> entries;
	WINDOW *window;

public:
	TestTool();
	~TestTool() override;

	std::string getEntry(const std::string&) const override;
	void setEntry(const std::string&, const std::string&) override;
	void assignWindow(WINDOW*) override;

	void edition();
	void fill();
	void unsave();
	void param();
	void clear();
};

