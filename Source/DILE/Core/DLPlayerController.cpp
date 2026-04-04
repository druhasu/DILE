// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLPlayerController.h"
#include "DLCameraManager.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

ADLPlayerController::ADLPlayerController()
{
    // set the player camera manager class
    PlayerCameraManagerClass = ADLCameraManager::StaticClass();
}

void ADLPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ADLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // only add IMCs for local player controllers
    if (IsLocalPlayerController())
    {
        // Add Input Mapping Context
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
            {
                Subsystem->AddMappingContext(CurrentContext, 0);
            }
        }
    }
}
