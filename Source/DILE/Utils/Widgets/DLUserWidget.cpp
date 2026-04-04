// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLUserWidget.h"

void UDLUserWidget::NativeDestruct()
{
    Super::NativeDestruct();

    StopAllAnimations();
}
