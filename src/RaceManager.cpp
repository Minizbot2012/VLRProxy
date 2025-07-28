#include <RaceManager.h>
namespace vlrp::managers
{
#ifndef SKYRIM_AE
    RE::BSEventNotifyControl RaceManager::ProcessEvent(
        [[maybe_unused]] const RE::TESSwitchRaceCompleteEvent* ev,
        [[maybe_unused]] RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>*)
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
    void RaceManager::Register()
    {
        if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton())
        {
            scripts->AddEventSink<RE::TESSwitchRaceCompleteEvent>(this);
            logger::info("Registered Race Switch Event");
        }
    }
    void RaceManager::SetDOMRace([[maybe_unused]] RE::TESRace* race)
    {
        auto dom = RE::BGSDefaultObjectManager::GetSingleton();
        dom->objects[RE::DEFAULT_OBJECT::kVampireRace] = race;
    }
#endif
    void RaceManager::Reset()
    {
        this->OriginalVL =
            RE::TESForm::LookupByEditorID<RE::TESRace>("DLC1VampireBeastRace");
        this->race_pairs.clear();
    }
    int RaceManager::PushRaceData(RaceData& rd)
    {
        std::lock_guard guard(this->_lock);
        auto vm =
            std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                [&](auto rdi) { return rdi.vampireRace == rd.vampireRace; });
        auto vlm = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rdi) { return rdi.vlRace == rd.vlRace; });
        if (vm != this->race_pairs.end())
        {
            logger::info("Overriding vampire lord {} for vampire {}",
                rd.vlRace->GetFormEditorID(),
                rd.vampireRace->GetFormEditorID());
            vm->vlRace = rd.vlRace;
            return 1;
        }
        else if (vlm != this->race_pairs.end())
        {
            logger::info("Overriding vampire {} for vampire lord {}",
                rd.vampireRace->GetFormEditorID(),
                rd.vlRace->GetFormEditorID());
            vm->vampireRace = rd.vampireRace;
            return 1;
        }
        else
        {
            logger::info("Adding vampire lord {} for vampire {}",
                rd.vlRace->GetFormEditorID(),
                rd.vampireRace->GetFormEditorID());
            this->race_pairs.push_back(rd);
            return 0;
        }
        return -1;
    }
    void RaceManager::Save(SKSE::SerializationInterface* a_intf)
    {
        if (!a_intf->OpenRecord('TNS_', 1))
        {
            logger::error("Failed to open record for actor transforms");
        }
        else
        {
            size_t num = this->transforms.size();
            if (!a_intf->WriteRecordData(&num, sizeof(size_t)))
            {
                logger::error("Failed to write transformation count");
            }
            else
            {
                for (auto& elem : this->transforms)
                {
                    a_intf->WriteRecordData(&elem.actor->formID, sizeof(RE::FormID));
                    a_intf->WriteRecordData(&elem.original_race->formID,
                        sizeof(RE::FormID));
                    a_intf->WriteRecordData(&elem.transformed_race->formID,
                        sizeof(RE::FormID));
                }
            }
        }
    }
    void RaceManager::Load(SKSE::SerializationInterface* a_intf)
    {
        uint32_t len;
        uint32_t version;
        uint32_t typ;
        if (!a_intf->GetNextRecordInfo(typ, version, len))
        {
            logger::error("Error reading record {}, {}, {}", typ, version, len);
        }
        else
        {
            switch (typ)
            {
            case 'TNS_':
                size_t count;
                if (!a_intf->ReadRecordData(&count, sizeof(size_t)))
                {
                    logger::error("Failed to read transformation count");
                }
                else
                {
                    for (size_t i = 0; i < count; i++)
                    {
                        RE::FormID actor;
                        RE::FormID original_race;
                        RE::FormID transformed_race;
                        a_intf->ReadRecordData(&actor, sizeof(RE::FormID));
                        a_intf->ReadRecordData(&original_race, sizeof(RE::FormID));
                        a_intf->ReadRecordData(&transformed_race, sizeof(RE::FormID));
                        a_intf->ResolveFormID(actor, actor);
                        a_intf->ResolveFormID(original_race, original_race);
                        a_intf->ResolveFormID(transformed_race, transformed_race);
                        auto transformed_actor = RE::TESForm::LookupByID<RE::Actor>(actor);
                        auto og_race = RE::TESForm::LookupByID<RE::TESRace>(original_race);
                        auto transformation =
                            RE::TESForm::LookupByID<RE::TESRace>(transformed_race);
                        auto actor_crace = transformed_actor->GetRace();
                        if (!transformed_actor->IsDead() && actor_crace == transformation)
                        {
                            this->transforms.push_back(TransformData{
                                transformed_actor,
                                og_race,
                                transformation,
                            });
                        }
                        else
                        {
                            logger::warn(
                                "Actor race was changed outside of revert or actor is "
                                "dead, dropping reversion {}",
                                transformed_actor->GetFormEditorID());
                        }
                    }
                }
                break;
            default:
                logger::warn("Unknown type {}", typ);
                break;
            }
        }
    }
    void RaceManager::Revert(
        [[maybe_unused]] SKSE::SerializationInterface* a_intf) {}
    auto RaceManager::GetVLRace(const RE::TESRace* rc) -> const RE::TESRace*
    {
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rd) { return rd.vampireRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vlRace;
        }
        else
        {
            return this->OriginalVL;
        }
    }
    auto RaceManager::GetVampireRace(const RE::TESRace* rc) -> const RE::TESRace*
    {
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rd) { return rd.vlRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vampireRace;
        }
        else
        {
            logger::info("DID NOT FIND HUMAN VAMPIRE RACE");
            return NULL;
        }
    }
    auto RaceManager::GetOriginalVL() -> const RE::TESRace*
    {
        return this->OriginalVL;
    }
    bool RaceManager::IsVampireLord(const RE::TESRace* rc)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rn) { return rn.vlRace == rc; }) !=
                   this->race_pairs.end() ||
               rc == this->OriginalVL;
    }
    bool RaceManager::IsSupportedRace(const RE::TESRace* race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vampireRace; }) !=
               this->race_pairs.end();
    }
    bool RaceManager::IsSupportedVL(const RE::TESRace* race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vlRace; }) !=
               this->race_pairs.end();
    }

    bool RaceManager::TransformActor(RE::Actor* actor, RE::TESRace* to_race)
    {
        std::lock_guard guard(this->_lock);
        auto not_transformed = std::find_if(this->transforms.begin(),
                                   this->transforms.end(), [&](auto tdi) {
                                       return tdi.actor == actor;
                                   }) == this->transforms.end();
        if (to_race != nullptr && not_transformed)
        {
            this->transforms.push_back(TransformData{ actor, actor->GetRace(), to_race });
            actor->SwitchRace(to_race, actor->IsPlayerRef());
            return true;
        }
        else if (this->IsSupportedRace(actor->GetRace()) && not_transformed)
        {
            auto race = const_cast<RE::TESRace*>(this->GetVLRace(actor->GetRace()));
            actor->SwitchRace(race, actor->IsPlayerRef());
            return true;
        }
        else if (not_transformed)
        {
            auto race = const_cast<RE::TESRace*>(this->OriginalVL);
            this->transforms.push_back(
                TransformData{ actor, actor->GetRace(), this->OriginalVL });
            actor->SwitchRace(race, actor->IsPlayerRef());
            return true;
        }
        return false;
    }
    bool RaceManager::RevertActor(RE::Actor* actor)
    {
        std::lock_guard guard(this->_lock);
        auto td = std::find_if(this->transforms.begin(), this->transforms.end(),
            [&](auto ti) { return ti.actor == actor; });
        if (this->IsSupportedVL(actor->GetRace()) && td == this->transforms.end())
        {
            actor->SwitchRace(
                const_cast<RE::TESRace*>(this->GetVampireRace(actor->GetRace())),
                actor->IsPlayerRef());
            return true;
        }
        else
        {
            if (td != this->transforms.end())
            {
                actor->SwitchRace(const_cast<RE::TESRace*>(td->original_race),
                    actor->IsPlayerRef());
                this->transforms.erase(td);
                return true;
            }
            else
            {
                logger::error("Actor transform not found, actor cannot revert");
                return false;
            }
        }
        logger::error("Actor transform not found, actor cannot revert");
        return false;
    }
}  // namespace vlrp::managers