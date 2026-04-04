// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLWorldObject.generated.h"

/*
 * Base class for UObjects that implement GetWorld for use in Blueprints
 */
UCLASS(Blueprintable)
class DILE_API UDLWorldObject : public UObject
{
    GENERATED_BODY()

public:
    UWorld* GetWorld() const override
    {
        if (!HasAnyFlags(RF_ClassDefaultObject))
        {
            if (UObject* Outer = GetOuter())
            {
                return Outer->GetWorld();
            }
        }

        return nullptr;
    }
};
