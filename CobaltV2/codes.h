#pragma once

// Error codes for cobalt
/*
0-99:
Minhook Errors
100-199:
Cobalt Errors
200-299:
Kiero Errors
300-399
*/

namespace
{
	int BOOL_TO_STR_FAILED = 100;
	int FSTREAM_NOT_OPEN = 200;
	int KIERO_NOT_INITIALIZED = 300;
	int KIERO_UNKNOWN_ERROR = 399;
}