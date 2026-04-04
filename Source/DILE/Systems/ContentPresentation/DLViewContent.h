// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "StructUtils/StructView.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DLViewContent.generated.h"

USTRUCT(BlueprintType)
struct FDLViewContent
{
    GENERATED_BODY()

public:
    FDLViewContent()
        : ContentData(TInPlaceType<FNoopStruct>())
    {
    }

    FDLViewContent(TObjectPtr<UObject> InObjectContent)
        : ContentData(TInPlaceType<TObjectPtr<UObject>>(), InObjectContent)
    {
    }

    FDLViewContent(FConstStructView InStructContent)
        : ContentData(TInPlaceType<FInstancedStruct>(), InStructContent)
    {
    }

    FDLViewContent(FInstancedStruct InStructContent)
        : ContentData(TInPlaceType<FInstancedStruct>(), MoveTemp(InStructContent))
    {
    }

    bool HasContent() const
    {
        return !ContentData.IsType<FNoopStruct>();
    }

    bool HasObject() const
    {
        return ContentData.IsType<TObjectPtr<UObject>>();
    }

    TObjectPtr<UObject> GetObject() const
    {
        return ContentData.Get<TObjectPtr<UObject>>();
    }

    bool HasStruct() const
    {
        return ContentData.IsType<FInstancedStruct>();
    }

    FConstStructView GetStruct() const
    {
        return ContentData.Get<FInstancedStruct>();
    }

    void Reset()
    {
        ContentData.Emplace<FNoopStruct>();
    }

    void AddStructReferencedObjects(class FReferenceCollector& Collector)
    {
        if (HasObject())
        {
            Collector.AddReferencedObject(ContentData.Get<TObjectPtr<UObject>>());
        }
        else if (HasStruct())
        {
            ContentData.Get<FInstancedStruct>().AddStructReferencedObjects(Collector);
        }
    }

    friend bool operator== (const FDLViewContent& A, const FDLViewContent& B)
    {
        if (A.ContentData.GetIndex() != B.ContentData.GetIndex())
        {
            return false;
        }

        switch (A.ContentData.GetIndex())
        {
            case 0:
                return true;
            case 1:
                return A.GetObject() == B.GetObject();
            case 2:
            {
                FConstStructView AView = A.GetStruct();
                FConstStructView BView = B.GetStruct();

                if (AView.GetScriptStruct() != BView.GetScriptStruct())
                {
                    return false;
                }

                if (AView.GetScriptStruct() == nullptr)
                {
                    // we already know that Structs are equal, enough to compare only one of them
                    // if both structs are nullptr, treat them as same
                    return true;
                }

                return AView.GetScriptStruct()->CompareScriptStruct(AView.GetMemory(), BView.GetMemory(), 0);
            }
            default:
                checkNoEntry();
                return false;
        }
    }

    friend bool operator!= (const FDLViewContent& A, const FDLViewContent& B)
    {
        return !(A == B);
    }

private:
    TVariant<FNoopStruct, TObjectPtr<UObject>, FInstancedStruct> ContentData;
};

template<>
struct TStructOpsTypeTraits<FDLViewContent> : public TStructOpsTypeTraitsBase2<FDLViewContent>
{
    enum
    {
        WithAddStructReferencedObjects = true,
    };
};

UCLASS()
class UDLViewContentLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (NativeMakeFunc))
    static FDLViewContent MakeObjectViewContent(UObject* InObjectContent)
    {
        return FDLViewContent(InObjectContent);
    }

    UFUNCTION(BlueprintCallable, meta = (NativeMakeFunc))
    static FDLViewContent MakeStructViewContent(FInstancedStruct InStructContent)
    {
        return FDLViewContent(MoveTemp(InStructContent));
    }
};
