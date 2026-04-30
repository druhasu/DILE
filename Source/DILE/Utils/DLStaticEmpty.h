// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

/*
 * Helper struct to provide static Empty value
 * Intended to be used in methods that should return const reference to some container, but rely on value from some other place.
 * You can define a static variable inside the method and return it, but all of those functions will have separate objects created by compiler
 * This struct allows to reuse single object of each type between all of those functions
 *
 * Usage Example:
 *
 * const TArray<int32>& GetIndexes() const
 * {
 *     return MyObject != nullptr ? MyObject->GetActualIndexes() : TDLStaticEmpty<TArray<int32>>::Value;
 * }
 */
template <typename TValue>
struct TDLStaticEmpty
{
    static const TValue Value;
};

template <typename TValue>
const TValue TDLStaticEmpty<TValue>::Value = {};
