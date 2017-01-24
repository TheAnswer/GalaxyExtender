#pragma once

#include "Object.h"
#include "soewrappers.h"

#define VIEW_DISTANCE_OFFSET 0xFC

class FreeChaseCamera : public Object {
public:
	const static uint32_t RTTI = 0x01613640;

	const float& getViewDistance() const {
		return getMemoryReference<float>(VIEW_DISTANCE_OFFSET);
	}
	
	float& getViewDistance() {
		return getMemoryReference<float>(VIEW_DISTANCE_OFFSET);
	}

	int getNumberOfSettings() {
		return getMemoryReference<int>(0x298);
	}

	float* getSettings() {
		return getMemoryReference<float*>(0x29C);
	}

	void setViewDistance(float value) {
		getViewDistance() = value;
	}

	void setZoomMultiplier(float multiplier) {
		runMethod<0x006D7EE0, void>(multiplier);
	}

	static void setCameraZoomSpeed(float val) {
		runStatic<0x006D7D20, void>(val);
	}

	static float getCameraZoomSpeed() {
		return runStatic<0x006D7D30, float>();
	}
};