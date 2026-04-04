// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ObjectTracking/IDLObjectTrackerService.h"

#include "DLObjectTrackerServiceImpl.generated.h"

UCLASS()
class DILE_API UDLObjectTrackerServiceImpl : public UObject, public IDLObjectTrackerService
{
    GENERATED_BODY()

public:
    void InitDependencies();

    // Begin IDLObjectTrackerService
    void ForEachObject(UClass* Class, TFunctionRef<bool(UObject* Object)> Action) const override;
    void RemoveRegister(UClass* InClass, UObject* Listener) override;
    void RemoveUnregister(UClass* InClass, UObject* Listener) override;
    void RemoveRegister(UClass* InClass, FDelegateHandle Handle) override;
    void RemoveUnregister(UClass* InClass, FDelegateHandle Handle) override;

protected:
    FDelegateHandle OnRegisterImpl(UClass* InClass, FChangeDelegate&& Delegate) override;
    FDelegateHandle OnUnregisterImpl(UClass* InClass, FChangeDelegate&& Delegate) override;
    // End IDLObjectTrackerService

private:
    using FChangedSignature = TMulticastDelegate<void(UObject* Object)>;
    struct FWItem
    {
        FChangedSignature Registered;
        FChangedSignature Unregistered;
    };

private:
    void OnRegistrationChanged(UObject* Object, UClass* Class, bool bRegistered);

private:
    TMap<UClass*, FWItem> Delegates;
};
