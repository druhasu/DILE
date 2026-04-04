// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "IDLAbilityService.h"

#include "DLAbilitySystemComponent.generated.h"

UCLASS()
class UDLAbilitySystemComponent : public UAbilitySystemComponent, public IDLAbilityService
{
    GENERATED_BODY()
};
