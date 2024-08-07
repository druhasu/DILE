// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ReplicatedService.generated.h"

struct FFrame;
struct FOutParmRec;

UINTERFACE(MinimalApi)
class UReplicatedService : public UInterface { GENERATED_BODY() };

/*
 * Interface to a service that needs replication capabilites.
 * You need to do following to enable them:
 *   1. Implement this interface in your service
 *   2. Add REPLICATED_SERVICE_BOILERPLATE() to your service class right after GENERATED_BODY()
 *   3. When registering in Container don't forget to add 'As<IReplicatedService>()'
 *
 * Alternatively you can just inherit from UReplicatedServiceBase and skip steps 1 and 2
 *
 * Replicated services fully support property replication and RPCs
 */
class IReplicatedService
{
    GENERATED_BODY()
};

/* Implementation details of REPLICATED_SERVICE_BOILERPLATE macro */
namespace ReplicatedService_Private
{
    int32 GetFunctionCallspace(UObject* Service, UFunction* Function, FFrame* Stack);
    bool CallRemoteFunction(UObject* Service, UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack);
}

/*
 * Add this macro for manual implementations of IReplicatedService interface
 */
#define REPLICATED_SERVICE_BOILERPLATE() \
    public: \
        static_assert(TIsDerivedFrom<ThisClass, IReplicatedService>::Value, "ThisClass must implement IReplicatedService"); \
        bool IsSupportedForNetworking() const override { return true; } \
        int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override \
        { return ReplicatedService_Private::GetFunctionCallspace(this, Function, Stack); } \
        bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override \
        { return ReplicatedService_Private::CallRemoteFunction(this, Function, Parameters, OutParms, Stack); } \
    private:

/* Base class for replicated services. See IReplicatedService for more details */
UCLASS()
class UReplicatedServiceBase : public UObject, public IReplicatedService
{
    GENERATED_BODY()
    REPLICATED_SERVICE_BOILERPLATE()
};
