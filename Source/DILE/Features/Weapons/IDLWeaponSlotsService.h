// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "NativeGameplayTags.h"

#include "IDLWeaponSlotsService.generated.h"

class UDLEquipmentSlot;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_EquipmentSlot_Weapon);

UINTERFACE(MinimalApi)
class UDLWeaponSlotsService : public UInterface { GENERATED_BODY() };

/*
 * Provides access to Weapon slot selection.
 * Selected slot determines which weapon is in Player's hands.
 */
class DILE_API IDLWeaponSlotsService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(int32 NewSlotIndex)>;

    /* All available Weapon slots */
    virtual TConstArrayView<const UDLEquipmentSlot*> GetSlots() const = 0;

    /* Currently selected Weapon slot index, or INDEX_NONE if not selected */
    virtual int32 GetSelectedSlotIndex() const = 0;

    /* Selects new Weapon slot */
    virtual void SetSelectedSlotIndex(int32 SlotIndex) = 0;

    /* Called when selected Weapon slot changes */
    virtual FChangedSignature& OnSelectedSlotChanged() = 0;

    /* Currently selected Weapon slot, or nullptr if not selected */
    const UDLEquipmentSlot* GetSelectedSlot() const
    {
        if (int32 Index = GetSelectedSlotIndex(); Index != INDEX_NONE)
            return GetSlots()[Index];

        return nullptr;
    }
};
