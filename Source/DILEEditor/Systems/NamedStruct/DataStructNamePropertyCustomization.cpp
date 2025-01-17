// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DataStructNamePropertyCustomization.h"

#include "Systems/NamedStruct/Impl/NamedStructRegistry.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"

void FDataStructNamePropertyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    PropertyHandle = InPropertyHandle;

    FPropertyComboBoxArgs ComboArgs(
        nullptr,
        FOnGetPropertyComboBoxStrings::CreateSP(this, &ThisClass::GetOptionNames),
        FOnGetPropertyComboBoxValue::CreateSP(this, &ThisClass::GetCurrentValue),
        FOnPropertyComboBoxValueSelected::CreateSP(this, &ThisClass::SetNewValue));

    ComboArgs.ShowSearchForItemCount = 5;

    HeaderRow.NameContent()
    [
        PropertyHandle->CreatePropertyNameWidget()
    ]
    .ValueContent()
    [
        PropertyCustomizationHelpers::MakePropertyComboBox(ComboArgs)
    ];
}

void FDataStructNamePropertyCustomization::GetOptionNames(TArray<TSharedPtr<FString>>& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const
{
    OutStrings.Emplace(MakeShared<FString>(TEXT("None")));
    OutRestrictedItems.Add(false);

    for (auto& RowName : FNamedStructRegistry::Get().GetPossibleNames(NamedStruct))
    {
        OutStrings.Emplace(MakeShared<FString>(RowName.ToString()));
        OutRestrictedItems.Add(false);
    }
}

FString FDataStructNamePropertyCustomization::GetCurrentValue() const
{
    FString Value;
    if (PropertyHandle->GetValueAsFormattedString(Value) == FPropertyAccess::Success)
    {
        return Value;
    }

    return {};
}

void FDataStructNamePropertyCustomization::SetNewValue(const FString& InValue)
{
    PropertyHandle->SetValueFromFormattedString(InValue);
}
