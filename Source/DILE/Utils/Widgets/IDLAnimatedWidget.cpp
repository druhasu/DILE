// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Utils/Widgets/IDLAnimatedWidget.h"

#include "Components/Widget.h"

FVoidCoroutine IDLAnimatedWidget::TryShow(UWidget* Widget)
{
    if (Widget->Implements<UDLAnimatedWidget>())
    {
        return IDLAnimatedWidget::Execute_Appear(Widget);
    }

    return FDLTask::CompletedCoroutine;
}

FVoidCoroutine IDLAnimatedWidget::TryHide(UWidget* Widget)
{
    if (Widget->Implements<UDLAnimatedWidget>())
    {
        return IDLAnimatedWidget::Execute_Disappear(Widget);
    }

    return FDLTask::CompletedCoroutine;
}
