// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Misc/AutomationTest.h"

#include "DILE/Private/Services/MapLoadServiceImpl.h"

#include "Commands/WaitWorldTravelLatentCommand.h"
#include "Utils/TempWorldHelper.h"

BEGIN_DEFINE_SPEC(FMapLoadServiceSpec, "DILE.Services.MapLoadService", EAutomationTestFlags::ClientContext | EAutomationTestFlags::ServerContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
void TestCommon(TFunction<void(UMapLoadServiceImpl*)> Action, const TSoftObjectPtr<UWorld>& ExpectedWorld);
END_DEFINE_SPEC(FMapLoadServiceSpec)

void FMapLoadServiceSpec::Define()
{
    It("Should have valid MainMenu level", [this]
    {
        TestTrue("Valid MainMenu level", !GetDefault<UMapLoadServiceSettings>()->MainMenuMap.IsNull());
    });

    It("Should have valid Game level", [this]
    {
        TestTrue("Valid Game level", !GetDefault<UMapLoadServiceSettings>()->GameMap.IsNull());
    });

    It("Should open MainMenu level", [this]
    {
        TestCommon(
            [](UMapLoadServiceImpl* Service) { Service->OpenMainMenuLevel(); },
            GetDefault<UMapLoadServiceSettings>()->MainMenuMap
        );
    });

    It("Should open Game level", [this]
    {
        TestCommon(
            [](UMapLoadServiceImpl* Service) { Service->OpenGameLevel(); },
            GetDefault<UMapLoadServiceSettings>()->GameMap
        );
    });
}

void FMapLoadServiceSpec::TestCommon(TFunction<void(UMapLoadServiceImpl*)> Action, const TSoftObjectPtr<UWorld>& ExpectedWorld)
{
    // make shared FTempWorldHelper, because we need to capture it inside lambda
    auto Helper = MakeShared<FTempWorldHelper>();
    Helper->WorldContext->OwningGameInstance = NewObject<UGameInstance>();

    // create and initialize service, which we are going to test
    auto Service = NewObject<UMapLoadServiceImpl>(Helper->GetWorld());
    Service->InitDependencies(GetMutableDefault<UMapLoadServiceSettings>());

    // perform required action
    Action(Service);

    // wait until loading is finished
    ADD_LATENT_AUTOMATION_COMMAND(FWaitWorldTravelCommand(Helper->GetWorld()));

    // check that correct level was loaded
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Helper, ExpectedWorld]()
    {
        TestEqual("LevelName", ExpectedWorld, TSoftObjectPtr<UWorld>(Helper->GetWorld()));
        return true;
    }));
}
