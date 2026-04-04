// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLSubObjectReplicationService.generated.h"

UINTERFACE(MinimalApi)
class UDLSubObjectReplicationService : public UInterface { GENERATED_BODY() };

/*
 * Allows replication of SubObjects
 */
class DILE_API IDLSubObjectReplicationService
{
    GENERATED_BODY()

public:
    /* Returns Outer to use when creating new SubObjects */
    virtual UObject* GetOuterForSubObjects() const = 0;

    /* Adds SubObject for replication */
    virtual void AddSubObject(UObject* SubObject, ELifetimeCondition NetCondition = COND_None) = 0;

    /* Removes SubObject from replication */
    virtual void RemoveSubObject(UObject* SubObject) = 0;
};
