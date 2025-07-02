#include <DOMManager.h>
namespace vlrp::managers
{
    void DOMManager::Register()
    {
        if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton())
        {
            scripts->AddEventSink<RE::TESSwitchRaceCompleteEvent>(GetSingleton());
            logger::info("Registered Race Switch Event");
        }
    }

    RE::BSEventNotifyControl DOMManager::ProcessEvent(const RE::TESSwitchRaceCompleteEvent *ev, RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent> *)
    {
        if (ev->subject->IsPlayerRef())
        {
            auto rc = ev->subject->As<RE::Actor>()->GetRace();
            auto rm = vlrp::managers::RaceManager::GetSingleton();
            if (rm->IsVampireLord(rc))
            {
                rm->SetDOMRace(rc);
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}