// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructUtils/InstancedStruct.h"

#include "DLItemStatics.generated.h"

class UDLItemType;
class UDLItemInstance;
struct FDLItemFragment;

UCLASS()
class DILE_API UDLItemStatics : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /* Finds fragment of given type inside ItemType */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Find Fragment (ItemType)"))
    static TInstancedStruct<FDLItemFragment> FindFragmentInItemType(const UScriptStruct* FragmentType, const UDLItemType* ItemType);

    /* Finds fragment of given type inside ItemInstance */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Find Fragment (ItemInstance)"))
    static TInstancedStruct<FDLItemFragment> FindFragmentInItemInstance(const UScriptStruct* FragmentType, const UDLItemInstance* ItemInstance);
};
