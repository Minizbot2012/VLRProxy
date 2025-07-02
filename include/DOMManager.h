#pragma once
#include <RaceManager.h>
namespace vlrp::managers
{
    class DOMManager : public ISingleton<DOMManager>,
                       public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
    {
    public:
        static void Register();

    protected:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSwitchRaceCompleteEvent *a_event, RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent> *) override;
    };
}