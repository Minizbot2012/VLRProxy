#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "REX/W32.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <windows.h>
#include <ClibUtil/singleton.hpp>
#include <ClibUtil/editorID.hpp>
using namespace clib_util;
using namespace singleton;
#ifdef SKYRIM_AE
#define OFFSET(se, ae) ae
#define OFFSET_3(se, ae, vr) ae
#elif SKYRIMVR
#define OFFSET(se, ae) se
#define OFFSET_3(se, ae, vr) vr
#else
#define OFFSET(se, ae) se
#define OFFSET_3(se, ae, vr) se
#endif
#define DLLEXPORT __declspec(dllexport)
namespace logger = SKSE::log;
using namespace std::literals;

#include "Hooking.h"