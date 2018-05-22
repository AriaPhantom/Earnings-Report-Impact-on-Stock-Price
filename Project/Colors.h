#ifndef Color_h
#define Color_h

#include <iostream>
#include <windows.h>

static void  SetConsoleColor(WORD wAttribute)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, wAttribute);
}

inline std::ostream&  defcolor(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	return ostr;
}

inline std::ostream&  greencolor(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	return ostr;
}

inline std::ostream&  bluecolor(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	return ostr;
}

inline std::ostream&  redcolor(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	return ostr;
}

inline std::ostream&  lredcolor(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_RED);
	return ostr;
}

#endif