// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DataStructNameGraphPanelPinFactory.h"

#include "PropertyCustomizationHelpers.h"
#include "SGraphPin.h"
#include "ScopedTransaction.h"
#include "EdGraphSchema_K2.h"
#include "Systems/NamedStruct/Impl/NamedStructRegistry.h"

class SNamedStructGraphPin : public SGraphPin
{
public:
    using ThisClass = SNamedStructGraphPin;

    SLATE_BEGIN_ARGS(SNamedStructGraphPin)
        : _NameStruct(nullptr)
    {}
    SLATE_ARGUMENT(UScriptStruct*, NameStruct)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

    virtual TSharedRef<SWidget> GetDefaultValueWidget() override;

private:
    void GetOptionNames(TArray<TSharedPtr<FString>>& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const;
    FString GetCurrentValue() const;
    void SetValue(const FString& InValue);

private:
    UScriptStruct* NameStruct;
};

TSharedPtr<SGraphPin> FDataStructNameGraphPanelPinFactory::CreatePin(UEdGraphPin* InPin) const
{
    const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
    if (InPin->PinType.PinCategory == K2Schema->PC_Struct)
    {
        UScriptStruct* ValueStruct = Cast<UScriptStruct>(InPin->PinType.PinSubCategoryObject.Get());

        if (FNamedStructRegistry::Get().IsNameStruct(ValueStruct))
        {
            return SNew(SNamedStructGraphPin, InPin).NameStruct(ValueStruct);
        }
    }

    return nullptr;
}

void SNamedStructGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    NameStruct = InArgs._NameStruct;

    SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget> SNamedStructGraphPin::GetDefaultValueWidget()
{
    FPropertyComboBoxArgs ComboArgs(
        nullptr,
        FOnGetPropertyComboBoxStrings::CreateSP(this, &ThisClass::GetOptionNames),
        FOnGetPropertyComboBoxValue::CreateSP(this, &ThisClass::GetCurrentValue),
        FOnPropertyComboBoxValueSelected::CreateSP(this, &ThisClass::SetValue));

    ComboArgs.ShowSearchForItemCount = 5;

    auto Result = PropertyCustomizationHelpers::MakePropertyComboBox(ComboArgs);
    Result->SetVisibility(TAttribute<EVisibility>(this, &SGraphPin::GetDefaultValueVisibility));
    Result->SetEnabled(TAttribute<bool>(this, &SGraphPin::GetDefaultValueIsEditable));

    return Result;
}

void SNamedStructGraphPin::GetOptionNames(TArray<TSharedPtr<FString>>& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const
{
    OutStrings.Emplace(MakeShared<FString>(TEXT("None")));
    OutRestrictedItems.Add(false);

    for (auto& RowName : FNamedStructRegistry::Get().GetPossibleNames(NameStruct))
    {
        OutStrings.Emplace(MakeShared<FString>(RowName.ToString()));
        OutRestrictedItems.Add(false);
    }
}

FString SNamedStructGraphPin::GetCurrentValue() const
{
    FString Value = GraphPinObj->GetDefaultAsString();
    if (!Value.IsEmpty())
    {
        return Value;
    }

    return TEXT("None");
}

void SNamedStructGraphPin::SetValue(const FString& InValue)
{
    if (GraphPinObj->IsPendingKill())
    {
        return;
    }

    if (GetCurrentValue() != InValue)
    {
        const FScopedTransaction Transaction(NSLOCTEXT("DILEEditor", "ChangeDataStructNamePinValue", "Change DataStructName Pin Value"));
        GraphPinObj->Modify();

        GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, InValue);

        if (OwnerNodePtr.IsValid())
        {
            OwnerNodePtr.Pin()->UpdateGraphNode();
        }
    }
}
