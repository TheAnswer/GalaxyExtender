#include "stdafx.h"

#include "Graphics.h"

SET_CLIENT_STATIC(Graphics::ms_api, 0x01922E6C);

#ifdef GLHACKS
void Graphics::setAntiAlias(bool enable) {
	ms_api->runOffset<0x1A4, void, bool>(enable);
}
#endif