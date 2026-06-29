#pragma once
#include "pch.h"

#if defined(SEED_PLATFORM_WINDOWS)
#ifdef SEED_BUILD
#define SEED_API __declspec(dllexport)
#else
#define SEED_API __declspec(dllimport)
#endif
#elif defined(SEED_PLATFORM_LINUX)
#define SEED_API
#else
#error "Seed only supports Windows and Linux platforms"
#endif

#ifdef SEED_ENABLE_ASSERTS
#define SEED_ASSERT(x, ...)                                                    \
    {                                                                          \
        if (!(x)) {                                                            \
            Seed_Error("Assertion Failed: " #x __VA_OPT__(" - ") __VA_ARGS__); \
        }                                                                      \
    }
#define SEED_CORE_ASSERT(x, ...)                                               \
    {                                                                          \
        if (!(x)) {                                                            \
            Seed_Fatal("Assertion Failed: " #x __VA_OPT__(" - ") __VA_ARGS__); \
        }                                                                      \
    }
#else
#define SEED_ASSERT(x, ...)
#define SEED_CORE_ASSERT(x, ...)
#endif

// macros
#define BIT(x) (1 << x) // for event bits.
#define SEED_BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)
