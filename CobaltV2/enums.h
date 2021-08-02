#pragma once

namespace {
	int VEH = 0; // VectoredExceptionHandler
	int MH = 1; // MinHook
	int KIERO = 2; // DX9, DX10, Vulkan, DX11, DX12, OpenGl, etc.
}

namespace {
	bool InitVEH = false;
	bool InitMH = false;
	bool InitKiero = false;
}

namespace {
	int S13 = 0;
	int PROD = 1;
}