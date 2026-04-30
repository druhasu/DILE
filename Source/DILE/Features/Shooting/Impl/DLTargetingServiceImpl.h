// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Shooting/IDLTargetingService.h"

#include "Abilities/GameplayAbilityTargetTypes.h"

#include "DLTargetingServiceImpl.generated.h"

class IDLAimingService;
class IDLWeaponSocketsService;
class IDLPlayerPawnService;

UCLASS(Blueprintable)
class DILE_API UDLTargetingServiceImpl : public UObject, public IDLTargetingService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLAimingService> InAimingService,
        TScriptInterface<IDLWeaponSocketsService> InWeaponSocketsService,
        TScriptInterface<IDLPlayerPawnService> InPawnService
    );

    // Begin IDLTargetingService
    FGameplayAbilityTargetDataHandle MakeHitscanTargetData() const override;
    FGameplayAbilityTargetDataHandle MakeProjectileTargetData() const override;
    // End IDLTargetingService

protected:
    UPROPERTY(EditDefaultsOnly, meta = (Units = "cm"))
    double TraceDistance = 2000;

    UPROPERTY(EditDefaultsOnly)
    TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;

private:
    UPROPERTY()
    TScriptInterface<IDLAimingService> AimingService;

    UPROPERTY()
    TScriptInterface<IDLWeaponSocketsService> WeaponSocketsService;

    UPROPERTY()
    TScriptInterface<IDLPlayerPawnService> PawnService;
};

USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_Projectile : public FGameplayAbilityTargetData
{
    GENERATED_BODY()

public:
    /* Starting location for Projectile */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Targeting)
    FGameplayAbilityTargetingLocationInfo SourceLocation;

    bool HasOrigin() const override { return true; }
    FTransform GetOrigin() const override { return SourceLocation.GetTargetingTransform(); }

    UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
    FString ToString() const override { return TEXT("FGameplayAbilityTargetData_Projectile"); }

    bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_Projectile> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_Projectile>
{
    enum { WithNetSerializer = true };
};
