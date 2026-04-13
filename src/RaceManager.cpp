#include <RaceManager.h>
#include <SKSE/API.h>
namespace MPL::Managers
{
    void Managers::RaceManager::LoadConfig()
    {
        if (this->conf_loaded) return;
        std::lock_guard _guard(this->_lock);
        if (!this->conf_loaded)
        {
            auto TDH = RE::TESDataHandler::GetSingleton();
            auto races = TDH->GetFormArray(RE::FormType::Race);
            auto headParts = TDH->GetFormArray(RE::FormType::HeadPart);
            auto armorAddons = TDH->GetFormArray(RE::FormType::Armature);
            auto file = TDH->LookupModByName("VLRP.esp");
            if (this->baseID == 0x0)
                this->baseID = (uint32_t)file->compileIndex << 24 | (uint32_t)file->smallFileCompileIndex << 12;
            auto cfc = RE::IFormFactory::GetConcreteFormFactoryByType<RE::TESRace>();
            this->OriginalVL = RE::TESForm::LookupByEditorID<RE::TESRace>("DLC1VampireBeastRace");
            if (std::filesystem::exists("./Data/SKSE/vlrp-cache.bin"))
            {
                auto fl = rfl::msgpack::load<std::unordered_map<std::string, uint32_t>>("./Data/SKSE/vlrp-cache.bin");
                if (fl.has_value())
                {
                    this->race_formid_cache = fl.value();
                    logger::info("Loaded formid cache with {} entries", this->race_formid_cache.size());
                }
            }
            for (auto frm : races)
            {
                auto race = frm->As<RE::TESRace>();
                if (race->keywords != NULL && race->HasKeywordString("Vampire") && !race->HasKeywordString("VampireLord") && !race->HasKeywordString("HVL_Ignore"))
                {
                    auto form = cfc->Create();
                    auto edid = std::format("{}Lord", race->GetFormEditorID());
                    if (this->race_formid_cache.contains(edid))
                    {
                        auto oset = this->race_formid_cache.at(edid);
                        form->SetFormID(this->baseID | oset, false);
                        logger::info("Using formid cache with id of {:8X}", this->baseID | oset);
                        if (oset > this->offset)
                        {
                            this->offset = oset;
                        }
                    }
                    else
                    {
                        form->SetFormID(this->baseID | ++this->offset, false);
                        this->race_formid_cache[edid] = this->offset;
                    }
                    form->SetFormEditorID(edid.c_str());
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
                    form->armorParentRace = race->armorParentRace;
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
                    for (auto hp : headParts)
                    {
                        auto headPart = hp->As<RE::BGSHeadPart>();
                        if (headPart->validRaces)
                        {
                            if (headPart->validRaces->HasForm(race) && !headPart->validRaces->HasForm(form))
                            {
                                headPart->validRaces->AddForm(form);
                            }
                        }
                    }
                    form->SetFile(const_cast<RE::TESFile*>(file));
                    Managers::RaceData rd{
                        .vampireRace = race,
                        .vlRace = form
                    };
                    this->PushRaceData(rd);
                    logger::info("Generated VL Race {} {:08X} for Vampire Race {} {:06X}:{}", form->GetFormEditorID(), form->formID, race->GetFormEditorID(), race->GetLocalFormID(), race->GetFile(0)->GetFilename());
                }
            }
            logger::info("Saving formid cache with {} entries", this->race_formid_cache.size());
            rfl::msgpack::save("./Data/SKSE/vlrp-cache.bin", this->race_formid_cache);
        }
        this->conf_loaded = true;
    }
    int RaceManager::PushRaceData(RaceData& rd)
    {
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
    auto RaceManager::GetLordRace(RE::TESRace* rc) -> RE::TESRace*
    {
        this->LoadConfig();
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

    auto RaceManager::GetVampireRace(RE::TESRace* rc) -> RE::TESRace*
    {
        this->LoadConfig();
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
        this->LoadConfig();
        return this->OriginalVL;
    }

    bool RaceManager::IsVampireLord(RE::TESRace* rc)
    {
        this->LoadConfig();
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rn) { return rn.vlRace == rc; }) !=
                   this->race_pairs.end() ||
               rc == this->OriginalVL;
    }

    bool RaceManager::IsSupportedRace(RE::TESRace* race)
    {
        this->LoadConfig();
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vampireRace; }) !=
               this->race_pairs.end();
    }

    bool RaceManager::IsSupportedLord(RE::TESRace* race)
    {
        this->LoadConfig();
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vlRace; }) !=
               this->race_pairs.end();
    }
}  // namespace MPL::Managers
