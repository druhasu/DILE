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

    /*
     * Binds InputAction to a delegate
     * @param Action - InputAction to bind to
     * @param TriggerEvent - which trigger event to bind to
     * @param Delegate - which delegate to call when Action is triggered
     * @result binding Handle that can later be used to Unbind
     */
    virtual uint32 BindInput(const UInputAction* Action, ETriggerEvent TriggerEvent, FActionEventSignature&& Delegate) = 0;

    /* Unbind input action by Handle */
    virtual void UnbindInput(uint32 Handle) = 0;

    /* Unbind all delegates of given Object */
    virtual void UnbindInput(UObject* Object) = 0;
};
