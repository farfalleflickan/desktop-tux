#pragma once
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void fatalError(const char *why);
int isExtensionSupported(const char *extList, const char *extension);
int msleep(long msec);
uint64_t getTimeMS();