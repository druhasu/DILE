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
 *
 */
class DILE_API IDLWidgetDisplayService
{
    GENERATED_BODY()

public:
    virtual TDLTask<UDLUserWidget*> DisplayWidget(FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content) = 0;
    virtual FDLTask CloseWidget(FGameplayTag SlotTag) = 0;

    UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "WidgetClass"))
    virtual FVoidCoroutine K2_DisplayWidget(UPARAM(meta = (Categories = "WidgetSlot")) FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content, UDLUserWidget*& Result)
    {
        Result = co_await DisplayWidget(SlotTag, WidgetClass, MoveTemp(Content));
    }

    UFUNCTION(BlueprintCallable)
    virtual FVoidCoroutine K2_CloseWidget(UPARAM(meta = (Categories = "WidgetSlot")) FGameplayTag SlotTag)
    {
        co_await CloseWidget(SlotTag);
    }
};
