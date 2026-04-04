// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContentPresenterWidget.h"
#include "Systems/ViewFactory/IDLViewFactory.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/Layout/SBox.h"

void UDLContentPresenterWidget::InitDependencies(TScriptInterface<IDLViewFactory> InViewFactory)
{
    ViewFactory = InViewFactory;
}

void UDLContentPresenterWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (!MyBox)
    {
        return;
    }

    MyBox->SetPadding(ContentPadding);
    MyBox->SetHAlign(ContentHorizontalAlignment);
    MyBox->SetVAlign(ContentVerticalAlignment);

    // TODO: Expose Width/Height overrides if needed
}

void UDLContentPresenterWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    if (CurrentViewWidget != nullptr)
    {
        CurrentViewWidget->ReleaseSlateResources(bReleaseChildren);
    }

    MyBox.Reset();
}

void UDLContentPresenterWidget::ClearContent()
{
    if (CurrentViewWidget != nullptr)
    {
        CurrentViewWidget->ReleaseSlateResources(true);
        CurrentViewWidget = nullptr;

        if (MyBox)
        {
            MyBox->SetContent(SNullWidget::NullWidget);
        }
    }

    CurrentContent.Reset();
}

TSharedRef<SWidget> UDLContentPresenterWidget::RebuildWidget()
{
    MyBox = SNew(SBox);
    TryInitDependencies(this);

    if (CurrentViewWidget != nullptr)
    {
        MyBox->SetContent(CurrentViewWidget->TakeWidget());
    }

    return MyBox.ToSharedRef();
}

void UDLContentPresenterWidget::K2_SetContent_Implementation(const FDLViewContent& Content)
{
    if (CurrentContent == Content)
    {
        // no need to change anything
        return;
    }

    if (!Content.HasContent())
    {
        // requested to set empty content, just clear
        ClearContent();
        return;
    }

    // check whether Content is already a widget
    if (Content.HasObject())
    {
        if (UUserWidget* UserWidgetContent = Cast<UUserWidget>(Content.GetObject()))
        {
            CurrentContent = Content;
            SetViewWidget(UserWidgetContent);
            return;
        }
    }

    // find View class for content
    UClass* ViewClass = FindViewClass(Content);
    if (ViewClass == nullptr)
    {
        ClearContent();
        return;
    }

    // create View of required class
    CurrentViewWidget = ViewFactory->CreateView<UUserWidget>(ViewClass, Content);
    if (CurrentViewWidget == nullptr)
    {
        // view cannot be created, just clear
        ClearContent();
        return;
    }

    CurrentContent = Content;
    SetViewWidget(CurrentViewWidget);
}

UClass* UDLContentPresenterWidget::FindViewClass(const FDLViewContent& Content) const
{
    UClass* Result = nullptr;

    if (Content.HasObject())
    {
        // check whether Content is a class
        UObject* ContentObject = Content.GetObject();
        UClass* ViewClass = Cast<UClass>(ContentObject);

        if (ViewClass == nullptr)
        {
            // not a class, search for association by class of Content
            const TSubclassOf<UUserWidget>* FoundViewClass = ClassViews.Find(ContentObject->GetClass());
            Result = FoundViewClass != nullptr ? **FoundViewClass : nullptr;
        }
        else if (!ViewClass->IsChildOf<UUserWidget>())
        {
            // is a class, but not a class of widget -> search association by the class itself
            const TSubclassOf<UUserWidget>* FoundViewClass = ClassViews.Find(ViewClass);
            Result = FoundViewClass != nullptr ? **FoundViewClass : nullptr;
        }
        else
        {
            // is already a class of widget - leave as it is
        }
    }
    else if (Content.HasStruct())
    {
        // find view class based on class of content
        const TSubclassOf<UUserWidget>* FoundViewClass = StructViews.Find(Content.GetStruct().GetScriptStruct());
        Result = FoundViewClass != nullptr ? **FoundViewClass : nullptr;
    }

    return Result;
}

void UDLContentPresenterWidget::SetViewWidget(UUserWidget* Widget)
{
    if (CurrentViewWidget != nullptr)
    {
        CurrentViewWidget->ReleaseSlateResources(true);
    }

    CurrentViewWidget = Widget;

    if (MyBox)
    {
        MyBox->SetContent(CurrentViewWidget->TakeWidget());
    }
}
