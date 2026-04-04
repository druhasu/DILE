// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "DLInteractionDefinitionAsset.generated.h"

class UDLInteractionAction;

UCLASS()
class DILE_API UDLInteractionDefinitionAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FText DisplayNameFormat;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UDLInteractionAction> ActionClass;
};
