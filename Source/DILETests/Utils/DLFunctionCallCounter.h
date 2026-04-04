// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Templates/Function.h"

/* Common base class for TFunctionCallCounter */
template<typename TResult, typename... TArgs>
struct TFunctionCallCounterBase
{
public:
    using FArgsTuple = TTuple<typename TDecay<TArgs>::Type...>;

public:
    ~TFunctionCallCounterBase()
    {
        Function = MoveTemp(OriginalFunction);
    }

    /* Whether the function was called at least once */
    bool WasCalled() const
    {
        return Calls.Num() > 0;
    }

    /* Whether the function was called exactly once */
    bool WasCalledOnce() const
    {
        return Calls.Num() == 1;
    }

public:
    /* All calls made to function */
    TArray<FArgsTuple> Calls;

protected:
    TFunctionCallCounterBase(TFunction<TResult(TArgs...)>& InFunction)
        : OriginalFunction(InFunction)
        , Function(InFunction)
    {}

    TFunction<TResult(TArgs...)> OriginalFunction;
    TFunction<TResult(TArgs...)>& Function;
};

/*
  Helper struct to simplify writing tests that check if TFunction was called and had correct values in arguments.
  This struct overrides provided function, records all calls to it, and restores original function when destroyed.
  Be careful when selecting whic MakeFunctionCallCounter overload to use, they all have slightly different behaviour. Read their descriptions.

  Example:

    auto Counter = MakeFunctionCallCounter(SomeServiceMock->DoSomethingCallback);

    SomeServiceMock->DoSomething(42, Service->GetCurrentObject(), TEXT("Some text")); // assume that it will call DoSomethingCallback

    DileTestTrue("Event called", Counter.WasCalled());
    DileTestTrue("Event called", Counter.WasCalledOnce());
    DileTestEqual("First argument", Counter.Calls[0].Get<0>(), 42);
    DileTestEqual("String argument", Counter.Calls[0].Get<FString>(), FString("Some text"));
    DileTestEqual("Object argument", Counter.Calls[0].Get<UObject*>(), Service->GetCurrentObject());

 */
template<typename TResult, typename... TArgs>
struct TFunctionCallCounter;

template<typename TResult, typename... TArgs>
struct TFunctionCallCounter : public TFunctionCallCounterBase<TResult, TArgs...>
{
public:
    TFunctionCallCounter(TFunction<TResult(TArgs...)>& InFunction, TFunction<TResult(TArgs...)>&& InResultProvider)
        : TFunctionCallCounterBase<TResult, TArgs...>(InFunction)
    {
        // if InResultProvider is not set, then we will crash if we attempt to call it
        // we must ensure always so all affected tests will be marked as failed
        if (ensureAlways(InResultProvider))
        {
            this->Function = [this, ResultProvider = MoveTemp(InResultProvider)](TArgs... InArgs)
            {
                this->Calls.Add(MakeTuple(InArgs...));
                return ResultProvider(Forward<TArgs>(InArgs)...);
            };
        }
    }
};

template<typename... TArgs>
struct TFunctionCallCounter<void, TArgs...> : public TFunctionCallCounterBase<void, TArgs...>
{
public:
    TFunctionCallCounter(TFunction<void(TArgs...)>& InFunction, TFunction<void(TArgs...)>&& InSideEffect)
        : TFunctionCallCounterBase<void, TArgs...>(InFunction)
    {
        this->Function = [this, SideEffect = MoveTemp(InSideEffect)](TArgs... InArgs)
        {
            this->Calls.Add(MakeTuple(InArgs...));

            // if SideEffect is not set, we can simply ignore it
            if (SideEffect)
            {
                SideEffect(Forward<TArgs>(InArgs)...);
            }
        };
    }
};

/*
  Helper function to create TFunctionCallCounter without manually specifying all template arguments

  @param Function  Function to count calls to. It will be called to obtain return value, so it must be already bound
 */
template<typename TResult, typename... TArgs>
TFunctionCallCounter<TResult, TArgs...> MakeFunctionCallCounter(TFunction<TResult(TArgs...)>& Function)
{
    return TFunctionCallCounter<TResult, TArgs...>(Function, CopyTemp(Function));
}

/*
  Helper function to create TFunctionCallCounter without manually specifying all template arguments

  @param Function  Function to count calls to. Will NOT be called, even if already bound
  @param Result    Value that will be returned from a call to Function
 */
template<typename TResult, typename... TArgs>
TFunctionCallCounter<TResult, TArgs...> MakeFunctionCallCounter(TFunction<TResult(TArgs...)>& Function, TResult&& Result)
{
    return TFunctionCallCounter<TResult, TArgs...>(Function, [Result = MoveTemp(Result)](TArgs...)
    {
        return Result;
    });
}

/*
  Helper function to create TFunctionCallCounter without manually specifying all template arguments

  @param Function          Function to count calls to. Will NOT be called, even if already bound
  @param ResultProvider    Function that will be called to obtain return value. Must be bound
 */
template<typename TResult, typename... TArgs>
TFunctionCallCounter<TResult, TArgs...> MakeFunctionCallCounter(TFunction<TResult(TArgs...)>& Function, TFunction<TResult(TArgs...)>&& ResultProvider)
{
    return TFunctionCallCounter<TResult, TArgs...>(Function, MoveTemp(ResultProvider));
}

/*
  Helper function to create TFunctionCallCounter without manually specifying all template arguments

  @param Function  Function to count calls to. Will be called only if already bound, otherwise ignored
 */
template<typename... TArgs>
TFunctionCallCounter<void, TArgs...> MakeFunctionCallCounter(TFunction<void(TArgs...)>& Function)
{
    return TFunctionCallCounter<void, TArgs...>(Function, CopyTemp(Function));
}

/*
  Helper function to create TFunctionCallCounter without manually specifying all template arguments

  @param Function      Function to count calls to. Will NOT be called, even if already bound
  @param InSideEffect  Additional function to call when Function is called
 */
template<typename... TArgs>
TFunctionCallCounter<void, TArgs...> MakeFunctionCallCounter(TFunction<void(TArgs...)>& Function, TFunction<void(TArgs...)>&& InSideEffect)
{
    return TFunctionCallCounter<void, TArgs...>(Function, MoveTemp(InSideEffect));
}
