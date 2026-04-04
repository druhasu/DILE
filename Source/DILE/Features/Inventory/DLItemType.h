// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "DLItemFragment.h"
#include "StructUtils/InstancedStruct.h"

#include "DLItemType.generated.h"

class UDLItemInstance;

UCLASS(Abstract)
class UDLItemType : public UPrimaryDataAsset
{
    GENERATED_BODY()
    friend class UDLItemStatics;

public:
    template <typename T UE_REQUIRES(TIsDerivedFrom<T, FDLItemFragment>::Value)>
    const T* FindFragment() const { return (const T*)FindFragment(T::StaticStruct()); }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Common Common;

    UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (ShowOnlyInnerProperties))
    TArray<TInstancedStruct<FDLItemFragment>> ExtraFragments;

protected:
    const FDLItemFragment* FindFragment(const UScriptStruct* StructType) const;
};
