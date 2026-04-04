// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Components/Widget.h"
#include "Systems/WidgetSlots/IDLWidgetSlot.h"

#include "DLSingleWidgetSlot.generated.h"

class UDLUserWidget;

UCLASS()
class DILE_API UDLSingleWidgetSlot : public UWidget, public IDLWidgetSlot
{
    GENERATED_BODY()

public:
    UDLSingleWidgetSlot();

    // Begin IWIWidgetSlot
    FGameplayTag GetSlotTag() const override { return SlotTag; }
    TArrayView<TObjectPtr<UWidget>> GetWidgets() const override { return MakeArrayView(const_cast<TObjectPtr<UWidget>*>(&Content), 1); }
    FDLTask AddWidget(UWidget* Widget, float Priority = 0.f) override { return AddWidgetImpl(Widget); }
    FDLTask RemoveWidget(UWidget* Widget) override { return RemoveWidgetImpl(Widget); }
    // End IWIWidgetSlot

    void ReleaseSlateResources(bool bReleaseChildren) override;

    void SetSlotTag(FGameplayTag InTag);

protected:
    TSharedRef<SWidget> RebuildWidget() override;

protected:
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (Categories = "WidgetSlot"))
    FGameplayTag SlotTag;

private:
    FDLTask AddWidgetImpl(UWidget* Widget, FForceLatentCoroutine = {});
    FDLTask RemoveWidgetImpl(UWidget* Widget, FForceLatentCoroutine = {});
    void OnCloseRequested(UDLUserWidget* Widget);

private:
    UPROPERTY()
    TObjectPtr<UWidget> Content;

    TSharedPtr<SBox> MyBox;
};
