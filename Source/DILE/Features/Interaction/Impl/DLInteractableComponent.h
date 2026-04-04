// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Features/Interaction/IDLInteractable.h"

#include "DLInteractableComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class DILE_API UDLInteractableComponent : public UActorComponent, public IDLInteractable
{
    GENERATED_BODY()

public:
    // Begin IDLInteractable
    FText GetInteractableName() const override { return InteractableName; }
    TConstArrayView<TObjectPtr<UDLInteractionDefinitionAsset>> GetInteractions() const override { return MakeArrayView(Interactions); }
    // End IDLInteractable

protected:
    UPROPERTY(EditAnywhere)
    FText InteractableName;

    UPROPERTY(EditAnywhere)
    TArray<TObjectPtr<UDLInteractionDefinitionAsset>> Interactions;
};
