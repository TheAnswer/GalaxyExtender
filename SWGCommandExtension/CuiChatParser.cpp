#include "stdafx.h"

#include <windows.h>
#include <wchar.h>

#include "CuiChatParser.h"
#include "CuiMediatorFactory.h"
#include "SwgCuiConsole.h"
#include "EmuCommandParser.h"

template<typename String, typename Vector>
void split(const String& s, Vector& v) {
	wchar_t* buffer;
	wchar_t input[128];

	wcscpy_s(input, sizeof(input), s.c_str());

	std::size_t length = sizeof(input);

	auto token = wcstok_s(static_cast<wchar_t*>(input), L" ", &buffer);

	while (token) {
		v.emplace_back(token);

		token = wcstok_s(nullptr, L" ", &buffer);
	}
}

bool CuiChatParser::parse(const soe::unicode& incomingCommand, soe::unicode& resultUnicode, uint32_t chatRoomID, bool useChatRoom) {
	CuiMediator* console = CuiMediatorFactory::get("Console");

	bool consoleActive = console ? console->isActive() : false;

	if (consoleActive)
		resultUnicode += (L"\\#ffffff > \\#888888" + incomingCommand + L"\\#ffffff\n");

	auto foundSlash = incomingCommand.find(L"/");
	if (foundSlash != soe::unicode::npos) {
		// Strip the slash.
		auto command = incomingCommand.substr(foundSlash + 1);

		if (command == L"console") {
			if (console == nullptr) {
				typedef CuiMediatorFactory::Constructor<SwgCuiConsole> ctor_t;

				auto ctor = new ctor_t(("/Console"));
				CuiMediatorFactory::addConstructor("Console", ctor);

				console = CuiMediatorFactory::get("Console");

				if (console != nullptr) {
					resultUnicode += L"swgemu console installed";

					CuiMediatorFactory::activate("Console");
				} else {
					resultUnicode += "could not find console in cui mediator factory";
				}
			} else {
				CuiMediatorFactory::toggle("Console");
			}

			return true;
		} else if (command == L"exit") {
			if (consoleActive) {
				CuiMediatorFactory::toggle("Console");

				return true;
			}
		} else if ((command.size() == 3 && command == L"emu") || (command.find(L"emu ") == 0)) {
			if (!consoleActive) {
				soe::vector<soe::unicode> args;
				split(command, args);

				EmuCommandParser::parse(args, command, resultUnicode);

				return true;
			}
		}
	}

	return originalParse::run(incomingCommand, resultUnicode, chatRoomID, useChatRoom);
}
