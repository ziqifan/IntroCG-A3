#pragma once
#include <string>
#include <Windows.h>

std::string readFile(const std::string &filename);

std::string zeroPadNumber(unsigned int num, unsigned int padding);

#if _DEBUG

#define SAT_DEBUG_LOG(message, ...)			printf(message, ##__VA_ARGS__); printf("\n");

#define SAT_DEBUG_LOG_ERROR(message, ...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0C);\
											printf(message, ##__VA_ARGS__);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07); printf("\n");

#define SAT_DEBUG_LOG_WARNING(message, ...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E); printf(message, ##__VA_ARGS__);\
											SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07); printf("\n");

#define SAT_DEBUG_LOG_SAFE(message, ...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0A); printf(message, ##__VA_ARGS__);\
											SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07); printf("\n");

#else

#define SAT_DEBUG_LOG(message, ...)			((void)0)
#define SAT_DEBUG_LOG_ERROR(message, ...)	((void)0)	
#define SAT_DEBUG_LOG_WARNING(message, ...)	((void)0)
#define SAT_DEBUG_LOG_SAFE(message, ...)	((void)0)

#endif