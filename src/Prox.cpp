#include <Papyrus.h>
#include <Hook.h>
#include <Config.h>
#include <DOMManager.h>
#include <RaceManager.h>

void OnInit(SKSE::MessagingInterface::Message *a_msg)
{
	switch (a_msg->type)
	{
	case SKSE::MessagingInterface::kDataLoaded:
		vlrp::config::LoadConfigs();
		vlrp::hook::TryInstall();
		vlrp::managers::DOMManager::Register();
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		if (vlrp::managers::RaceManager::GetSingleton()->IsVampireLord(RE::PlayerCharacter::GetSingleton()->GetRace()))
		{
			vlrp::managers::RaceManager::GetSingleton()->SetDOMRace(RE::PlayerCharacter::GetSingleton()->GetRace());
		}
		break;
	default:
		break;
	}
}

extern "C" DLLEXPORT auto SKSEPlugin_Version = []()
{
	SKSE::PluginVersionData v;
	v.PluginVersion({1, 0, 0, 0});
	v.PluginName("VLRProxy");
	v.AuthorName("mini");
	v.UsesAddressLibrary();
	v.UsesNoStructs();

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface *a_skse)
{
	SKSE::Init(a_skse);
	logger::info("Game version : {}", a_skse->RuntimeVersion().string());
	SKSE::GetMessagingInterface()->RegisterListener(OnInit);
	SKSE::GetPapyrusInterface()->Register(vlrp::papyrus::Bind);
	return true;
}