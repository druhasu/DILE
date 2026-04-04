// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemStatics.h"
#include "DLItemFragment.h"
#include "DLItemInstance.h"
#include "DLItemType.h"
#include "Utils/DLEnsure.h"

#include "StructUtils/StructView.h"

TInstancedStruct<FDLItemFragment> UDLItemStatics::FindFragmentInItemType(const UScriptStruct* FragmentType, const UDLItemType* ItemType)
{
    DL_ENSURE_RETURN(ItemType != nullptr, {});

    TInstancedStruct<FDLItemFragment> Result;

    const FDLItemFragment* Fragment = ItemType->FindFragment(FragmentType);
    if (Fragment != nullptr)
    {
        Result.InitializeAsScriptStruct(FragmentType, (const uint8*)Fragment);
    }

    return Result;
}

TInstancedStruct<FDLItemFragment> UDLItemStatics::FindFragmentInItemInstance(const UScriptStruct* FragmentType, const UDLItemInstance* ItemInstance)
{
    DL_ENSURE_RETURN(ItemInstance != nullptr, {});
    return FindFragmentInItemType(FragmentType, ItemInstance->GetItemType());
}
