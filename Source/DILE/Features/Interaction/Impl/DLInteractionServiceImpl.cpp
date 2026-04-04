// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInteractionServiceImpl.h"
#include "Features/Interaction/IDLInteractable.h"
#include "Features/Interaction/DLInteractionAction.h"
#include "Features/Interaction/DLInteractionDefinitionAsset.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"
#include "Systems/Timers/IDLTimerService.h"
#include "Utils/DLEnsure.h"
#include "Utils/DLInterfaceStatics.h"

#include "DI/IResolver.h"

void UDLInteractionServiceImpl::InitDependencies(
    TScriptInterface<IDLPlayerPawnService> InPawnService,
    TScriptInterface<IDLTimerService> InTimerService,
    TScriptInterface<IResolver> InResolver
)
{
    PawnService = InPawnService;
    TimerService = InTimerService;
    Resolver = InResolver;

    PawnService->OnPawnChanged().AddUObject(this, &ThisClass::OnPawnChanged);
    OnPawnChanged(PawnService->GetPawn());
}

FDLTask UDLInteractionServiceImpl::Interact()
{
    if (CurrentInteractable == nullptr || CurrentInteraction == nullptr)
        return FDLTask::CompletedCoroutine;

    UDLInteractionAction* Action = GetAction(CurrentInteraction->ActionClass);
    DL_ENSURE_RETURN(Action != nullptr, FDLTask::CompletedCoroutine);

    return Action->Execute(CurrentInteractable);
}

void UDLInteractionServiceImpl::OnPawnChanged(APawn* NewPawn)
{
    if (NewPawn != nullptr)
    {
        TimerService->SetTimer(TickHandle, this, &ThisClass::OnTick, TickInterval, true);
        OnTick();
    }
    else
    {
        TimerService->ClearTimer(TickHandle);
        SetInteractable(nullptr);
    }
}

void UDLInteractionServiceImpl::OnTick()
{
    APawn* Pawn = PawnService->GetPawn();
    DL_ENSURE_RETURN(Pawn != nullptr);

    if (TraceTask.IsDone())
    {
        const FVector Location = Pawn->GetPawnViewLocation();
        const FRotator Rotation = Pawn->GetViewRotation();

        const FVector Direction = Rotation.RotateVector(FVector::ForwardVector) * TraceDistance;
        const FVector EndPoint = Location + Direction;

        TraceTask = TraceInteractions(Location, EndPoint);
    }
}

FDLTask UDLInteractionServiceImpl::TraceInteractions(FVector Start, FVector End, FForceLatentCoroutine)
{
    const TArray<FHitResult>& HitResults = co_await UE5Coro::Latent::AsyncLineTraceByChannel(this, EAsyncTraceType::Single, Start, End, TraceChannel);

    if (HitResults.Num() == 0)
    {
        SetInteractable(nullptr);
        co_return;
    }

    const FHitResult& Hit = HitResults[0];
    if (!Hit.bBlockingHit)
    {
        SetInteractable(nullptr);
        co_return;
    }

    AActor* Actor = Hit.GetActor();
    DL_ENSURE_CORETURN(Actor != nullptr);

    SetInteractable(GetInterfaceFromObject<IDLInteractable>(Actor));
}

void UDLInteractionServiceImpl::SetInteractable(TScriptInterface<IDLInteractable> NewInteractable)
{
    if (CurrentInteractable != NewInteractable)
    {
        CurrentInteractable = NewInteractable;

        SelectInteraction();
    }
}

void UDLInteractionServiceImpl::SetInteraction(TObjectPtr<UDLInteractionDefinitionAsset> NewInteraction)
{
    CurrentInteraction = NewInteraction;
    InteractionChangedDelegate.Broadcast(CurrentInteraction);
}

void UDLInteractionServiceImpl::SelectInteraction()
{
    if (CurrentInteractable == nullptr)
    {
        SetInteraction(nullptr);
        return;
    }

    TConstArrayView<TObjectPtr<UDLInteractionDefinitionAsset>> Interactions = CurrentInteractable->GetInteractions();
    if (Interactions.Num() == 0)
    {
        SetInteraction(nullptr);
        return;
    }

    // TODO: Add some selection logic
    SetInteraction(Interactions[0]);
}

UDLInteractionAction* UDLInteractionServiceImpl::GetAction(UClass* ActionClass)
{
    DL_ENSURE_RETURN(ActionClass != nullptr, nullptr);

    TObjectPtr<UDLInteractionAction> Action = ActionInstances.FindRef(ActionClass);
    if (Action == nullptr)
    {
        Action = Cast<UDLInteractionAction>(Resolver->Resolve(ActionClass));
        ActionInstances.Add(ActionClass, Action);
    }

    return Action;
}
