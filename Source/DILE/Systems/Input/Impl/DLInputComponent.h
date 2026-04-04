// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/Input/IDLInputBindingService.h"
#include "EnhancedInputComponent.h"

#include "DLInputComponent.generated.h"

UCLASS()
class DILE_API UDLInputComponent : public UEnhancedInputComponent, public IDLInputBindingService
{
    GENERATED_BODY()

public:
    uint32 BindInput(const UInputAction* Action, ETriggerEvent TriggerEvent, FActionEventSignature&& Delegate) override;
    void UnbindInput(uint32 Handle) override;
    void UnbindInput(UObject* Object) override;
};
