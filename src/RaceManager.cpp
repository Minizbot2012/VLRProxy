#include <Externals/MMSF_API.h>
#include <RaceManager.h>
#include <algorithm>
#include <format>
namespace MPL::Managers
{
    void RaceManager::InitMMSF()
    {
        this->MMSF = MPL::API::MMSF::RequestMMSFAPI();
    }

    void RaceManager::InitLords()
    {
        if (this->lords_initialized) return;
        std::lock_guard _guard(this->_lock);
        if (!this->lords_initialized)
        {
            this->lords_initialized = true;
            auto TDH = RE::TESDataHandler::GetSingleton();
            auto races = TDH->GetFormArray(RE::FormType::Race);
            this->OriginalVL = RE::TESForm::LookupByEditorID<RE::TESRace>("DLC1VampireBeastRace");
            for (auto frm : races)
            {
                auto race = frm->As<RE::TESRace>();
                if (race->keywords != nullptr && race->HasKeywordString("Vampire") && !race->HasKeywordString("VampireLord") && !race->HasKeywordString("HVL_Ignore") && race != this->OriginalVL)
                {
                    std::string temp_edid = race->GetFormEditorID();
                    temp_edid.erase(temp_edid.length() - 7);
                    auto humanRace = RE::TESForm::LookupByEditorID<RE::TESRace>(temp_edid);
                    if (humanRace)
                    {
                        auto edid = std::format("{}Lord", race->GetFormEditorID());
                        auto form = this->MMSF->AllocateForm(edid, RE::FormType::Race)->As<RE::TESRace>();
                        form->SetFullName(this->OriginalVL->GetFullName());
                        form->clampFaceGeoValue = race->clampFaceGeoValue;
                        form->clampFaceGeoValue2 = race->clampFaceGeoValue2;
                        form->corpseOpenSound = race->corpseOpenSound;
                        form->corpseCloseSound = race->corpseCloseSound;
                        form->skin = race->skin;
                        form->keywords = this->OriginalVL->keywords;
                        form->numKeywords = this->OriginalVL->numKeywords;
                        form->dismemberBlood = race->dismemberBlood;
                        form->validEquipTypes = race->validEquipTypes;
                        if (race->armorParentRace)
                        {
                            form->armorParentRace = race->armorParentRace;
                        }
                        else
                        {
                            form->armorParentRace = race->morphRace;
                        }
                        form->morphRace = race->morphRace;
                        form->bodyPartData = race->bodyPartData;
                        form->phonemeTargets = race->phonemeTargets;
                        form->attackDataMap = this->OriginalVL->attackDataMap;
                        form->impactDataSet = this->OriginalVL->impactDataSet;
                        form->unarmedEquipSlot = this->OriginalVL->unarmedEquipSlot;
                        form->bipedModelData = race->bipedModelData;
                        form->data = race->data;
                        form->data.flags.set(RE::RACE_DATA::Flag::kNoKnockdowns);
                        form->unk280 = race->unk280;
                        form->unk298 = race->unk298;
                        form->unk2B0 = race->unk2B0;
                        form->unk2B8 = race->unk2B8;
                        form->unk2C0 = race->unk2C0;
                        form->unk2C8 = race->unk2C8;
                        form->unk42C = race->unk42C;
                        form->unk448 = race->unk448;
                        form->actorEffects = this->OriginalVL->actorEffects;
                        for (int i = 0; i < 32; i++)
                        {
                            form->bipedObjectNameA[i] = race->bipedObjectNameA[i];
                        }
                        for (int i = 0; i < RE::TESRace::MovementTypes::kTotal; i++)
                        {
                            form->baseMoveTypes[i] = this->OriginalVL->baseMoveTypes[i];
                        }
                        form->data.weight[0] = race->data.weight[0];
                        form->data.weight[1] = race->data.weight[1];
                        form->data.height[0] = race->data.height[0];
                        form->data.height[1] = race->data.height[1];
                        form->rootBehaviorGraphNames[0] = this->OriginalVL->rootBehaviorGraphNames[0];
                        form->rootBehaviorGraphNames[1] = this->OriginalVL->rootBehaviorGraphNames[1];
                        form->behaviorGraphProjectNames[0] = this->OriginalVL->behaviorGraphProjectNames[0];
                        form->behaviorGraphProjectNames[1] = this->OriginalVL->behaviorGraphProjectNames[1];
                        form->behaviorGraphs[0] = this->OriginalVL->behaviorGraphs[0];
                        form->behaviorGraphs[1] = this->OriginalVL->behaviorGraphs[1];
                        form->attackAnimationArrayMap[0] = this->OriginalVL->attackAnimationArrayMap[0];
                        form->attackAnimationArrayMap[1] = this->OriginalVL->attackAnimationArrayMap[1];
                        form->defaultVoiceTypes[0] = race->defaultVoiceTypes[0];
                        form->defaultVoiceTypes[1] = race->defaultVoiceTypes[1];
                        form->faceRelatedData[0] = race->faceRelatedData[0];
                        form->faceRelatedData[1] = race->faceRelatedData[1];
                        form->decapitateArmors[0] = race->decapitateArmors[0];
                        form->decapitateArmors[1] = race->decapitateArmors[1];
                        form->bodyTextureModels[0] = race->bodyTextureModels[0];
                        form->bodyTextureModels[1] = race->bodyTextureModels[1];
                        form->defaultVoiceTypes[0] = race->defaultVoiceTypes[0];
                        form->defaultVoiceTypes[1] = race->defaultVoiceTypes[1];
                        form->skeletonModels[0] = race->skeletonModels[0];
                        form->skeletonModels[1] = race->skeletonModels[1];
                        form->bloodImpactMaterial = race->bloodImpactMaterial;
                        Managers::RaceData rd{
                            .vampireRace = race,
                            .vlRace = form,
                            .humanRace = humanRace
                        };
                        this->PushRaceData(rd);
                    }
                }
            }
        }
    }
    int RaceManager::PushRaceData(RaceData& rd)
    {
        logger::info("Adding vampire lord {} for vampire {} and human {}",
            rd.vlRace->GetFormEditorID(),
            rd.vampireRace->GetFormEditorID(),
            rd.humanRace->GetFormEditorID());
        this->race_pairs.push_back(rd);
        return 0;
    }
    auto RaceManager::GetLordRace(RE::TESRace* rc) -> RE::TESRace*
    {
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rd) { return rd.vampireRace == rc || rd.humanRace == rc; });
        if (it != this->race_pairs.end())
        {
            return it->vlRace;
        }
        else
        {
            return this->OriginalVL;
        }
    }

    auto RaceManager::GetVampireRace(RE::TESRace* rc) -> RE::TESRace*
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
            return nullptr;
        }
    }

    auto RaceManager::GetOriginalLord() -> RE::TESRace*
    {
        return this->OriginalVL;
    }

    bool RaceManager::IsVampireLord(RE::TESRace* rc)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rn) { return rn.vlRace == rc; }) !=
                   this->race_pairs.end() ||
               rc == this->OriginalVL;
    }

    bool RaceManager::IsSupportedRace(RE::TESRace* race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vampireRace; }) !=
               this->race_pairs.end();
    }

    bool RaceManager::IsSupportedLord(RE::TESRace* race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vlRace; }) !=
               this->race_pairs.end();
    }
    void RaceManager::AttachWings(RE::Actor* actor)
    {
        if (!actor) return;
        if (!this->transforms.contains(actor->GetDisplayFullName())) return;
        auto& transform = this->transforms[actor->GetDisplayFullName()];
        if (!transform.artObject) {
            auto artObject = this->MMSF->AllocateForm(std::format("{}_Wings", actor->GetActorBase()->GetFormEditorID()), RE::FormType::ArtObject);
            if (!artObject) return;
            auto effect = artObject->As<RE::BGSArtObject>();
            if (!effect) return;
            effect->data.artType = RE::BGSArtObject::ArtType::kMagicCastingArt;
            effect->model = transform.wingPath;
            transform.artObject = effect;
        }
        if (!transform.artObject) return;
        transform.artObject->model = transform.wingPath;
        actor->ApplyArtObject(transform.artObject);
        logger::info("Attached wings to actor {}", actor->GetActorBase()->GetFormEditorID());
    };

    void RaceManager::DetachWings(RE::Actor* actor)
    {
        if (!actor) return;
        if (!this->transforms.contains(actor->GetDisplayFullName())) return;
        auto& transform = this->transforms[actor->GetDisplayFullName()];
        if (!transform.artObject) return;
        SKSE::GetTaskInterface()->AddTask([&]() {
            auto procList = RE::ProcessLists::GetSingleton();
            for (auto effect : procList->globalTempEffects)
            {
                if (auto* mrf = effect->As<RE::ModelReferenceEffect>(); mrf)
                {
                    if (mrf->artObject == transform.artObject && mrf->target.get().get() == actor)
                    {
                        mrf->Detach();
                        mrf->finished = true;
                        logger::info("Detached wings from actor {}", actor->GetActorBase()->GetFormEditorID());
                        break;
                    }
                }
            }
        });
    }

    void RaceManager::ClearTransforms()
    {
        for (auto& [npc, transform] : this->transforms)
        {
            transform.artObject = nullptr;
        }
    }
}  // namespace MPL::Managers
