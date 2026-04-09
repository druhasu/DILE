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
    /* Plays Appear animation. Returns coroutine to track when animation finishes */
    UFUNCTION(BlueprintNativeEvent)
    FVoidCoroutine Appear();

    /* Plays Disappear animation. Returns coroutine to track when animation finishes */
    UFUNCTION(BlueprintNativeEvent)
    FVoidCoroutine Disappear();

    /* Tries to show the Widget playing its Appear animation if possible */
    static FVoidCoroutine TryShow(UWidget* Widget);

    /* Tries to hide the Widget playing its Disappear animation if possible */
    static FVoidCoroutine TryHide(UWidget* Widget);
};

