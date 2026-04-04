// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLObjectTrackerService.generated.h"

struct CStaticInterfaceProvider
{
    template <typename T>
    auto Requires(UClass*& ClassRef) -> decltype(
        ClassRef = T::UClassType::StaticClass()
    );
};

UINTERFACE(MinimalApi)
class UDLObjectTrackerService : public UInterface { GENERATED_BODY() };

/*
 * Allows tracking of registration of Objects
 */
class DILE_API IDLObjectTrackerService
{
    GENERATED_BODY()

public:
    /* Iterates over all objects of given class until requested to stop by returning false from a lambda */
    virtual void ForEachObject(UClass* Class, TFunctionRef<bool(UObject* Object)> Action) const = 0;

    /* Removes Register subscription for specified class */
    virtual void RemoveRegister(UClass* InClass, UObject* Listener) = 0;

    /* Removes Unregister subscription for specified class */
    virtual void RemoveUnregister(UClass* InClass, UObject* Listener) = 0;

    /* Removes Register subscription for specified class */
    virtual void RemoveRegister(UClass* InClass, FDelegateHandle Handle) = 0;

    /* Removes Unregister subscription for specified class */
    virtual void RemoveUnregister(UClass* InClass, FDelegateHandle Handle) = 0;

    /* Iterates over all objects of given class until requested to stop by returning false from a lambda */
    template<typename TObj>
    void ForEachObject(TFunctionRef<bool(TObj* Object)> Action) const
    {
        ForEachObject(TObj::StaticClass(), [&](UObject* Object)
        {
            return Action((TObj*)Object);
        });
    }

    /* Subscribes for Registration events for given class. Immediately calls event for all already registered objects */
    template <typename TObject, typename TSubscriber UE_REQUIRES(TIsDerivedFrom<TObject, UObject>::Value)>
    FDelegateHandle OnRegister(TSubscriber* Subscriber, typename TMemFunPtrType<false, TSubscriber, void(TObject*)>::Type Method)
    {
        return OnRegisterImpl(TObject::StaticClass(), FChangeDelegate::CreateWeakLambda(Subscriber, [Subscriber, Method](UObject* Obj) { (Subscriber->*Method)((TObject*)Obj); }));
    }

    /* Subscribes for Registration events for given interface. Immediately calls event for all already registered objects */
    template <typename TInterface, typename TSubscriber UE_REQUIRES(TModels_V<CStaticInterfaceProvider, TInterface>)>
    FDelegateHandle OnRegister(TSubscriber* Subscriber, typename TMemFunPtrType<false, TSubscriber, void(TScriptInterface<TInterface>)>::Type Method)
    {
        return OnRegisterImpl(TInterface::UClassType::StaticClass(), FChangeDelegate::CreateWeakLambda(Subscriber, [Subscriber, Method](UObject* Obj) { (Subscriber->*Method)(TScriptInterface<TInterface>(Obj)); }));
    }

    /* Subscribes for Unregistration events for given class . Immediately calls event for all already registered objects */
    template <typename TObject, typename TSubscriber UE_REQUIRES(TIsDerivedFrom<TObject, UObject>::Value)>
    FDelegateHandle OnUnregister(TSubscriber* Subscriber, typename TMemFunPtrType<false, TSubscriber, void(TObject*)>::Type Method)
    {
        return OnUnregisterImpl(TObject::StaticClass(), FChangeDelegate::CreateWeakLambda(Subscriber, [Subscriber, Method](UObject* Obj) { (Subscriber->*Method)((TObject*)Obj); }));
    }

    /* Subscribes for Unregistration events for given interface. Immediately calls event for all already registered objects */
    template <typename TInterface, typename TSubscriber UE_REQUIRES(TModels_V<CStaticInterfaceProvider, TInterface>)>
    FDelegateHandle OnUnregister(TSubscriber* Subscriber, typename TMemFunPtrType<false, TSubscriber, void(TScriptInterface<TInterface>)>::Type Method)
    {
        return OnUnregisterImpl(TInterface::UClassType::StaticClass(), FChangeDelegate::CreateWeakLambda(Subscriber, [Subscriber, Method](UObject* Obj) { (Subscriber->*Method)(TScriptInterface<TInterface>(Obj)); }));
    }

    /* Removes Register subscription for specified class */
    template <typename TObject UE_REQUIRES(TIsDerivedFrom<TObject, UObject>::Value)>
    void RemoveRegister(UObject* Listener) { RemoveRegister(TObject::StaticClass(), Listener); }

    /* Removes Unregister subscription for specified class */
    template <typename TObject UE_REQUIRES(TIsDerivedFrom<TObject, UObject>::Value)>
    void RemoveUnregister(UObject* Listener) { RemoveUnregister(TObject::StaticClass(), Listener); }

    /* Removes Register subscription for specified interface */
    template <typename TInterface UE_REQUIRES(TModels_V<CStaticInterfaceProvider, TInterface>)>
    void RemoveRegister(UObject* Listener) { RemoveRegister(TInterface::UClassType::StaticClass(), Listener); }

    /* Removes Unregister subscription for specified interface */
    template <typename TInterface UE_REQUIRES(TModels_V<CStaticInterfaceProvider, TInterface>)>
    void RemoveUnregister(UObject* Listener) { RemoveUnregister(TInterface::UClassType::StaticClass(), Listener); }

protected:
    using FChangeDelegate = TDelegate<void(UObject*)>;

    /* Subscribes for Registration events for given class or interface. Immediately calls event for all already registered objects */
    virtual FDelegateHandle OnRegisterImpl(UClass* InClass, FChangeDelegate&& Delegate) = 0;

    /* Subscribes for Unregistration events for given class or interface. Immediately calls event for all already registered objects */
    virtual FDelegateHandle OnUnregisterImpl(UClass* InClass, FChangeDelegate&& Delegate) = 0;
};
