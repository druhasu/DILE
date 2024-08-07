// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "NamedStructRegistry.h"

#include "DataStructName.generated.h"

/*

  intention:
     simple way of defining "sidecar" data identified by a name+type
     data is always a ustruct, name is FName
     data is always accessible and loaded
     each datatype can only come from single source (for simplicity)

  struct:
      we need CRTP base for typed getters & registrations & common functionality


  registry:
      getter for list of names for each struct
      getter for list of all structs


  what to do:
      iterate over all assets
        find assets that are compatible
          extract names+values from them
          pin assets for GC
      store map of <structType, map<name, value*>>
      listen for assets add/remove and rescan everything

  how to extend:
      custom provider for each type?
      register via static call to Registry.RegisterProvider(TSharedRef)


  provider:
    bool ExtractNames(in asset, out StructType, out map<name, value*>)

  struct:
    custom serializer instead of base UStruct inside IMPLEMENT macro
    we need IMPLEMENT macro for TStructOpsTypeTraitsBase2::WithGetPreloadDependencies anyways, so can put everything in there











*/

USTRUCT(BlueprintType)
struct FDataStructName
{
    GENERATED_BODY()
};

template <typename TKeyStruct, typename TValueStruct>
struct TDataStructName
{
public:
    using StructType = TKeyStruct;

    TDataStructName()
    {
        const uint8& Register = Registrar;
    }

    TDataStructName(FName InName)
        : Name(InName)
    {
    }

    static TArrayView<const TKeyStruct> GetPossibleNames()
    {
        TArrayView<const FName> Names = FNamedStructRegistry::GetPossibleNames(StaticStruct());
        return MakeArrayView(reinterpret_cast<const TKeyStruct*>(Names.GetData()), Names.Num());
    }

    const TValueStruct* GetValuePtr() const
    {
        return (const TValueStruct*)FNamedStructRegistry::GetValuePtr(StaticStruct(), Name);
    }

    bool IsNone() const { return Name.IsNone(); }

    FString ToString() const { return Name.ToString(); }

    FName operator*() const { return Name; }

    bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
    {
        Name = Buffer;
        return true;
    }

    bool ExportTextItem(FString& ValueStr, TDataStructName const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
    {
        Name.ToString(ValueStr);
        return true;
    }

    bool Serialize(FArchive& Ar)
    {
        Ar << Name;
        return true;
    }

    void GetPreloadDependencies(TArray<UObject*>& OutDeps)
    {
        FNamedStructRegistry::Get().GetPreloadDependencies(StaticStruct(), Name, OutDeps);
    }

private:
    static UScriptStruct* StaticStruct()
    {
        return TKeyStruct::StaticStruct();
    }

    static uint8 Registrar;

    FName Name;
};

template <typename TKeyStruct, typename TValueStruct>
uint8 TDataStructName<TKeyStruct, TValueStruct>::Registrar = FNamedStructRegistry::Get().RegisterNamedStruct<TKeyStruct, TValueStruct>();


#define IMPLEMENT_DATA_STRUCT_NAME(StructName) \
}; \
template<> \
struct TStructOpsTypeTraits<StructName> : public TStructOpsTypeTraitsBase2<StructName> \
{ \
    enum \
    { \
        WithExportTextItem = true, \
        WithImportTextItem = true, \
        WithSerializer = true, \
        WithGetPreloadDependencies = true, \
    }; \

