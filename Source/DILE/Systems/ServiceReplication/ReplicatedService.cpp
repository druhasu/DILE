// Copyright Andrei Sudarikov. All Rights Reserved.

#include "ReplicatedService.h"
#include "Engine/NetDriver.h"

int32 ReplicatedService_Private::GetFunctionCallspace(UObject* Service, UFunction* Function, FFrame* Stack)
{
    AActor* MyOwner = Service->GetTypedOuter<AActor>();
    return (MyOwner ? MyOwner->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local);
}

bool ReplicatedService_Private::CallRemoteFunction(UObject* Service, UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
    bool bProcessed = false;

    if (AActor* MyOwner = Service->GetTypedOuter<AActor>())
    {
        FWorldContext* const Context = GEngine->GetWorldContextFromWorld(Service->GetWorld());
        if (Context != nullptr)
        {
            for (FNamedNetDriver& Driver : Context->ActiveNetDrivers)
            {
                if (Driver.NetDriver != nullptr && Driver.NetDriver->ShouldReplicateFunction(MyOwner, Function))
                {
                    Driver.NetDriver->ProcessRemoteFunction(MyOwner, Function, Parameters, OutParms, Stack, Service);
                    bProcessed = true;
                }
            }
        }
    }

    return bProcessed;
}
