#pragma once

#include "Object.h"

class CuiActionManager : BaseHookedObject {
public:
	static bool addAction(const soe::string& id, Object* action, bool own) {
		return runStatic<0x008CC650, bool, const soe::string&, Object*, bool>(id, action, own);
	}
};