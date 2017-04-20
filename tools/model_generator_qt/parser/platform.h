/*
 * File: platform.h
 * Desc:
 */
#ifndef GEN_CONFIG_PLATFORM_H
#define GEN_CONFIG_PLATFORM_H

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#elif __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#endif

#endif
