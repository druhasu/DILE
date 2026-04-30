// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/WeaponView/IDLWeaponSocketsService.h"

#include "DLWeaponSocketsServiceImpl.generated.h"

class IDLPlayerPawnService;

UCLASS()
class DILE_API UDLWeaponSocketsServiceImpl : public UObject, public IDLWeaponSocketsService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLPlayerPawnService> InPawnService
    );

    // Begin IDLWeaponSocketsService
    FDLSocketInfo GetMuzzleSocket() const override;
    // End IDLWeaponSocketsService

private:
    UPROPERTY()
    TScriptInterface<IDLPlayerPawnService> PawnService;
};
