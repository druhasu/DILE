// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLTask.h"

#include "IDLInteractionService.generated.h"

class IDLInteractable;
class UDLInteractionDefinitionAsset;

UINTERFACE(MinimalApi)
class UDLInteractionService : public UInterface { GENERATED_BODY() };

/*
 * Allows interaction with objects in the World
 */
class DILE_API IDLInteractionService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(const UDLInteractionDefinitionAsset* NewInteraction)>;

    /* Current Interactable or nullptr if not available */
    virtual TScriptInterface<IDLInteractable> GetInteractable() const = 0;

    /* Current available interaction or nullptr if no interactions are available */
    virtual const UDLInteractionDefinitionAsset* GetInteraction() const = 0;

    /* Called when current interaction changes */
    virtual FChangedSignature& OnInteractionChanged() = 0;

    /* Interacts with current Interactable object. Returns task that is completed with the interaction. If interaction is not possible, returns completed task */
    virtual FDLTask Interact() = 0;
};
