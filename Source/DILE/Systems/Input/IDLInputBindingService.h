// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLInputBindingService.generated.h"

class UInputAction;
struct FInputActionInstance;
enum class ETriggerEvent : uint8;

UINTERFACE(MinimalApi)
class UDLInputBindingService : public UInterface
{
    GENERATED_BODY()
};

/*
 * Allows to bind to Enhanced Input event actions
 */
class DILE_API IDLInputBindingService
{
    GENERATED_BODY()

public:
    using FActionEventSignature = TDelegate<void(const FInputActionInstance&)>;

    virtual uint32 BindInput(const UInputAction* Action, ETriggerEvent TriggerEvent, FActionEventSignature&& Delegate) = 0;
    virtual void UnbindInput(uint32 Handle) = 0;
    virtual void UnbindInput(UObject* Object) = 0;
};
