// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/NetMode/IDLNetModeService.h"

#include "DLNetModeServiceImpl.generated.h"

UCLASS()
class DILE_API UDLNetModeServiceImpl : public UObject, public IDLNetModeService
{
    GENERATED_BODY()

public:
    // Begin IDLNetModeService
    virtual ENetMode GetNetMode() const override;
    // End IDLNetModeService
};
