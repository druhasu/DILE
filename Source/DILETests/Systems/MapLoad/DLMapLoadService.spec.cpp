// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Systems/MapLoad/Impl/DLMapLoadServiceImpl.h"
#include "Utils/Commands/DLWaitWorldTravelLatentCommand.h"
#include "Utils/DLTempWorldHelper.h"
#include "Utils/DLTask.h"

#include "Misc/AutomationTest.h"
#include "Engine/GameInstance.h"

BEGIN_DEFINE_SPEC(FDLMapLoadServiceSpec, "DILE.Systems.MapLoad.MapLoadService", EAutomationTestFlags::ClientContext | EAutomationTestFlags::ServerContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
void TestCommon(TFunction<void(UDLMapLoadServiceImpl*)> Action, const TSoftObjectPtr<UWorld>& ExpectedWorld);
END_DEFINE_SPEC(FDLMapLoadServiceSpec)

void FDLMapLoadServiceSpec::Define()
{
    It("Should have valid MainMenu level", [this]
    {
        TestTrue("Valid MainMenu level", !GetDefault<UDLMapLoadServiceSettings>()->MainMenuMap.IsNull());
    });

    It("Should have valid Game level", [this]
    {
        TestTrue("Valid Game level", !GetDefault<UDLMapLoadServiceSettings>()->GameMap.IsNull());
    });

    xIt("Should open MainMenu level", [this]
    {
        TestCommon(
            [](UDLMapLoadServiceImpl* Service) { Service->OpenMainMenuLevel(); },
            GetDefault<UDLMapLoadServiceSettings>()->MainMenuMap
        );
    });

    xIt("Should open Game level", [this]
    {
        TestCommon(
            [](UDLMapLoadServiceImpl* Service) { Service->OpenGameLevel(); },
            GetDefault<UDLMapLoadServiceSettings>()->GameMap
        );
    });
}

void FDLMapLoadServiceSpec::TestCommon(TFunction<void(UDLMapLoadServiceImpl*)> Action, const TSoftObjectPtr<UWorld>& ExpectedWorld)
{
    // make shared FDLTempWorldHelper, because we need to capture it inside lambda
    auto Helper = MakeShared<FDLTempWorldHelper>();
    Helper->WorldContext->OwningGameInstance = NewObject<UGameInstance>();

    // create and initialize service, which we are going to test
    auto Service = NewObject<UDLMapLoadServiceImpl>(Helper->GetWorld());
    Service->InitDependencies(GetMutableDefault<UDLMapLoadServiceSettings>());

    // perform required action
    Action(Service);

    // wait until loading is finished
    ADD_LATENT_AUTOMATION_COMMAND(FDLWaitWorldTravelCommand(Helper->GetWorld()));

    // check that correct level was loaded
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Helper, ExpectedWorld]()
    {
        TestEqual("LevelName", ExpectedWorld, TSoftObjectPtr<UWorld>(Helper->GetWorld()));
        return true;
    }));
}
