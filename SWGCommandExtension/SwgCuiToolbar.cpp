#include "stdafx.h"

#include "SwgCuiToolbar.h"
#include "UIPage.h"
#include "CuiActionManager.h"

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

SwgCuiToolbar* SwgCuiToolbar::ctor(UIPage* page, int sceneType) {
	originalCtor::run(this, page, sceneType);

	int newNumberOfButtons = getNumberOfToolbarButtons();

	for (int i = DEFAULT_NUMBER_OF_BUTTONS; i < newNumberOfButtons; ++i) {
		const char* format = "toolbarSlot%d";
		char str[64];

		sprintf_s(str, sizeof(str), format, i);

		CuiActionManager::addAction(str, getAction(), false);
	}

	return this;
}

int SwgCuiToolbar::getNumberOfToolbarButtons() {
	static const int val = MAX(ConfigFile::getKeyInt("GalaxyExtender", "toolbarButtons", DEFAULT_NUMBER_OF_BUTTONS), DEFAULT_NUMBER_OF_BUTTONS);

	return val;
}

bool SwgCuiToolbarAction::performAction(const soe::string& id, const soe::unicode& u) {
	int newNumberOfButtons = SwgCuiToolbar::getNumberOfToolbarButtons();

	for (int i = SwgCuiToolbar::DEFAULT_NUMBER_OF_BUTTONS; i < newNumberOfButtons; ++i) {
		const char* format = "toolbarSlot%d";
		char str[64];

		sprintf_s(str, sizeof(str), format, i);

		if (id == str) {
			getToolbar()->performToolbarAction(i);

			return true;
		}
	}

	return originalPerformAction::run(this, id, u);
}