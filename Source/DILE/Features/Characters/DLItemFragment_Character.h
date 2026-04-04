// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLItemFragment_Character.generated.h"

class ADLPawn;
class UTexture2D;

USTRUCT()
struct FDLItemFragment_Character
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, meta = (AssetBundles = "Client"))
    TSoftClassPtr<ADLPawn> PawnClass;

    UPROPERTY(EditAnywhere, meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> Portrait;
};
