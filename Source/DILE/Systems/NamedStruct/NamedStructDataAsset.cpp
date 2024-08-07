// Copyright Andrei Sudarikov. All Rights Reserved.

#include "NamedStructDataAsset.h"

FName UNamedStructDataAsset::GetStructName() const
{
    if (CachedName.IsNone())
    {
        FString Name = GetName();
        FStringView NameView(Name);

        // strip DA_ prefix from the Name
        if (NameView.StartsWith(TEXT("DA_")))
        {
            NameView.RemovePrefix(3);
        }

        CachedName = FName(NameView);
    }

    return CachedName;
}

UScriptStruct* UNamedStructDataAsset::GetDataStructType() const
{
    FStructProperty* Property = GetDataProperty();
    return Property ? Property->Struct : nullptr;
}

const void* UNamedStructDataAsset::GetValuePtr() const
{
    FStructProperty* Property = GetDataProperty();
    return Property ? Property->ContainerPtrToValuePtr<void>(this) : nullptr;
}

#if WITH_EDITORONLY_DATA
void UNamedStructDataAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
    UScriptStruct* DataStruct = GetDataStructType();

    if (DataStruct)
    {
        // save StructType into asset tags so we can read it during scan, without loading the asset
        OutTags.Add(FAssetRegistryTag(DataStructureTag, DataStruct->GetStructPathName().ToString(), FAssetRegistryTag::TT_Alphabetical));
    }

    Super::GetAssetRegistryTags(OutTags);
}
#endif

FStructProperty* UNamedStructDataAsset::GetDataProperty() const
{
    // derived class must contain only one property. and this property must contain actual DataStruct
    return CastField<FStructProperty>(GetClass()->PropertyLink);
}
