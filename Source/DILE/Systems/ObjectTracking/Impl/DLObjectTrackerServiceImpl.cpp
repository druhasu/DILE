// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLObjectTrackerServiceImpl.h"
#include "Systems/ObjectTracking/DLTrackedObject.h"
#include "Utils/DLEnsure.h"

void UDLObjectTrackerServiceImpl::InitDependencies()
{
    FDLTrackedObject::RegistrationChanged.AddUObject(this, &ThisClass::OnRegistrationChanged);
}

void UDLObjectTrackerServiceImpl::ForEachObject(UClass* Class, TFunctionRef<bool(UObject* Object)> Action) const
{
    const TArray<TWeakObjectPtr<UObject>>* Array = FDLTrackedObject::Objects.Find(Class);

    if (Array == nullptr)
    {
        return;
    }

    UWorld* World = GetWorld();

    for (TWeakObjectPtr<UObject> WeakObject : *Array)
    {
        UObject* Object = WeakObject.Get();
        if (Object != nullptr && Object->GetWorld() == World)
        {
            if (!Action(Object))
            {
                break;
            }
        }
    }
}

void UDLObjectTrackerServiceImpl::RemoveRegister(UClass* InClass, UObject* Listener)
{
    if (FWItem* Item = Delegates.Find(InClass))
    {
        Item->Registered.RemoveAll(Listener);
    }
}

void UDLObjectTrackerServiceImpl::RemoveUnregister(UClass* InClass, UObject* Listener)
{
    if (FWItem* Item = Delegates.Find(InClass))
    {
        Item->Unregistered.RemoveAll(Listener);
    }
}

void UDLObjectTrackerServiceImpl::RemoveRegister(UClass* InClass, FDelegateHandle Handle)
{
    if (FWItem* Item = Delegates.Find(InClass))
    {
        Item->Registered.Remove(Handle);
    }
}

void UDLObjectTrackerServiceImpl::RemoveUnregister(UClass* InClass, FDelegateHandle Handle)
{
    if (FWItem* Item = Delegates.Find(InClass))
    {
        Item->Unregistered.Remove(Handle);
    }
}

FDelegateHandle UDLObjectTrackerServiceImpl::OnRegisterImpl(UClass* InClass, FChangeDelegate&& Delegate)
{
    DL_ENSURE_RETURN(Delegate.IsBound(), {});
    const TArray<TWeakObjectPtr<UObject>>* Array = FDLTrackedObject::Objects.Find(InClass);
    if (Array != nullptr)
    {
        UWorld* World = GetWorld();
        for (TWeakObjectPtr<UObject> WeakObject : *Array)
        {
            UObject* Object = WeakObject.Get();
            if (Object != nullptr && Object->GetWorld() == World)
            {
                Delegate.Execute(Object);
            }
        }
    }

    return Delegates.FindOrAdd(InClass).Registered.Add(MoveTemp(Delegate));
}

FDelegateHandle UDLObjectTrackerServiceImpl::OnUnregisterImpl(UClass* InClass, FChangeDelegate&& Delegate)
{
    return Delegates.FindOrAdd(InClass).Unregistered.Add(MoveTemp(Delegate));
}

void UDLObjectTrackerServiceImpl::OnRegistrationChanged(UObject* Object, UClass* Class, bool bRegistered)
{
    DL_ENSURE_RETURN(Object != nullptr);
    DL_ENSURE_RETURN(Class != nullptr);

    if (Object->GetWorld() == GetWorld())
    {
        const FWItem* Item = Delegates.Find(Class);
        if (Item != nullptr)
        {
            const FChangedSignature& Delegate = bRegistered ? Item->Registered : Item->Unregistered;
            Delegate.Broadcast(Object);
        }
    }
}
