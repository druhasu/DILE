// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#define DL_ENSURE_RETURN(Condition, ...) \
    if (!ensureAlways(Condition)) \
    { \
        return __VA_ARGS__; \
    }

#define DL_ENSURE_CORETURN(Condition, ...) \
    if (!ensure(Condition)) \
    { \
        co_return __VA_ARGS__; \
    }
