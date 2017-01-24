#pragma once

#include "Object.h"

class ConfigClientGame : public BaseHookedObject {
public:
	DEFINE_CLIENT_STATIC(float, ms_freeChaseCameraMaximumZoom);

	static float getFreeChaseCameraMaximumZoom() {
		return ms_freeChaseCameraMaximumZoom;
	}

	static void setFreeChaseCameraMaximumZoom(float val) {
		ms_freeChaseCameraMaximumZoom = val;
	}
};