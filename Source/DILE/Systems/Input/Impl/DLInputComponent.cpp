// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInputComponent.h"

struct FDLActionEventBinding : FEnhancedInputActionEventBinding
{
    FDLActionEventBinding(const UInputAction* Action, ETriggerEvent InTriggerEvent, UDLInputComponent::FActionEventSignature&& InDelegate)
        : FEnhancedInputActionEventBinding(Action, InTriggerEvent)
        , Delegate(MoveTemp(InDelegate))
    {
    }

    FDLActionEventBinding(const FDLActionEventBinding& CloneFrom, EInputBindingClone Clone)
        : FEnhancedInputActionEventBinding(CloneFrom, Clone)
        , Delegate(CloneFrom.Delegate)
    {
    }

    void Execute(const FInputActionInstance& ActionData) const override
    {
        Delegate.Execute(ActionData);
    }

    TUniquePtr<FEnhancedInputActionEventBinding> Clone() const override
    {
        return TUniquePtr<FEnhancedInputActionEventBinding>(new FDLActionEventBinding(*this, EInputBindingClone::ForceClone));
    }

    void SetShouldFireWithEditorScriptGuard(const bool bNewValue) override {}

    bool IsBoundToObject(FDelegateUserObjectConst Object) const override
    {
        return Delegate.IsBoundToObject(Object);
    }

    UObject* GetUObject() const override
    {
        return Delegate.GetUObject();
    }

    UDLInputComponent::FActionEventSignature Delegate;
};

uint32 UDLInputComponent::BindInput(const UInputAction* Action, ETriggerEvent TriggerEvent, FActionEventSignature&& Delegate)
{
    TUniquePtr<FDLActionEventBinding> AB = MakeUnique<FDLActionEventBinding>(Action, TriggerEvent, MoveTemp(Delegate));
    AB->SetShouldFireWithEditorScriptGuard(bShouldFireDelegatesInEditor);
    uint32 Result = AB->GetHandle();
    EnhancedActionEventBindings.Add(MoveTemp(AB));
    return Result;
}

void UDLInputComponent::UnbindInput(uint32 Handle)
{
    RemoveBindingByHandle(Handle);
}

void UDLInputComponent::UnbindInput(UObject* Object)
{
    ClearBindingsForObject(Object);
}
