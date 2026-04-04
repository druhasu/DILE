// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Interaction/IDLInteractionService.h"
#include "Utils/DLTask.h"

#include "DLInteractionServiceImpl.generated.h"

class IDLPlayerPawnService;
class IDLTimerService;
class IResolver;
class APawn;
class UDLInteractionAction;

UCLASS(Blueprintable)
class DILE_API UDLInteractionServiceImpl : public UObject, public IDLInteractionService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLPlayerPawnService> InPawnService,
        TScriptInterface<IDLTimerService> InTimerService,
        TScriptInterface<IResolver> InResolver
    );

    // Begin IDLInteractionService
    TScriptInterface<IDLInteractable> GetInteractable() const override { return CurrentInteractable; }
    const UDLInteractionDefinitionAsset* GetInteraction() const override { return CurrentInteraction; }
    FChangedSignature& OnInteractionChanged() override { return InteractionChangedDelegate; }
    FDLTask Interact() override;
    // End IDLInteractionService

protected:
    UPROPERTY(EditDefaultsOnly, meta = (Units = "s"))
    float TickInterval = 1 / 30.f;

    UPROPERTY(EditDefaultsOnly, meta = (Units = "cm"))
    double TraceDistance = 200;

    UPROPERTY(EditDefaultsOnly)
    TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;

private:
    void OnPawnChanged(APawn* NewPawn);
    void OnTick();

    FDLTask TraceInteractions(FVector Start, FVector End, FForceLatentCoroutine = {});
    void SetInteractable(TScriptInterface<IDLInteractable> NewInteractable);
    void SetInteraction(TObjectPtr<UDLInteractionDefinitionAsset> NewInteraction);
    void SelectInteraction();

    UDLInteractionAction* GetAction(UClass* ActionClass);

private:
    UPROPERTY()
    TScriptInterface<IDLPlayerPawnService> PawnService;

    UPROPERTY()
    TScriptInterface<IDLTimerService> TimerService;

    UPROPERTY()
    TScriptInterface<IResolver> Resolver;

    UPROPERTY()
    TScriptInterface<IDLInteractable> CurrentInteractable;

    UPROPERTY()
    TObjectPtr<UDLInteractionDefinitionAsset> CurrentInteraction;

    UPROPERTY()
    TMap<TObjectPtr<UClass>, TObjectPtr<UDLInteractionAction>> ActionInstances;

    FChangedSignature InteractionChangedDelegate;
    FDLTask TraceTask = FDLTask::CompletedCoroutine;
    FTimerHandle TickHandle;
};
