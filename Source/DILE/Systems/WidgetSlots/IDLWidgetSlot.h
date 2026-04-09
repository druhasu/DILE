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
    /* Tag of this Slot */
    virtual FGameplayTag GetSlotTag() const = 0;

    /* All widgets that are added to this Slot */
    virtual TArrayView<TObjectPtr<UWidget>> GetWidgets() const = 0;

    /* Adds widget to this Slot with given priority. Widgets with higher priority will be added on top
     * Returns Task that is completed when widget has finished its Appear animation */
    virtual FDLTask AddWidget(UWidget* Widget, float Priority = 0.f) = 0;

    /* Removes widget from this Slot
     * Returns Task that is completed when widget has finished its Disappear animation */
    virtual FDLTask RemoveWidget(UWidget* Widget) = 0;
};
