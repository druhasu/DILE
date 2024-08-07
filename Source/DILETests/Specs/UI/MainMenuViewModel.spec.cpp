// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Misc/AutomationTest.h"

#include "Mechanics/MainMenu/UI/MainMenuViewModel.h"

#include "Mocks/MapLoadServiceMock.h"

BEGIN_DEFINE_SPEC(FMainMenuViewModelSpec, "DILE.UI.MainMenuViewModel", EAutomationTestFlags::ClientContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
END_DEFINE_SPEC(FMainMenuViewModelSpec)

void FMainMenuViewModelSpec::Define()
{
    It("Should call OpenGameLevel only once", [this]
    {
        auto ViewModel = NewObject<UMainMenuViewModel>();
        auto MapLoadServiceMock = NewObject<UMapLoadServiceMock>();

        ViewModel->InitDependencies(MapLoadServiceMock);

        ViewModel->StartPlaying();

        TestEqual("OpenGameLevelCallCounter", MapLoadServiceMock->OpenGameLevelCallCounter, 1);
        TestEqual("OpenMainMenuLevelCallCounter", MapLoadServiceMock->OpenMainMenuLevelCallCounter, 0);
    });
}
