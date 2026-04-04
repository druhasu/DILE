// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLMoverComponent.h"

bool UDLMoverComponent::IsFalling() const
{
    return HasGameplayTag(Mover_IsFalling, true);
}

bool UDLMoverComponent::IsOnGround() const
{
    return HasGameplayTag(Mover_IsOnGround, true);
}
