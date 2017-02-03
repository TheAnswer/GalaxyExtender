#pragma once

#include "Object.h"

class ConfigFile : public BaseHookedObject {
public:
	static int getKeyInt(const char* c, const char* k, int defaultValue) {
		return runStatic<0x00A9CEA0, int>(c, k, defaultValue);
	}
};
