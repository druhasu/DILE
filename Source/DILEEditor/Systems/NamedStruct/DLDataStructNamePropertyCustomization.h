// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "IPropertyTypeCustomization.h"

class SToolTip;

class FDLDataStructNamePropertyCustomization : public IPropertyTypeCustomization
{
public:
    using ThisClass = FDLDataStructNamePropertyCustomization;

    static TSharedRef<IPropertyTypeCustomization> Make(UScriptStruct* NamedStruct) { return MakeShared<FDLDataStructNamePropertyCustomization>(NamedStruct); }

    FDLDataStructNamePropertyCustomization(UScriptStruct* InNamedStruct)
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
