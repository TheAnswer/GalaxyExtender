#pragma once

#include "Object.h"
#include "Game.h"
#include "ConfigFile.h"

class UIPage;


class SwgCuiToolbar : public BaseHookedObject {
public:
	const static int DEFAULT_NUMBER_OF_BUTTONS = 24;

	SwgCuiToolbar* ctor(UIPage* page, int sceneType);
	DEFINE_HOOK(0x00F64830, ctor, originalCtor);

	void performToolbarAction(int slot) {
		runMethod<0x00F67990, void>(slot);
	}

	Object* getAction() {
		return getMemoryReference<Object*>(0x9C);
	}

	static int getNumberOfToolbarButtons();
};

class SwgCuiToolbarAction : public BaseHookedObject {
public:
	bool performAction(const soe::string& id, const soe::unicode& u);

	SwgCuiToolbar* getToolbar() {
		return getMemoryReference<SwgCuiToolbar*>(0x4);
	}

	DEFINE_HOOK(0x00F65260, performAction, originalPerformAction);

};