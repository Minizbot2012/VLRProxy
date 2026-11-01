#include <Externals/MMSF_API.h>
#include <RaceManager.h>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <ostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace MPL::Managers
{
    const std::string WHITESPACE = " \n\r\t\f\v";
    // Trim from both ends
    void trim(std::string& s)
    {
        size_t start = s.find_first_not_of(WHITESPACE);
        if (start != std::string::npos)
        {
            s.erase(0, start);
        }
        else
        {
            s.clear();
        }
        size_t end = s.find_last_not_of(WHITESPACE);
        if (end != std::string::npos)
        {
            s.erase(end + 1);
        }
        else
        {
            s.clear();  // String is entirely whitespace
        }
    }
    RaceManager::RaceManager()
    {
        logger::info("Race Manager Initialized");
    }
    RaceManager::~RaceManager()
    {
    }
    void RaceManager::InitMMSF()
    {
        this->MMSF = MPL::API::MMSF::RequestMMSFAPI();
    }
    void RaceManager::PatchRMInis(std::filesystem::path drp)
    {
        std::vector<std::string> newFile;
        std::vector<std::string> oldFile;
        bool was_good = false;
        auto races = drp / "races.ini";
        {
            auto race_ini = RE::BSResourceNiBinaryStream(races.string());
            if (was_good = race_ini.good(); was_good)
            {
                std::string read;
                while (std::getline(race_ini, read))
                {
                    trim(read);
                    if (!read.empty())
                    {
                        oldFile.push_back(read);
                    }
                }
            }
        }
        for (auto read : oldFile)
        {
            if (!read.starts_with("#") && !read.empty())
            {
                auto config_parts = read | std::views::split('=') | std::ranges::to<std::vector<std::string>>();
                if (config_parts.size() == 2)
                {
                    trim(config_parts[0]);
                    trim(config_parts[1]);
                    auto humLine = std::format("{} = {}", config_parts[0], config_parts[1]);
                    auto vlLine = std::format("{}Lord = {}", config_parts[0], config_parts[1]);
                    if (!config_parts[0].ends_with("Lord") && !config_parts[0].ends_with("Vampire"))
                    {
                        newFile.push_back(humLine);
                    }
                    else if (!config_parts[0].ends_with("Lord") && config_parts[0].ends_with("Vampire"))
                    {
                        newFile.push_back(humLine);
                        newFile.push_back(vlLine);
                    }
                }
            }
            else if (read.starts_with("#"))
            {
                newFile.push_back(read);
            }
        }
        if (was_good && oldFile.size() != newFile.size())
        {
            logger::info("Patched {} racemenu Races.ini from {} to {} lines", drp.stem().string(), oldFile.size(), newFile.size());
            std::ofstream output("Data" / drp / "races.ini");
            for (auto line : newFile)
            {
                output << line << std::endl;
            }
            output.flush();
            output.close();
        }
        this->Ready.store(true);
        this->Ready.notify_all();
    }

    void RaceManager::WaitForReadySignal()
    {
        this->Ready.wait(false);
    }

    void RaceManager::InitLords()
    {
        if (!this->MMSF)
        {
            this->InitMMSF();
        }
        if (!API::MMSF::HasFeature(this->MMSF->GetVersion(), API::MMSF::MMSFAPIFeatures::kCoreService))
        {
            logger::error("MMSF CORE SERVICE NOT AVAILABLE");
            stl::report_and_fail("MMSF CORE SERVICE NOT AVAILABLE");
            return;
        }
        if (!API::MMSF::HasFeature(this->MMSF->GetVersion(), API::MMSF::MMSFAPIFeatures::kAllocator))
        {
            logger::error("MMSF ALLOCATOR NOT AVAILABLE");
            stl::report_and_fail("MMSF ALLOCATOR NOT AVAILABLE");
            return;
        }
        if (!this->alloc)
        {
            this->alloc = static_cast<API::MMSF::IFormAllocator*>(this->MMSF->QueryService("ALLOC"));
            if (!this->alloc)
            {
                logger::info("FAILED TO GET FORM ALLOCATOR");
                stl::report_and_error("FAILED TO GET FORM ALLOCATOR");
                return;
            }
        }
        auto* dataHandler = RE::TESDataHandler::GetSingleton();
        this->race_pairs.clear();
        if (!this->VampireLordKeyword)
        {
            this->VampireLordKeyword = alloc->AllocateForm("VampireLord", RE::FormType::Keyword)->As<RE::BGSKeyword>();
            if (!this->VampireLordKeyword)
            {
                logger::info("FAILED TO ALLOCATE VAMPIRE LORD KEYWORD");
            };
        }
        this->OriginalVL = dataHandler->LookupForm<RE::TESRace>(0x283A, "Dawnguard.esm");
        auto races = dataHandler->GetFormArray<RE::TESRace>();
        if (!this->OriginalVL->HasKeyword(VampireLordKeyword)) this->OriginalVL->AddKeyword(VampireLordKeyword);
        for (auto* race : races)
        {
            if (race->keywords != nullptr && race->HasKeywordString("Vampire") && !race->HasKeywordString("VampireLord") && !race->HasKeywordString("HVL_Ignore") && race != this->OriginalVL)
            {
                std::string temp_edid = race->GetFormEditorID();
                auto edid = std::format("{}Lord", race->GetFormEditorID());
                temp_edid.erase(temp_edid.length() - 7);
                auto humanRace = RE::TESForm::LookupByEditorID<RE::TESRace>(temp_edid);
                if (humanRace && race)
                {
                    auto form = alloc->AllocateForm(edid, RE::FormType::Race)->As<RE::TESRace>();
                    form->SetFullName(this->OriginalVL->GetFullName());
                    form->clampFaceGeoValue = race->clampFaceGeoValue;
                    form->clampFaceGeoValue2 = race->clampFaceGeoValue2;
                    form->corpseOpenSound = race->corpseOpenSound;
                    form->corpseCloseSound = race->corpseCloseSound;
                    form->skin = race->skin;
                    for (uint32_t i = 0; i < OriginalVL->GetNumKeywords(); i++)
                    {
                        auto keyword = OriginalVL->GetKeywordAt(i);
                        if (keyword)
                        {
                            form->AddKeyword(*keyword);
                        }
                    }
                    form->AddKeyword(VampireLordKeyword);
                    form->dismemberBlood = race->dismemberBlood;
                    form->validEquipTypes = race->validEquipTypes;
                    form->armorParentRace = race->armorParentRace ? race->armorParentRace : humanRace;
                    form->morphRace = race->morphRace ? race->morphRace : humanRace;
                    form->bodyPartData = race->bodyPartData;
                    form->phonemeTargets = race->phonemeTargets;
                    form->attackDataMap = this->OriginalVL->attackDataMap;
                    form->impactDataSet = this->OriginalVL->impactDataSet;
                    form->unarmedEquipSlot = this->OriginalVL->unarmedEquipSlot;
                    form->bipedModelData = race->bipedModelData;
                    form->data = race->data;
                    form->data.flags.set(RE::RACE_DATA::Flag::kNoKnockdowns);
                    form->data.flags.set(RE::RACE_DATA::Flag::kFaceGenHead);
                    form->unk280 = race->unk280;
                    form->unk298 = race->unk298;
                    form->unk2B0 = race->unk2B0;
                    form->unk2B8 = race->unk2B8;
                    form->unk2C0 = race->unk2C0;
                    form->unk2C8 = race->unk2C8;
                    form->unk42C = race->unk42C;
                    form->unk448 = race->unk448;
                    form->actorEffects = this->OriginalVL->actorEffects;
                    form->bloodImpactMaterial = race->bloodImpactMaterial;
                    for (unsigned int i = 0; i < RE::BIPED_OBJECT::kEditorTotal; i++)
                    {
                        form->bipedObjectNameA[i] = race->bipedObjectNameA[i];
                    }
                    for (int i = 0; i < RE::TESRace::MovementTypes::kTotal; i++)
                    {
                        form->baseMoveTypes[i] = this->OriginalVL->baseMoveTypes[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->data.weight[i] = race->data.weight[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->data.height[i] = race->data.height[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->rootBehaviorGraphNames[i] = this->OriginalVL->rootBehaviorGraphNames[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->behaviorGraphProjectNames[i] = this->OriginalVL->behaviorGraphProjectNames[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->behaviorGraphs[i] = this->OriginalVL->behaviorGraphs[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->attackAnimationArrayMap[i] = this->OriginalVL->attackAnimationArrayMap[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->defaultVoiceTypes[i] = race->defaultVoiceTypes[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->faceRelatedData[i] = race->faceRelatedData[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->decapitateArmors[i] = race->decapitateArmors[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->bodyTextureModels[i] = race->bodyTextureModels[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->bodyTextureModels[i] = race->bodyTextureModels[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->defaultVoiceTypes[i] = race->defaultVoiceTypes[i];
                    }
                    for (std::size_t i = 0; i < RE::SEXES::kTotal; ++i)
                    {
                        form->skeletonModels[i] = race->skeletonModels[i];
                    }
                    Managers::RaceData rd{
                        .vampireRace = race,
                        .vlRace = form,
                        .humanRace = humanRace
                    };
                    this->PushRaceData(rd);
                    this->AddOverride(humanRace, "racemenubase", "OnMenuInitialized", humanRace);
                    this->AddOverride(humanRace, "racemenubase", "OnMenuReinitialized", humanRace);
                    this->AddOverride(humanRace, "racemenubase", "OnReloadSettings", humanRace);
                    this->AddOverride(humanRace, "racemenubase", "On3DLoaded", humanRace);
                    this->AddOverride(humanRace, "DLC1VampireTransformVisual", "OnEffectStart", humanRace);
                    this->AddOverride(humanRace, "DLC1VampireTransformVisual", "TransformIfNecessary", humanRace);
                    this->AddOverride(humanRace, "DLC1PlayerVampireScript", "OnRaceSwitchComplete", humanRace);
                    this->AddOverride(race, "racemenubase", "OnMenuInitialized", race);
                    this->AddOverride(race, "racemenubase", "OnMenuReinitialized", race);
                    this->AddOverride(race, "racemenubase", "OnReloadSettings", race);
                    this->AddOverride(race, "racemenubase", "On3DLoaded", race);
                    this->AddOverride(race, "DLC1VampireTransformVisual", "OnEffectStart", race);
                    this->AddOverride(race, "DLC1VampireTransformVisual", "TransformIfNecessary", race);
                    this->AddOverride(race, "DLC1PlayerVampireScript", "OnRaceSwitchComplete", race);
                    this->AddOverride(form, "racemenubase", "OnMenuInitialized", race);
                    this->AddOverride(form, "racemenubase", "OnMenuReinitialized", race);
                    this->AddOverride(form, "racemenubase", "OnReloadSettings", race);
                    this->AddOverride(form, "racemenubase", "On3DLoaded", race);
                    this->AddOverride(form, "DLC1VampireTransformVisual", "OnEffectStart", this->OriginalVL);
                    this->AddOverride(form, "DLC1VampireTransformVisual", "TransformIfNecessary", this->OriginalVL);
                    this->AddOverride(form, "DLC1PlayerVampireScript", "OnRaceSwitchComplete", this->OriginalVL);
                }
            }
        }
        for (auto file : dataHandler->files)
        {
            PatchRMInis(SLIDER_DIR / file->GetFilename());
        }
    }
    RE::TESRace* RaceManager::Filter(RE::BSScript::ObjectTypeInfo* obj, RE::BSScript::IFunction* func, RE::TESRace* originalRace)
    {
        if (!obj || !func || !originalRace) return nullptr;
        auto override = OverrideEntry{ originalRace, obj->GetName(), func->GetName() };
        if (this->override_table.contains(override))
        {
            return this->override_table[override];
        }
        else if (obj->GetParent() != nullptr)
        {
            return this->Filter(obj->GetParent(), func, originalRace);
        }
        else
        {
            return nullptr;
        }
    }
    RE::TESRace* RaceManager::Filter(RE::BSScript::Internal::VirtualMachine* a_vm, RE::VMStackID stackId, RE::TESRace* originalRace)
    {
        RE::BSScript::Stack* stack;
        if (!a_vm->GetStackByID(stackId, &stack) || !stack) return nullptr;
        if (stack->top && stack->top->previousFrame)
        {
            auto frame = stack->top->previousFrame;
            //while (frame != nullptr)
            //{
            if (auto itm = Filter(frame->owningObjectType.get(), frame->owningFunction.get(), originalRace)) return itm;
            //frame = frame->previousFrame;
            //}
        }
        return nullptr;
    }
    void RaceManager::AddOverride(RE::TESRace* source_race, std::string_view owningType, std::string_view functionName, RE::TESRace* overrideRace)
    {
        this->override_table[OverrideEntry{ source_race, owningType, functionName }] = overrideRace;
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
    RE::TESRace* RaceManager::GetLordRace(RE::TESRace* rc)
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

    RE::TESRace* RaceManager::GetVampireRace(RE::TESRace* rc)
    {
        auto it = std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
            [&](auto rd) { return rd.vlRace == rc || rd.humanRace == rc; });
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

    RE::TESRace* RaceManager::GetOriginalLord()
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
                   [&](auto rd) { return race == rd.vampireRace || race == rd.humanRace; }) !=
               this->race_pairs.end();
    }

    bool RaceManager::IsSupportedLord(RE::TESRace* race)
    {
        return std::find_if(this->race_pairs.begin(), this->race_pairs.end(),
                   [&](auto rd) { return race == rd.vlRace; }) !=
               this->race_pairs.end();
    }
    void RaceManager::AttachWings([[maybe_unused]] RE::Actor* actor) {
    };

    void RaceManager::DetachWings([[maybe_unused]] RE::Actor* actor)
    {
    }
}  // namespace MPL::Managers
