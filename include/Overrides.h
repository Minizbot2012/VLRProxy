namespace MPL::Managers
{
    struct OverrideEntry
    {
        RE::TESRace* source_race;
        std::string_view object_name;
        std::string_view function_name;
        auto operator<=>(const OverrideEntry&) const = default;
    };
}  // namespace MPL::Managers
namespace std
{
    template <>
    struct hash<MPL::Managers::OverrideEntry>
    {
        std::size_t operator()(const MPL::Managers::OverrideEntry& t) const noexcept
        {
            std::size_t seed = 0;
            hash_combine(seed, std::hash<std::decay_t<decltype(t.source_race)>>{}(t.source_race));
            hash_combine(seed, std::hash<std::decay_t<decltype(t.function_name)>>{}(t.function_name));
            hash_combine(seed, std::hash<std::decay_t<decltype(t.object_name)>>{}(t.object_name));
            return seed;
        }
    };
}  // namespace std
