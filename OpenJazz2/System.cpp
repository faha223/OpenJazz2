#include "System.h"
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

void System::LogError(const char *format, ...)
{
	if (format == nullptr)
		return;
	char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buffer, 1023, format, ap);
	va_end(ap);

#if defined(_WIN32)|| defined(_WIN64)
	OutputDebugStringA(buffer);
#else
	fprintf(stderr, buffer);
#endif
}