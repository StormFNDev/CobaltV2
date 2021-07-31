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

enum class ErrCodes
{
	FSTREAM_NOT_OPEN = 200,
	KIERO_NOT_INITIALIZED = 300,
	KIERO_UNKNOWN_ERROR = 399
};