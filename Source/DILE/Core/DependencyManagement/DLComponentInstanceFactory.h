// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DI/IInstanceFactory.h"

#include "DLComponentInstanceFactory.generated.h"

UCLASS()
class UDLComponentInstanceFactory : public UObject, public IInstanceFactory
{
    GENERATED_BODY()

public:
    // Begin IInstanceFactory
    bool IsClassSupported(UClass* EffectiveClass) const override;
    UObject* Create(UObject* Outer, UClass* EffectiveClass) const override;
    void FinalizeCreation(UObject* Object) const override;
    // End IInstanceFactory
};
