// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Misc/AutomationTest.h"

#include "Features/MainMenu/UI/DLMainMenuViewModel.h"

#include "Systems/MapLoad/DLMapLoadServiceMock.h"
#include "Utils/DLTestMacros.h"

BEGIN_DEFINE_SPEC(FMainMenuViewModelSpec, "DILE.Mechanics.MainMenu.MainMenuViewModel", EAutomationTestFlags::ClientContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
END_DEFINE_SPEC(FMainMenuViewModelSpec)

void FMainMenuViewModelSpec::Define()
{
    It("Should call OpenGameLevel only once", [this]
    {
        auto ViewModel = NewObject<UDLMainMenuViewModel>();
        auto MapLoadServiceMock = NewObject<UDLMapLoadServiceMock>();

        ViewModel->InitDependencies(MapLoadServiceMock);

        ViewModel->StartPlaying();

        DileTestEqual(MapLoadServiceMock->OpenGameLevelCallCounter, 1);
        DileTestEqual(MapLoadServiceMock->OpenMainMenuLevelCallCounter, 0);
    });
}
