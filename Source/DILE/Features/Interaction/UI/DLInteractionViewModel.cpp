// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInteractionViewModel.h"
#include "Features/Interaction/IDLInteractionService.h"
#include "Features/Interaction/IDLInteractable.h"
#include "Features/Interaction/DLInteractionDefinitionAsset.h"

void UDLInteractionViewModel::InitDependencies(
    TScriptInterface<IDLInteractionService> InInteractionService
)
{
    InteractionService = InInteractionService;

    InteractionService->OnInteractionChanged().AddUObject(this, &ThisClass::OnInteractionChanged);
}

bool UDLInteractionViewModel::GetIsAvailable() const
{
    return InteractionService->GetInteraction() != nullptr;
}

void UDLInteractionViewModel::Interact()
{
    InteractionService->Interact();
}

void UDLInteractionViewModel::OnInteractionChanged(const UDLInteractionDefinitionAsset* NewInteraction)
{
    if (NewInteraction != nullptr)
    {
        FText TargetName = InteractionService->GetInteractable()->GetInteractableName();

        FFormatNamedArguments Args;
        Args.Add(TEXT("Target"), TargetName);

        SetInteractionTitle(FText::Format(NewInteraction->DisplayNameFormat, MoveTemp(Args)));
        RaiseChanged(IsAvailableProperty());
    }
    else
    {
        SetInteractionTitle(FText::GetEmpty());
        RaiseChanged(IsAvailableProperty());
    }
}
