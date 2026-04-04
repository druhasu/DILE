// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemType.h"
#include "DLItemFragment_Character.h"

#include "DLItemType_Character.generated.h"

UCLASS()
class DILE_API UDLItemType_Character : public UDLItemType
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Character", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Character Character;
};
