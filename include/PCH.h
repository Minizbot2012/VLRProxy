#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define _UNICODE
// clang-format off
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/REX.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <Windows.h>
// clang-format on
#define DLLEXPORT __declspec(dllexport)
namespace logger = SKSE::log;
using namespace std::literals;
#include <Hooking.h>
inline void hash_combine(std::size_t& seed, std::size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
namespace std {
    template<typename... Ts>
    struct hash<std::pair<Ts...>> {
        std::size_t operator()(const std::tuple<Ts...>& t) const noexcept {
            std::size_t seed = 0;
            std::apply([&seed](const auto&... elem) {
                (hash_combine(seed, std::hash<std::decay_t<decltype(elem)>>{}(elem)), ...);
            }, t);
            return seed;
        }
    };
}
