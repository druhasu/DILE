// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLTask.h"
#include "GameplayTagContainer.h"

#include "IDLWidgetSlotsService.generated.h"

class IDLWidgetSlot;

UINTERFACE(MinimalApi)
class UDLWidgetSlotsService : public UInterface { GENERATED_BODY() };

/*
 * Allows querying WidgetSlots by their tag
 */
class DILE_API IDLWidgetSlotsService
{
    GENERATED_BODY()

public:
    /* Returns WidgetSlot with given tag or nullptr, if it is not found */
    virtual TScriptInterface<IDLWidgetSlot> GetSlot(FGameplayTag SlotTag) const = 0;

    /* Returns task that completes when WidgetSlot with given tag is added to the Service */
    virtual TDLTask<TScriptInterface<IDLWidgetSlot>> WaitForSlot(FGameplayTag SlotTag) = 0;
};
