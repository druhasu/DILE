// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "IPropertyTypeCustomization.h"

class FDataStructNamePropertyCustomization : public IPropertyTypeCustomization
{
public:
    using ThisClass = FDataStructNamePropertyCustomization;

    static TSharedRef<IPropertyTypeCustomization> Make(UScriptStruct* NamedStruct) { return MakeShared<FDataStructNamePropertyCustomization>(NamedStruct); }

    FDataStructNamePropertyCustomization(UScriptStruct* InNamedStruct)
        : NamedStruct(InNamedStruct)
    {}

    virtual bool ShouldInlineKey() const override { return true; }
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override {}

private:
    void GetOptionNames(TArray<TSharedPtr<FString>>& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const;
    FString GetCurrentValue() const;
    void SetNewValue(const FString&);

private:
    UScriptStruct* NamedStruct;
    TSharedPtr<IPropertyHandle> PropertyHandle;
};
