// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "DLItemFragment.h"
#include "StructUtils/InstancedStruct.h"

#include "DLItemType.generated.h"

class UDLItemInstance;

/*
 * Type of an item
 * Contains 'static' data shared between all item instances of this item type
 */
UCLASS(Abstract)
class UDLItemType : public UPrimaryDataAsset
{
    GENERATED_BODY()
    friend class UDLItemStatics;

public:
    /* Returns fragment of given type, or nullptr if it is not found */
    template <typename T UE_REQUIRES(TIsDerivedFrom<T, FDLItemFragment>::Value)>
    const T* FindFragment() const { return (const T*)FindFragment(T::StaticStruct()); }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Common Common;

    /* Extra item fragments to add */
    UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (ShowOnlyInnerProperties))
    TArray<TInstancedStruct<FDLItemFragment>> ExtraFragments;

protected:
    const FDLItemFragment* FindFragment(const UScriptStruct* StructType) const;
};
