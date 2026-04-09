// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Internationalization/Text.h"
#include "Misc/DataValidation.h"

#if WITH_EDITOR

/*
 * Helper macro for IsDataValid implementation
 * Assumes that FDataValidationContext parameter is called Context
 *
 * Usage example:
 *
 *    DL_VALIDATE_DATA(IconTexture != nullptr, "Icon must be set");
 *    DL_VALIDATE_DATA(!NpcName.IsEmpty(), "Npc '{0}' must have name", NpcID);
 */
#define DL_VALIDATE_DATA(Condition, ErrorFormat, ...) \
    if (!(Condition)) \
    { \
        Context.AddError(DLValidationUtils_Private::FormatOrderedOrSimple(TEXT(ErrorFormat), ##__VA_ARGS__)); \
    }

/*
 * Helper macro for returning result from IsDataValid funtion.
 * Assumes that FDataValidationContext parameter is called Context
 *
 * Usage example:
 *
 *    return DL_VALIDATION_RESULT();
 */
#define DL_VALIDATION_RESULT() \
    Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid

namespace DLValidationUtils_Private
{
    static FText ToText(FString&& Str) { return FText::FromString(Str); }
    static FText ToText(FName Name) { return FText::FromName(Name); }

    template <typename T>
    static auto ToText(T&& Arg) { return Forward<T>(Arg); }

    /* Helper template that wraps FText::FormatOrdered and allows to call it without arguments */
    template<typename... TArgs>
    FText FormatOrderedOrSimple(const FString& Format, TArgs&&... Args)
    {
        return FText::FormatOrdered(FText::FromString(Format), ToText(Forward<TArgs>(Args))...);
    }

    static FText FormatOrderedOrSimple(const FString& Message)
    {
        return FText::FromString(Message);
    }
}

#endif
