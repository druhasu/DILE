// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLItemFragment_Character.generated.h"

class ADLPawn;
class UTexture2D;

/*
 * Data of Character item
 */
USTRUCT()
struct FDLItemFragment_Character
{
    GENERATED_BODY()

public:
    /* Class of Pawn to use when playing as this Character */
    UPROPERTY(EditAnywhere, meta = (AssetBundles = "Client"))
    TSoftClassPtr<ADLPawn> PawnClass;

    /* Portrait of a Character to show in the UI */
    UPROPERTY(EditAnywhere, meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> Portrait;
};
