#pragma once

#ifdef HZ_PLATFORM_WINDOWS
        #ifdef HZ_BUILD_DLL
                #define HAZEL_API __declspec(dllexport)
        #else
                #define HAZEL_API __declspec(dllimport)
        #endif

        #ifdef HZ_ENABLE_ASSERTS
                #define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
                #define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
        #else
                #define HZ_ASSERT(x, ...)
                #define HZ_CORE_ASSERT(x, ...)
        #endif
#else 
        #if defined(HZ_PLATFORM_LINUX) 
                #ifdef HZ_BUILD_DLL
                        #define HAZEL_API __attribute__((visibility ("default")))
                #else
                        #define HAZEL_API
                #endif

                #ifdef HZ_ENABLE_ASSERTS
                        #define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_trap(); } }
                        #define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_trap(); } }
                #else
                        #define HZ_ASSERT(x, ...)
                        #define HZ_CORE_ASSERT(x, ...)
                #endif

        #else
                #error Unsopported Operating System!
        #endif

#endif



#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)