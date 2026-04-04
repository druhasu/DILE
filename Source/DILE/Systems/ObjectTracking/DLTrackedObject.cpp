// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLTrackedObject.h"
#include "Utils/DLEnsure.h"

FDLTrackedObject::FMapType FDLTrackedObject::Objects;
FDLTrackedObject::FChangedSignature FDLTrackedObject::RegistrationChanged;

void FDLTrackedObject::RegisterObject(UObject* Object, UClass* Class)
{
    DL_ENSURE_RETURN(Object != nullptr);
    DL_ENSURE_RETURN(Class != nullptr);

    TArray<TWeakObjectPtr<UObject>>& Array = Objects.FindOrAdd(Class);
    Array.Add(Object);
    RegistrationChanged.Broadcast(Object, Class, true);
}

void FDLTrackedObject::UnregisterObject(UObject* Object, UClass* Class)
{
    DL_ENSURE_RETURN(Object != nullptr);
    DL_ENSURE_RETURN(Class != nullptr);

    TArray<TWeakObjectPtr<UObject>>* Array = Objects.Find(Class);
    if (ensureMsgf(Array != nullptr, TEXT("Object '%s' is not registered"), *GetNameSafe(Object)))
    {
        Array->RemoveSingleSwap(Object);
        RegistrationChanged.Broadcast(Object, Class, false);
    }
}

void FDLTrackedObject::Cleanup()
{
    for (FMapType::TIterator It = Objects.CreateIterator(); It; ++It)
    {
        for (auto ArrIt = It.Value().CreateIterator(); ArrIt; ++ArrIt)
        {
            if (!ArrIt->IsValid())
            {
                ArrIt.RemoveCurrentSwap();
            }
        }
    }
}
