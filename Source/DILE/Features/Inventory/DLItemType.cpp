// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemType.h"

const FDLItemFragment* UDLItemType::FindFragment(const UScriptStruct* StructType) const
{
    for (TFieldIterator<FStructProperty> It(GetClass(), EFieldIterationFlags::IncludeSuper); It; ++It)
    {
        if (It->Struct->IsChildOf(StructType))
        {
            return It->ContainerPtrToValuePtr<FDLItemFragment>(this);
        }
    }

    for (const TInstancedStruct<FDLItemFragment>& FragmentStruct : ExtraFragments)
    {
        if (FragmentStruct.IsValid() && FragmentStruct.GetScriptStruct()->IsChildOf(StructType))
        {
            return FragmentStruct.GetPtr();
        }
    }

    return nullptr;
}
