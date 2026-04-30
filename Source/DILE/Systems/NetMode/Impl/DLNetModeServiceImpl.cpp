// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLNetModeServiceImpl.h"

ENetMode UDLNetModeServiceImpl::GetNetMode() const
{
    // get NetMode from World
    return GetWorld()->GetNetMode();
}
