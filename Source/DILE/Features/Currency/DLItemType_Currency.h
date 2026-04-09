// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemType.h"
#include "DLItemFragment_Currency.h"

#include "DLItemType_Currency.generated.h"

/*
 * Currency Item type
 */
UCLASS()
class UDLItemType_Currency : public UDLItemType
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Currency", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Currency Currency;

    UPROPERTY(EditDefaultsOnly, Category = "Currency", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Countable Countable;
};
