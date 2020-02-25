#include "NiceBackend.h"



NiceBackend::NiceBackend(BaseTool *const tool) :
	BaseBackend(tool),
	isOpen(true),
	terminal(),
	input(),
	menu(terminal.getMenuWindow(), terminal.getToolWindow()),
	normalMode(input, *tool, menu, editionHandler),
	tooSmallMode(input),
	helpMode(input, bindsCollection),
	alertMode(input),
	paramMode(input, *tool),
	currentMode(&normalMode)
{
	tool->assignWindow(terminal.getToolWindow());
}


NiceBackend::~NiceBackend()
{}


void NiceBackend::bind(const std::string &command, std::function<void()> handler, const std::string &description)
{
	auto toBind = Bind(command, handler, description);
	try {
		auto parsed = toBind.getParsed();
		menu.addItem(parsed, handler);
		bindsCollection.push_back(toBind);
	}
	catch (ParsedBind::Edition&) {
		editionHandler = handler;
		bindsCollection.push_back(toBind);
	}
	catch (ParsedBind::Invalid&) {}
}


void NiceBackend::start()
{
	bind(".File.Help...", [this]() { this->helpHandler(); }, "List manual entries for each bound handler");
	bind(".File.Quit", [this]() { this->quitHandler(); }, "Close application");
	menu.setPosition();
	menu.renderTop();
	while (isOpen) {
		try {
			(*currentMode)();
		}
		catch (const OptionItem *item) {
			handleParam(item);
		}
		catch (Mode::Resize&) {
			handleResize();
		}
		catch (Mode::End&) {
			handleModeEnd();
		}
	}
}


void NiceBackend::handleResize()
{
	terminal.resize();
	try {
		menu.setPosition();
		menu.renderTop();
		currentMode = &normalMode;
		tool->assignWindow(terminal.getToolWindow());
		tool->setEntry("KEY", "<RESIZE>");
		if (editionHandler)
			editionHandler();
	}
	catch (Menu::TooSmallTerminal&) {
		currentMode = &tooSmallMode;
	}
}


void NiceBackend::handleModeEnd()
{
	terminal.restore();
	currentMode = &normalMode;
}


void NiceBackend::handleParam(const OptionItem *item)
{
	paramMode.setTarget(item);
	currentMode = &paramMode;
}


void NiceBackend::helpHandler()
{
	currentMode = &helpMode;
}


void NiceBackend::quitHandler()
{
	try {
		std::string entry = tool->getEntry("IS_SAVED");
		if (entry == "NO") {
			//alertMode.setMessage(unsavedAlert);
			alertMode.AlertM();
			currentMode = &alertMode;
			return;
		}
	}
	catch (BaseTool::UndefinedEntry&) {}

	isOpen = false;
}


const std::string NiceBackend::unsavedAlert = "Can't close unsaved file!";
