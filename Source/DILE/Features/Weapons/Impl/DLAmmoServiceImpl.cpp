// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLAmmoServiceImpl.h"
#include "DruInterfaceEvents/EventRegistry.h"

void UDLAmmoServiceImpl::InitDependencies()
{
    AmmoCountChangedDynamicDelegate.Broadcast(nullptr, 0, 1);
}
