// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLComponentInstanceFactory.h"

#include "Components/SceneComponent.h"

bool UDLComponentInstanceFactory::IsClassSupported(UClass* EffectiveClass) const
{
    // only non Scene components are supported here
    // the rest will be handled by default InstanceFactory
    return EffectiveClass->IsChildOf<UActorComponent>() && ensure(!EffectiveClass->IsChildOf<USceneComponent>());
}

UObject* UDLComponentInstanceFactory::Create(UObject* Outer, UClass* EffectiveClass) const
{
    UActorComponent* Result = NewObject<UActorComponent>(Outer, EffectiveClass);

    // set CreationMethod to make component visible in Details panel during PIE
    Result->CreationMethod = EComponentCreationMethod::Instance;

    return Result;
}

void UDLComponentInstanceFactory::FinalizeCreation(UObject* Object) const
{
    // register component after it has received InitDependencies
    CastChecked<UActorComponent>(Object)->RegisterComponent();
}
