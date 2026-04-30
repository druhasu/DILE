// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Components/SceneComponent.h"

#include "IDLWeaponSocketsService.generated.h"

USTRUCT(BlueprintType)
struct FDLSocketInfo
{
    GENERATED_BODY()

public:
    FTransform GetTransform(ERelativeTransformSpace TransformSpace = ERelativeTransformSpace::RTS_World) const
    {
        if (Component != nullptr)
            return Component->GetSocketTransform(SocketName, TransformSpace);

        return {};
    }

public:
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<USceneComponent> Component;

    UPROPERTY(BlueprintReadWrite)
    FName SocketName;
};

UINTERFACE(MinimalApi, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDLWeaponSocketsService : public UInterface { GENERATED_BODY() };

/*
 * Provides locations of various sockets of a weapon mesh
 */
class DILE_API IDLWeaponSocketsService
{
    GENERATED_BODY()

public:
    /* World Space transform of a weapon muzzle */
    UFUNCTION(BlueprintCallable)
    virtual FDLSocketInfo GetMuzzleSocket() const = 0;
};

