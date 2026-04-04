// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLTask.h"

#include "IDLAnimatedWidget.generated.h"

class UWidget;

UINTERFACE(MinimalApi)
class UDLAnimatedWidget : public UInterface { GENERATED_BODY() };

/*
 * Interface to a widget that can animate its Appear/Disappear
 */
class DILE_API IDLAnimatedWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    FVoidCoroutine Appear();

    UFUNCTION(BlueprintNativeEvent)
    FVoidCoroutine Disappear();

    static FVoidCoroutine TryShow(UWidget* Widget);
    static FVoidCoroutine TryHide(UWidget* Widget);
};

