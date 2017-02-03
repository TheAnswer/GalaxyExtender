#pragma once


#include "Object.h"
#include "soewrappers.h"
#include "Controller.h"

#define SETFILLMODE 0x00755550

//#define GLHACKS

class Graphics : public BaseHookedObject {
	DEFINE_CLIENT_STATIC(BaseHookedObject*, ms_api);
public:
	static void setFillMode(int mode) {
		runStatic<SETFILLMODE, void>(mode);
	}

#ifdef GLHACKS
	static void setAntiAlias(bool enable);

	static bool supportsAntiAlias() {
		return ms_api->runOffset<0x1A0, bool>();
	}
#endif
};