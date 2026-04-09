// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "DLInteractionDefinitionAsset.generated.h"

class UDLInteractionAction;

/*
 * Definition of an interaction
 */
UCLASS()
class DILE_API UDLInteractionDefinitionAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    /* Format for the name of this Interaction to show in the UI */
    UPROPERTY(EditDefaultsOnly)
    FText DisplayNameFormat;

    /* Class of InteractionAction that handles actual interaction logic */
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UDLInteractionAction> ActionClass;
};
