// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"

#include "DI/Impl/StaticClass.h"

class UObject;
class UClass;

/*
 * Base class for object that can be registered and unregistered in IDLObjectTrackerService
 */
class FDLTrackedObject
{
    friend class UDLObjectTrackerServiceImpl;

protected:
    /* Register object in service. Pass 'this' as Obj */
    template<typename TObj>
    void RegisterObject(UObject* Obj)
    {
        RegisterObject(Obj, UnrealDI_Impl::TStaticClass<TObj>::StaticClass());
    }

    /* Unregister object from service. Pass 'this' as Obj */
    template<typename TObj>
    void UnregisterObject(UObject* Obj)
    {
        UnregisterObject(Obj, UnrealDI_Impl::TStaticClass<TObj>::StaticClass());
    }

private:
    static void RegisterObject(UObject* Obj, UClass* Class);
    static void UnregisterObject(UObject* Obj, UClass* Class);

    static void Cleanup();

private:
    using FMapType = TMap<UClass*, TArray<TWeakObjectPtr<UObject>>>;
    static FMapType Objects;

    using FChangedSignature = TMulticastDelegate<void(UObject* Obj, UClass* Class, bool bRegistered)>;
    static FChangedSignature RegistrationChanged;
};
