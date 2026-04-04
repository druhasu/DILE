// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "DLReplicatedService.generated.h"

struct FFrame;
struct FOutParmRec;

UINTERFACE(MinimalApi)
class UDLReplicatedService : public UInterface { GENERATED_BODY() };

/*
 * Interface to a service that needs replication capabilites.
 * You need to do following to enable them:
 *   1. Implement this interface in your service
 *   2. Add DL_REPLICATED_SERVICE_BOILERPLATE() to your service class right after GENERATED_BODY()
 *   3. When registering in Container don't forget to add 'As<IDLReplicatedService>()'
 *
 * Alternatively you can just inherit from UDLReplicatedServiceBase and skip steps 1 and 2
 *
 * Replicated services fully support property replication and RPCs
 */
class IDLReplicatedService
{
    GENERATED_BODY()

public:
    virtual ELifetimeCondition GetReplicationCondition() const { return ELifetimeCondition::COND_None; }
};

/* Implementation details of REPLICATED_SERVICE_BOILERPLATE macro */
namespace DLReplicatedService_Private
{
    int32 GetFunctionCallspace(UObject* Service, UFunction* Function, FFrame* Stack);
    bool CallRemoteFunction(UObject* Service, UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack);
}

/*
 * Add this macro for manual implementations of IDLReplicatedService interface
 */
#define DL_REPLICATED_SERVICE_BOILERPLATE() \
    public: \
        static_assert(TIsDerivedFrom<ThisClass, IDLReplicatedService>::Value, "ThisClass must implement IDLReplicatedService"); \
        bool IsSupportedForNetworking() const override { return true; } \
        bool IsNameStableForNetworking() const override { return true;} \
        int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override \
        { return DLReplicatedService_Private::GetFunctionCallspace(this, Function, Stack); } \
        bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override \
        { return DLReplicatedService_Private::CallRemoteFunction(this, Function, Parameters, OutParms, Stack); } \
    protected: \
        bool HasAuthority() const \
        { \
            AActor* Outer = GetTypedOuter<AActor>(); \
            return Outer != nullptr ? Outer->HasAuthority() : true; \
        } \
    private:

/* Base class for replicated services. See IDLReplicatedService for more details */
UCLASS()
class UDLReplicatedServiceBase : public UObject, public IDLReplicatedService
{
    GENERATED_BODY()
    DL_REPLICATED_SERVICE_BOILERPLATE()
};
