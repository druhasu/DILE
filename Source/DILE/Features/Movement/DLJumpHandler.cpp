// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLJumpHandler.h"

#include "DefaultMovementSet/InstantMovementEffects/BasicInstantMovementEffects.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "MoverComponent.h"

void UDLJumpHandler::InitDependencies(UMoverComponent* InMoverComponent)
{
    MoverComponent = InMoverComponent;
    InMoverComponent->OnPreSimulationTick.AddDynamic(this, &ThisClass::OnPreSimulationTick);
}

void UDLJumpHandler::OnPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd)
{
    const FCharacterDefaultInputs* CharacterInputs = InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();

    const bool bCanJump = MoverComponent->HasGameplayTag(Mover_IsOnGround, true);
    if (CharacterInputs && CharacterInputs->bIsJumpJustPressed && bCanJump)
    {
        if (const UCommonLegacyMovementSettings* CommonSettings = MoverComponent->FindSharedSettings<UCommonLegacyMovementSettings>())
        {
            TSharedPtr<FJumpImpulseEffect> JumpMove = MakeShared<FJumpImpulseEffect>();
            JumpMove->UpwardsSpeed = CommonSettings->JumpUpwardsSpeed;

            MoverComponent->QueueInstantMovementEffect(JumpMove);
        }
    }
}
