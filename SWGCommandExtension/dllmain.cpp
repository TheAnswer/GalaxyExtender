// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <windows.h>
#include <detours.h>

#include "CreatureObject.h"
#include "CuiMediatorFactory.h"
#include "CuiChatParser.h"
#include "Game.h"
#include "TerrainObject.h"
#include "SwgCuiLoginScreen.h"
#include "SwgCuiCommandParserDefault.h"
#include "SwgCuiMediatorFactorySetup.h"
#include "SwgCuiToolbar.h"
#include "ConfigFile.h"

using namespace std;

/// Memory Utilties
///
void writeJmp(BYTE* address, DWORD jumpTo, DWORD length) {
	DWORD oldProtect, newProtect, relativeAddress;

	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);

	relativeAddress = (DWORD)(jumpTo - (DWORD)address) - 5;
	*address = 0xE9;
	*((DWORD *)(address + 0x1)) = relativeAddress;

	for (DWORD x = 0x5; x < length; x++)
	{
		*(address + x) = 0x90;
	}

	VirtualProtect(address, length, oldProtect, &newProtect);
}

void writeBytes(BYTE* address, const BYTE* values, int size) {
	DWORD oldProtect, newProtect;

	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	memcpy(address, values, size);

	VirtualProtect(address, size, oldProtect, &newProtect);
}

#define ATTACH_HOOK(METHOD) METHOD##_hook_t::hookStorage_t::newMethod = &METHOD; \
											DetourAttach((PVOID*) &METHOD##_hook_t::hookStorage_t::original, (PVOID) METHOD##_hook_t::callHook);

#define DETACH_HOOK(METHOD) DetourDetach((PVOID*) &METHOD##_hook_t::hookStorage_t::original, (PVOID) METHOD##_hook_t::callHook);
	

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: {
		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		// Direct function hooks.
		ATTACH_HOOK(SwgCuiLoginScreen::onButtonPressed);
		ATTACH_HOOK(CuiChatParser::parse);
		ATTACH_HOOK(TerrainObject::setHighLevelOfDetailThresholdHook);
		ATTACH_HOOK(TerrainObject::setLevelOfDetailThresholdHook);
		ATTACH_HOOK(GroundScene::parseMessages);
		//ATTACH_HOOK(SwgCuiCommandParserDefault::ctorHook);
		//ATTACH_HOOK(SwgCuiCommandParserDefault::removeAliasStatic);
		ATTACH_HOOK(SwgCuiMediatorFactorySetup::install);
		ATTACH_HOOK(Transform::install);
		ATTACH_HOOK(Transform::invert);
		ATTACH_HOOK(Transform::reorthonormalize);
		ATTACH_HOOK(Transform::rotate_l2p);
		ATTACH_HOOK(Transform::rotateTranslate_l2p);
		ATTACH_HOOK(Transform::rotate_p2l);
		ATTACH_HOOK(Transform::rotateTranslate_l2pTr);
		ATTACH_HOOK(Transform::rotateTranslate_p2l);
		ATTACH_HOOK(Transform::yaw_l);
		ATTACH_HOOK(Transform::pitch_l);
		ATTACH_HOOK(Transform::roll_l);
		ATTACH_HOOK(SwgCuiToolbar::ctor);
		ATTACH_HOOK(SwgCuiToolbarAction::performAction);

		LONG errorCode = DetourTransactionCommit();

		if (errorCode == NO_ERROR) {
			//Detour successful

			const BYTE newData[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			writeBytes((BYTE*)0xC8D258, newData, 7);

			const BYTE numberOfItemsInToolbar = SwgCuiToolbar::getNumberOfToolbarButtons();
			writeBytes((BYTE*)0xf64ac5, &numberOfItemsInToolbar, 1);
			// Mid-function hooks for global detail and high detail terrain distance.
						
			// Show our loaded message (only displays if chat is already present).
			Game::debugPrintUi("[LOADED] Settings Override Extensions by N00854180T");
			Game::debugPrintUi("Use /exthelp for details on extension command usage.");
		} else {
			Game::debugPrintUi("[LOAD] FAILED");
		}

		break;
	}
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DETACH_HOOK(SwgCuiLoginScreen::onButtonPressed);
		DETACH_HOOK(CuiChatParser::parse);
		DETACH_HOOK(TerrainObject::setHighLevelOfDetailThresholdHook);
		DETACH_HOOK(TerrainObject::setLevelOfDetailThresholdHook);
		DETACH_HOOK(GroundScene::parseMessages);
		DETACH_HOOK(SwgCuiMediatorFactorySetup::install);
		DETACH_HOOK(Transform::install);
		DETACH_HOOK(Transform::invert);
		DETACH_HOOK(Transform::reorthonormalize);
		DETACH_HOOK(Transform::rotate_l2p);
		DETACH_HOOK(Transform::rotateTranslate_l2p);
		DETACH_HOOK(Transform::rotate_p2l);
		DETACH_HOOK(Transform::rotateTranslate_l2pTr);
		DETACH_HOOK(Transform::rotateTranslate_p2l);
		DETACH_HOOK(Transform::yaw_l);
		DETACH_HOOK(Transform::pitch_l);
		DETACH_HOOK(Transform::roll_l);
		DETACH_HOOK(SwgCuiToolbar::ctor);
		DETACH_HOOK(SwgCuiToolbarAction::performAction);

		DetourTransactionCommit();
		break;
	}

	return TRUE;
}

