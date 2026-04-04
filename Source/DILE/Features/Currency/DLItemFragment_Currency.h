// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Currency.generated.h"

USTRUCT()
struct FDLItemFragment_Currency : public FDLItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FText Description;
};
