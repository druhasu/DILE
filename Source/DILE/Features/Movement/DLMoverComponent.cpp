// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLMoverComponent.h"
#include "Systems/PlayerContext/IDLPlayerControllerService.h"
#include "Utils/DLEnsure.h"

#include "GameFramework/PlayerController.h"
#include "MoverDataModelTypes.h"

void UDLMoverComponent::InitDependencies(
    TOptional<TScriptInterface<IDLPlayerControllerService>> InPlayerControllerService
)
{
    if (InPlayerControllerService.IsSet())
    {
        PlayerControllerService = InPlayerControllerService.GetValue();

        OnPostFinalize.AddDynamic(this, &ThisClass::OnFrameFinalized);
    }
}

bool UDLMoverComponent::IsFalling() const
{
    return HasGameplayTag(Mover_IsFalling, true);
}

bool UDLMoverComponent::IsOnGround() const
{
    return HasGameplayTag(Mover_IsOnGround, true);
}

void UDLMoverComponent::OnFrameFinalized(const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState)
{
    DL_ENSURE_RETURN(PlayerControllerService != nullptr);

    if (const FCharacterDefaultInputs* Inputs = GetLastInputCmd().InputCollection.FindDataByType<FCharacterDefaultInputs>())
    {
        // propagate ControlRotation received from a client to the PlayerController
        // CharacterMovementComponent does this by default, but Mover doesn't
        PlayerControllerService->GetPlayerController()->SetControlRotation(Inputs->ControlRotation);
    }
}
