// Copyright Andrei Sudarikov. All Rights Reserved.

#include "ServiceReplicatorComponent.h"
#include "ReplicatedService.h"

UServiceReplicatorComponent::UServiceReplicatorComponent()
{
    SetIsReplicatedByDefault(true);

    bReplicateUsingRegisteredSubObjectList = true;
}

void UServiceReplicatorComponent::InitDependencies(TOptional<TObjectsCollection<IReplicatedService>> InReplicatedServices)
{
    if (InReplicatedServices.IsSet())
    {
        for (const TScriptInterface<IReplicatedService>&& Service : InReplicatedServices.GetValue())
        {
            AddReplicatedSubObject(Service.GetObject());
            Services.Add(Service.GetObject());
        }
    }
}

void UServiceReplicatorComponent::EndPlay(EEndPlayReason::Type InReason)
{
    Super::EndPlay(InReason);

    for (TObjectPtr<UObject> Obj : Services)
    {
        RemoveReplicatedSubObject(Obj);
    }
}
