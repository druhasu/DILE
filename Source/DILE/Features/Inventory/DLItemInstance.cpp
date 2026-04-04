// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemInstance.h"

#include "Net/UnrealNetwork.h"

void UDLItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ThisClass, ItemType, COND_InitialOnly);
}
