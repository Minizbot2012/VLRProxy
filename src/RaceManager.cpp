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

    void RaceManager::WaitForReadySignal() {
        this->Ready.wait(false);
    }

    void RaceManager::InitLords()
    {
        if(!this->MMSF) {
            this->InitMMSF();
        }
        this->alloc = static_cast<API::MMSF::IFormAllocator*>(this->MMSF->QueryService("ALLOC"));
        if(!this->alloc) {
            logger::info("FAILED TO GET FORM ALLOCATOR");
            stl::report_and_error("FAILED TO GET FORM ALLOCATOR");
            return;
        }

        this->race_pairs.clear();
        if(!this->VampireLordKeyword) {
            this->VampireLordKeyword = alloc->AllocateForm("VampireLord", RE::FormType::Keyword)->As<RE::BGSKeyword>();
            if(!this->VampireLordKeyword) {
                logger::info("FAILED TO ALLOCATE VAMPIRE LORD KEYWORD");
            };
        }
        auto TDH = RE::TESDataHandler::GetSingleton();
        auto races = TDH->GetFormArray<RE::TESRace>();
        this->OriginalVL = TDH->LookupForm<RE::TESRace>(0x283A, "Dawnguard.esm");
        if(!this->OriginalVL->HasKeyword(this->VampireLordKeyword)) this->OriginalVL->AddKeyword(this->VampireLordKeyword);
        for (auto* race : races)
        {
            if (race->keywords != nullptr && race->HasKeywordString("Vampire") && !race->HasKeywordString("VampireLord") && !race->HasKeywordString("HVL_Ignore") && race != this->OriginalVL)
            {
                std::string temp_edid = race->GetFormEditorID();
                auto edid = std::format("{}Lord", race->GetFormEditorID());
                temp_edid.erase(temp_edid.length() - 7);
                auto humanRace = RE::TESForm::LookupByEditorID<RE::TESRace>(temp_edid);
                auto vlRace = RE::TESForm::LookupByEditorID<RE::TESRace>(edid);
                if (humanRace && !vlRace)
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
                    form->AddKeyword(this->VampireLordKeyword);
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
                    if (race->morphRace != race)
                    {
                        form->morphRace = race->morphRace;
                    }
                    else
                    {
                        form->morphRace = race;
                    }
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
                }
            }
        }
        for (auto file : TDH->files)
        {
            PatchRMInis(SLIDER_DIR / file->GetFilename());
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

    //You found it! Secret WIPs, but how does it work? (Look for something about "Harkon") This is definitely WIP ;)
    void RaceManager::AttachWings(RE::Actor* actor)
    {
        if (!actor) return;
        if (!this->transforms.contains(actor->GetDisplayFullName())) return;
        auto& transform = this->transforms[actor->GetDisplayFullName()];
        if (!transform.artObject)
        {
            auto artObject = alloc->AllocateForm(std::format("{}_Wings", actor->GetActorBase()->GetFormEditorID()), RE::FormType::ArtObject);
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
        logger::info("Attached wings to actor {}", actor->GetDisplayFullName());
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
                        logger::info("Detached wings from actor {}", actor->GetDisplayFullName());
                        break;
                    }
                }
            }
        });
    }
}  // namespace MPL::Managers
