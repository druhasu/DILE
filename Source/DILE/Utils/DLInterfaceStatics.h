// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "DLInterfaceStatics.generated.h"

UCLASS()
class DILE_API UDLInterfaceStatics : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /*
     * Returns implementation of given interface inside Target object
     * This function checks object itself, its Components (if Actor) and its attachment parent
     */
    UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "Interface"))
    static TScriptInterface<IInterface> GetInterfaceFromObject(UObject* Target, TSubclassOf<UInterface> Interface)
    {
        UObject* ImplementingObject = FindImplementingObject(Target, Interface);
        if (ImplementingObject != nullptr)
        {
            // manually construct TScriptInterface<IInterface>, because its constructor does not propertly set its InterfacePointer
            TScriptInterface<IInterface> Result;
            Result.SetObject(ImplementingObject);
            Result.SetInterface((IInterface*)ImplementingObject->GetInterfaceAddress(*Interface));

            return Result;
        }

        return {};
    }

    /*
     * Returns implementation of given interface inside Target object
     * This function checks object itself, its Components (if Actor) and its attachment parent
     */
    template<typename T>
    static TScriptInterface<T> GetInterfaceFromObject(UObject* Target)
    {
        return FindImplementingObject(Target, T::UClassType::StaticClass());
    }

private:
    static UObject* FindImplementingObject(UObject* Target, UClass* InterfaceClass);
};

template<typename T>
TScriptInterface<T> GetInterfaceFromObject(UObject* Target)
{
    return UDLInterfaceStatics::GetInterfaceFromObject<T>(Target);
}
