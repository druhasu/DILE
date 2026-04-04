// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLSingleWidgetSlot.h"
#include "Utils/DLEnsure.h"
#include "Utils/Widgets/IDLAnimatedWidget.h"
#include "Utils/Widgets/IDLClosableWidget.h"
#include "Utils/Widgets/DLUserWidget.h"

UDLSingleWidgetSlot::UDLSingleWidgetSlot()
{
    PRAGMA_DISABLE_DEPRECATION_WARNINGS;
    // set default visibility value
    Visibility = ESlateVisibility::SelfHitTestInvisible;
    PRAGMA_ENABLE_DEPRECATION_WARNINGS;
}

void UDLSingleWidgetSlot::ReleaseSlateResources(bool bReleaseChildren)
{
    if (MyBox.IsValid())
    {
        UnregisterObject<IDLWidgetSlot>(this);
    }
    if (Content != nullptr)
    {
        Content->ReleaseSlateResources(bReleaseChildren);
        Content = nullptr;
    }
    MyBox.Reset();

    Super::ReleaseSlateResources(bReleaseChildren);
}

void UDLSingleWidgetSlot::SetSlotTag(FGameplayTag InTag)
{
    // we cannot change slot tag after creation/registration
    DL_ENSURE_RETURN(MyBox == nullptr);
    SlotTag = InTag;
}

TSharedRef<SWidget> UDLSingleWidgetSlot::RebuildWidget()
{
    MyBox = SNew(SBox)
        .HAlign(EHorizontalAlignment::HAlign_Fill)
        .VAlign(EVerticalAlignment::VAlign_Fill);

    RegisterObject<IDLWidgetSlot>(this);

    return MyBox.ToSharedRef();
}

FDLTask UDLSingleWidgetSlot::AddWidgetImpl(UWidget* Widget, FForceLatentCoroutine)
{
    DL_ENSURE_CORETURN(Widget != nullptr);

    if (MyBox)
    {
        if (Content != nullptr)
        {
            co_await RemoveWidgetImpl(Content);
        }

        Content = Widget;
        MyBox->SetContent(Content->TakeWidget());

        if (IDLClosableWidget* Closable = Cast<IDLClosableWidget>(Content))
        {
            Closable->OnCloseRequested().AddUObject(this, &ThisClass::OnCloseRequested);
        }

        co_await IDLAnimatedWidget::TryShow(Content);
    }
}

FDLTask UDLSingleWidgetSlot::RemoveWidgetImpl(UWidget* Widget, FForceLatentCoroutine)
{
    DL_ENSURE_CORETURN(Widget != nullptr);

    if (MyBox && Content == Widget)
    {
        co_await IDLAnimatedWidget::TryHide(Content);
        Content = nullptr;
        MyBox->SetContent(SNullWidget::NullWidget);
    }

    co_return;
}

void UDLSingleWidgetSlot::OnCloseRequested(UDLUserWidget* Widget)
{
    Cast<IDLClosableWidget>(Widget)->OnCloseRequested().RemoveAll(this);
    RemoveWidgetImpl(Widget);
}
