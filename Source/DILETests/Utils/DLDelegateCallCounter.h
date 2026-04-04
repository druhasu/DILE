// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Delegates/DelegateSignatureImpl.inl"

/*
  Helper struct to simplify writing tests that check if event was called and had correct values in arguments.
  This struct subscribes to provided event, records all calls to it, and unsubscribes when destroyed

  Example:

    auto Counter = MakeDelegateCallCounter(SomeService->OnSomethingHappened());

    DileTestTrue("Event called", Counter.WasCalled());
    DileTestTrue("Event called", Counter.WasCalledOnce());
    DileTestEqual("First argument", Counter.Calls[0].Get<0>(), 42);
    DileTestEqual("String argument", Counter.Calls[0].Get<FString>(), FString("Some text"));
    DileTestEqual("Object argument", Counter.Calls[0].Get<UObject*>(), Service->GetCurrentObject());

 */
template<typename... TArgs>
struct TDelegateCallCounter
{
public:
    using FArgsTuple = TTuple<typename TDecay<TArgs>::Type...>;

public:
    TDelegateCallCounter(TMulticastDelegate<void(TArgs...)>& InDelegate)
        : Delegate(InDelegate)
    {
        Handle = Delegate.AddLambda([this](TArgs... InArgs)
        {
            Calls.Add(MakeTuple(InArgs...));
        });
    }

    ~TDelegateCallCounter()
    {
        Delegate.Remove(Handle);
    }

    /* Whether the delegate was broadcasted at least once */
    bool WasCalled() const
    {
        return Calls.Num() > 0;
    }

    /* Whether the delegate was broadcasted exactly once */
    bool WasCalledOnce() const
    {
        return Calls.Num() == 1;
    }

public:
    /* All broadcasts made by delegate */
    TArray<FArgsTuple> Calls;

private:
    TMulticastDelegate<void(TArgs...)>& Delegate;
    FDelegateHandle Handle;
};

/*
  Helper function to create TDelegateCallCounter without manually specifying all template arguments
 
  @param Delegate - Delegate to count calls to
 */
template<typename... TArgs>
TDelegateCallCounter<TArgs...> MakeDelegateCallCounter(TMulticastDelegate<void(TArgs...)>& Delegate)
{
    return TDelegateCallCounter<TArgs...>(Delegate);
}
