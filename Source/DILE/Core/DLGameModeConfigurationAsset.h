// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "Core/DependencyManagement/DLContainerConfiguratorContext.h"

#include "DLGameModeConfigurationAsset.generated.h"

class FObjectContainerBuilder;
class UDLFeatureConfigurationAsset;

UCLASS()
class DILE_API UDLGameModeConfigurationAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    void RegisterClassesForActor(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context) const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

public:
    UPROPERTY(EditAnywhere)
    TArray<TObjectPtr<UDLFeatureConfigurationAsset>> Features;
};
