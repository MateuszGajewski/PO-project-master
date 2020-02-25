#include "NiceBackend.h"
#include "TestTool.h"


int main()
{
	TestTool *tool = new TestTool();
	BaseBackend *backend = new NiceBackend(tool);

	backend->bind(".File.New", []() {}, "");
	backend->bind(".File.New2", []() {}, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa");
	backend->bind(".File2.New", []() {}, "");
	backend->bind(".File3.New", []() {}, "");
	backend->bind(".File4.New", []() {}, "");
	backend->bind(".File5.New", []() {}, "");
	backend->bind(".Test.Fill", [&tool]() { tool->fill(); }, "");
	backend->bind(".Test.Unsave", [&tool]() {tool->unsave(); }, "");
	backend->bind(".Test.Param${Test:|param}", [&tool]() {tool->param(); }, "");
	backend->bind("#nice#<EDITION>", [&tool]() { tool->edition(); }, "");
	backend->bind("#nice#.Test.Clear", [&tool]() {tool->clear(); }, "Clear tool");

	backend->start();

	delete backend;
	delete tool;
}