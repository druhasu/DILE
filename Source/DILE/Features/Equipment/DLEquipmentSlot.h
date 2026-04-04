// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "DLEquipmentSlot.generated.h"

UCLASS()
class UDLEquipmentSlot : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Slot")
    FText SlotName;

    UPROPERTY(EditDefaultsOnly, Category = "Slot", meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> SlotIcon;

    UPROPERTY(EditDefaultsOnly, Category = "Slot", meta = (Categories = "EquipmentSlot"))
    FGameplayTag EquipmentSlotTag;
};
