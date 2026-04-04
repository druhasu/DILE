// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Systems/ObjectTracking/DLTrackedObject.h"
#include "Utils/DLTask.h"
#include "GameplayTagContainer.h"

#include "IDLWidgetSlot.generated.h"

class UWidget;

UINTERFACE(MinimalApi)
class UDLWidgetSlot : public UInterface { GENERATED_BODY() };

/*
 * Host for one or many widgets
 */
class DILE_API IDLWidgetSlot
#if CPP
    : public FDLTrackedObject
#endif
{
    GENERATED_BODY()

public:
    virtual FGameplayTag GetSlotTag() const = 0;
    virtual TArrayView<TObjectPtr<UWidget>> GetWidgets() const = 0;
    virtual FDLTask AddWidget(UWidget* Widget, float Priority = 0.f) = 0;
    virtual FDLTask RemoveWidget(UWidget* Widget) = 0;
};
