// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Systems/ContentPresentation/DLViewContent.h"
#include "Utils/DLTask.h"

#include "GameplayTagContainer.h"

#include "IDLWidgetDisplayService.generated.h"

class UDLUserWidget;

UINTERFACE(MinimalApi, NotBlueprintable)
class UDLWidgetDisplayService : public UInterface { GENERATED_BODY() };

/*
 * Allows creating and adding widgets to slots
 */
class DILE_API IDLWidgetDisplayService
{
    GENERATED_BODY()

public:
    /*
     * Creates new widget and adds it to a slot
     * @param SlotTag - Tag of a WidgetSlot to add new Widget to
     * @param WidgetClass - class of a widget to create
     * @param Content - optional Content to pass to a newly created Widget
     * @param bWaitForAnimation - whether returned task will wait until Appear animation of the widget is finished
     * @result Newly created widget
     */
    virtual TDLTask<UDLUserWidget*> DisplayWidget(FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content, bool bWaitForAnimation = false) = 0;

    /*
     * Closes widget in a WidgetSlot with given Tag
     * @param SlotTag - Tag of a WidgetSlot to add new Widget to
     * @param bWaitForAnimation - whether returned task will wait until Disappear animation of the widget is finished
     */
    virtual FDLTask CloseWidget(FGameplayTag SlotTag, bool bWaitForAnimation = false) = 0;

    /*
     * Creates new widget and adds it to a slot
     * @param SlotTag - Tag of a WidgetSlot to add new Widget to
     * @param WidgetClass - class of a widget to create
     * @param Content - optional Content to pass to a newly created Widget
     * @param bWaitForAnimation - whether returned task will wait until Appear animation of the widget is finished
     */
    UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "WidgetClass"))
    virtual FVoidCoroutine K2_DisplayWidget(UPARAM(meta = (Categories = "WidgetSlot")) FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content, bool bWaitForAnimation, UDLUserWidget*& Result)
    {
        Result = co_await DisplayWidget(SlotTag, WidgetClass, MoveTemp(Content), bWaitForAnimation);
    }

    /*
     * Closes widget in a WidgetSlot with given Tag
     * @param SlotTag - Tag of a WidgetSlot to add new Widget to
     * @param bWaitForAnimation - whether returned task will wait until Disappear animation of the widget is finished
     */
    UFUNCTION(BlueprintCallable)
    virtual FVoidCoroutine K2_CloseWidget(UPARAM(meta = (Categories = "WidgetSlot")) FGameplayTag SlotTag, bool bWaitForAnimation)
    {
        co_await CloseWidget(SlotTag, bWaitForAnimation);
    }
};
