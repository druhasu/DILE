// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "Systems/NamedStruct/Impl/NamedStructRegistry.h"

#include "DataStructName.generated.h"

/*
 Base type for creating a Named Struct type.

 This system allows you to create a piece of data (represented by a struct) that have a unique name.

 The name is represented by a strongly typed value. And this name allows you to easily retrieve the associated data.
 It also has Blueprint and Editor support, allowing to select a value using a combo box

 Example of defining such data+name pair is quite simple:

    #include "Engine/DataTable.h"
    #include "Systems/NamedStruct/DataStructName.h"

    // Struct used as an identifier (NameStruct)
    USTRUCT()
    struct FMyCustomName : public FDataStructName, public TDataStructName<FMyCustomName, FMyCustomData>
    {
        GENERATED_BODY()
        IMPLEMENT_DATA_STRUCT_NAME(FMyCustomName)
    };

    // Struct with data (DataStruct)
    USTRUCT()
    struct FMyCustomData : public FTableRowBase
    {
        GENERATED_BODY()
    public:
        // add necessary UPROPERTYs
    };

 Base type of DataStruct (FTableRowBase) dictates where this data is stored. In this example - inside the DataTable.
 Any data table containing this struct will be detected and processed, no additional setup required.

 Another option is to store data inside DataAssets. In this case single value is stored inside each asset.
 Chose the option that works better for each case.

 To place DataStruct inside the DataAsset, use this example:

    #include "Systems/NamedStruct/DataStructName.h"
    #include "Systems/NamedStruct/NamedStructDataAsset.h"

    // Struct used as an identifier (NameStruct) - same as in example above
    USTRUCT()
    struct FMyCustomName : public FDataStructName, public TDataStructName<FMyCustomName, FMyCustomData>
    {
        GENERATED_BODY()
        IMPLEMENT_DATA_STRUCT_NAME(FMyCustomName)
    };

    // Struct with data (DataStruct)
    USTRUCT()
    struct FMyCustomData
    {
        GENERATED_BODY()
    public:
        // add necessary UPROPERTYs
    };

    // Asset with data
    UCLASS()
    class UMyCustomDataAsset : public UNamedStructDataAsset
    {
        GENERATED_BODY()

    public:
        UPROPERTY(EditDefaultsOnly, meta = (ShowOnlyInnerProperties))
        FMyCustomData Data;
    };

 Important part here is to create subclass of UNamedStructDataAsset that have a single UPROPERTY of your struct's type.
 Name of the property does not matter, choose the one that better suits your use case

 To access data using the name:

    FMyCustomDataName Name(TEXT("CoolName"));
    const FMyCustomData* DataPtr = Name.GetValuePtr();

 For more practical examples see usages of FDataStructName across the project

*/

USTRUCT(BlueprintType)
struct FDataStructName
{
    GENERATED_BODY()
};

template <typename TNameStruct, typename TDataStruct>
struct TDataStructName
{
public:
    using FNameType = TNameStruct;
    using FDataType = TDataStruct;

    /* Default construct. Creates Name that has None value */
    TDataStructName()
    {
        const uint8& Register = Registrar;
    }

    /* Constructor from existing FName */
    TDataStructName(FName InName)
        : Name(InName)
    {
    }

    /* List of all possible/valid names that this Name may have */
    static TArrayView<const FNameType> GetPossibleNames()
    {
        TArrayView<const FName> Names = FNamedStructRegistry::Get().GetPossibleNames(StaticStruct());
        return MakeArrayView(reinterpret_cast<const FNameType*>(Names.GetData()), Names.Num());
    }

    /* Pointer to a Data, or nullptr if name is not set or not valid */
    const FDataType* GetValuePtr() const
    {
        return (const FDataType*)FNamedStructRegistry::Get().GetValuePtr(StaticStruct(), Name);
    }

    /* Whether Name is not set */
    bool IsNone() const { return Name.IsNone(); }

    /* String representation of the name */
    FString ToString() const { return Name.ToString(); }

    /* Conversion to FName */
    FName operator*() const { return Name; }

    friend bool operator== (const FNameType& A, const FNameType& B)
    {
        return A.Name == B.Name;
    }

    friend bool operator!= (const FNameType& A, const FNameType& B)
    {
        return A.Name != B.Name;
    }

    friend uint32 GetTypeHash(const FNameType& Struct)
    {
        return GetTypeHash(Struct.Name);
    }

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

protected:
    using ImplType = TDataStructName<FNameType, FDataType>;

private:
    static UScriptStruct* StaticStruct()
    {
        return FNameType::StaticStruct();
    }

    static uint8 Registrar;

    FName Name;
};

template <typename TKeyStruct, typename TValueStruct>
uint8 TDataStructName<TKeyStruct, TValueStruct>::Registrar = FNamedStructRegistry::Get().RegisterNamedStruct<TKeyStruct, TValueStruct>();


#define IMPLEMENT_DATA_STRUCT_NAME(StructName) \
public: \
    StructName() = default; \
    StructName(FName InName) : ImplType(InName) {}; \
    StructName(const StructName&) = default; \
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

/*
 * Helper for creating temporary values of Named Structs
 */
template<typename T>
struct TDataStructName_Temporary
{
    TDataStructName_Temporary(FName InName)
        : Name(InName)
    {
        FNamedStructRegistry::Get().AddTemporaryValue(T::FNameType::StaticStruct(), Name, &Value);
    }

    ~TDataStructName_Temporary()
    {
        FNamedStructRegistry::Get().RemoveTemporaryValue(T::FNameType::StaticStruct(), Name);
    }

    TDataStructName_Temporary(const TDataStructName_Temporary&) = delete;
    TDataStructName_Temporary(TDataStructName_Temporary&&) = delete;

    typename T::FNameType AsDataStructName() const
    {
        return T::FNameType(Name);
    }

    operator typename T::FNameType() const
    {
        return AsDataStructName();
    }

    typename T::FDataType Value;

private:
    FName Name;
};
