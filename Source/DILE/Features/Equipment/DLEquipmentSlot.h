// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "DLEquipmentSlot.generated.h"

/*
 * Slot for Equipment
 */
UCLASS()
class UDLEquipmentSlot : public UDataAsset
{
    GENERATED_BODY()

public:
    /* Display name of this slot */
    UPROPERTY(EditDefaultsOnly, Category = "Slot")
    FText SlotName;

    /* Icon of the slot in the UI */
    UPROPERTY(EditDefaultsOnly, Category = "Slot", meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> SlotIcon;

    /* Tag of equipment slot. Used to determine which items can be equipped into it */
    UPROPERTY(EditDefaultsOnly, Category = "Slot", meta = (Categories = "EquipmentSlot"))
    FGameplayTag EquipmentSlotTag;
};
