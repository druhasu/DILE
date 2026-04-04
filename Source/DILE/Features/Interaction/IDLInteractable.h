// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLInteractable.generated.h"

class UDLInteractionDefinitionAsset;

UINTERFACE(MinimalApi)
class UDLInteractable : public UInterface { GENERATED_BODY() };

/*
 * Object that can be interacted with by a Player
 */
class DILE_API IDLInteractable
{
    GENERATED_BODY()

public:
    /* User friendly name of this Interactable */
    virtual FText GetInteractableName() const = 0;

    /* List of all interactions that can be performed with this Interactable */
    virtual TConstArrayView<TObjectPtr<UDLInteractionDefinitionAsset>> GetInteractions() const = 0;
};
