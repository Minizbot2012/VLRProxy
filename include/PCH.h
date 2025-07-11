#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <type_traits>
#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <windows.h>
#include <ClibUtil/editorID.hpp>
#include <REX/REX.h>
namespace logger = SKSE::log;
using namespace clib_util;
using namespace REX;
using namespace std::literals;
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

#include <Hooking.h>