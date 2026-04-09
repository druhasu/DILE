// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Currency.generated.h"

/*
 * Data of Curreny item
 */ 
USTRUCT()
struct FDLItemFragment_Currency : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Description of the Currency to show in the UI */
    UPROPERTY(EditDefaultsOnly)
    FText Description;
};
