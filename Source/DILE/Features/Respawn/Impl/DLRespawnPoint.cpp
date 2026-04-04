// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLRespawnPoint.h"

void ADLRespawnPoint::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    RegisterObject<ThisClass>(this);
}

void ADLRespawnPoint::EndPlay(EEndPlayReason::Type InReason)
{
    UnregisterObject<ThisClass>(this);
    Super::EndPlay(InReason);
}
