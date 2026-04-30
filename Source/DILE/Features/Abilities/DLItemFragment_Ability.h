// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Ability.generated.h"

class UDLGameplayAbility;

USTRUCT()
struct FDLItemFragment_Ability : public FDLItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TSubclassOf<UDLGameplayAbility> Ability;
};
