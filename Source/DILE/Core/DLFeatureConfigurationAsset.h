// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "Core/DependencyManagement/DLContainerConfiguratorContext.h"

#include "GameplayTagContainer.h"

#include "DLFeatureConfigurationAsset.generated.h"

class FObjectContainerBuilder;

UENUM()
enum class EDLContainerConfigurationClassLifetime : uint8
{
    /* Object is created on demand and kept alive only if referenced by some other object */
    OnDemandWeak,

    /* Object is created on demand and kept alive until Container is destroyed */
    OnDemand,

    /* Object is created initially and kept alive until Container is destroyed */ 
    Initial,
};

/* Configuration of a single class in container */
USTRUCT()
struct FDLContainerConfigurationClassEntry
{
    GENERATED_BODY()

public:
    /* Class of service/component to create */
    UPROPERTY(EditAnywhere)
    TSubclassOf<UObject> ServiceClass;

    /* Lifetime of created object */
    UPROPERTY(EditAnywhere)
    EDLContainerConfigurationClassLifetime Lifetime = EDLContainerConfigurationClassLifetime::OnDemand;

    /* Determines when this class should be registered */
    UPROPERTY(EditAnywhere, meta = (Categories = "ContainerContext"))
    FGameplayTagQuery Condition;

    /* Additionaly register Service Class As these classes */
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<UObject>> RegisterAs;
};

/* Configuration of several classes in a single container */
USTRUCT()
struct FDLContainerConfigurationClassesList
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, meta = (TitleProperty = "ServiceClass", ShowOnlyInnerProperties))
    TArray<FDLContainerConfigurationClassEntry> Entries;
};

/* Configuration of a Container */
UCLASS()
class DILE_API UDLFeatureConfigurationAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    void RegisterClassesForActor(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context) const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

public:
    /* Base asset for this. All its classes will be inherited */
    UPROPERTY(EditAnywhere)
    TArray<TObjectPtr<UDLFeatureConfigurationAsset>> RequiredFeatures;

    /* Classes to create in a Container. UObject classes are supported in every container, also ActorComponents are supported in Pawns */
    UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Engine.Actor"))
    TMap<TObjectPtr<UClass>, FDLContainerConfigurationClassesList> Classes;

private:
    void RegisterEntry(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context, const FDLContainerConfigurationClassEntry& Entry) const;
    bool MatchesCondition(const FDLActorContainerConfiguratorContext& Context, const FDLContainerConfigurationClassEntry& Entry) const;
};
