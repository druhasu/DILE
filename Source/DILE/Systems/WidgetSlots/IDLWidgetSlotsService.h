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
 *
 */
class DILE_API IDLWidgetSlotsService
{
    GENERATED_BODY()

public:
    virtual TScriptInterface<IDLWidgetSlot> GetSlot(FGameplayTag SlotTag) const = 0;
    virtual TDLTask<TScriptInterface<IDLWidgetSlot>> WaitForSlot(FGameplayTag SlotTag) = 0;
};
