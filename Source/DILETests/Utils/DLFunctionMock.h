// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Templates/Function.h"

namespace DLFunctionMock_Private
{
    template<typename T>
    struct TFunctionCallbackType;

    template<typename TResult, typename TClass, typename... TArgs>
    struct TFunctionCallbackType<TResult(TClass::*)(TArgs...)>
    {
        using FunctionType = TFunction<TResult(TArgs...)>;
    };

    template<typename TResult, typename TClass, typename... TArgs>
    struct TFunctionCallbackType<TResult(TClass::*)(TArgs...) const>
    {
        using FunctionType = TFunction<TResult(TArgs...)>;
    };

    template <typename T>
    using TFunctionCallbackType_T = typename TFunctionCallbackType<T>::FunctionType;
}

#define DL_MOCK_FUNCTION_BODY(DefaultResult, FunctionName, ...) \
    if(FunctionName ## Callback) \
    { \
        return FunctionName ## Callback( __VA_ARGS__ ); \
    } \
    return DefaultResult

#define DL_MOCK_FUNCTION_VARIABLE(FunctionName) \
    DLFunctionMock_Private::TFunctionCallbackType_T<decltype(&ThisClass::FunctionName)> FunctionName ## Callback

