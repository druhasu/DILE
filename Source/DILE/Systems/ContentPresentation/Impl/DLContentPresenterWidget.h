// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Components/Widget.h"
#include "DI/InjectOnConstruction.h"
#include "Systems/ContentPresentation/IDLContentPresenter.h"

#include "DLContentPresenterWidget.generated.h"

class IInjector;
class IDLViewFactory;
class SBox;

/*
 * Allows displaying arbitrary Content via configured set of View classes.
 *
 * Content may be represented by one of the following:
 *   - UUserWidget. Will be shown as is.
 *   - UClass* of UUserWidget. New widget will be created.
 *   - UObject*. New widget will be created based on ClassViews property.
 *   - Struct. New widget will be created based on StructViews property.
 */
UCLASS()
class UDLContentPresenterWidget : public UWidget, public IDLContentPresenter, public FInjectOnConstruction
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IDLViewFactory> InViewFactory);

    // Begin UWidget overrides
    void SynchronizeProperties() override;
    void ReleaseSlateResources(bool bReleaseChildren) override;
    //~End UWidget overrides

    /* Clears all Content */
    void ClearContent();

    /* Currently created widget for displaying Content */
    UFUNCTION(BlueprintPure)
    UUserWidget* GetContentWidget() const { return CurrentViewWidget; }

protected:
    TSharedRef<SWidget> RebuildWidget() override;

    // Begin IDLContentPresenter implementation
    void K2_SetContent_Implementation(const FDLViewContent& Content) override;
    //~End IDLContentPresenter implementation

    void SetObjectContent(UObject* Content);
    void SetStructContent(FConstStructView Content);

    UClass* FindViewClass(const FDLViewContent& Content) const;

    /* The padding area between the slot and the Content it contains. */
    UPROPERTY(EditAnywhere, Category = "Settings")
    FMargin ContentPadding;

    /* The alignment of Content horizontally. */
    UPROPERTY(EditAnywhere, Category = "Settings")
    TEnumAsByte<EHorizontalAlignment> ContentHorizontalAlignment;

    /* The alignment of Content vertically. */
    UPROPERTY(EditAnywhere, Category = "Settings")
    TEnumAsByte<EVerticalAlignment> ContentVerticalAlignment;

    /* Containt associations between Content class and View widget used to present it */
    UPROPERTY(EditAnywhere, Category = "Settings")
    TMap<UClass*, TSubclassOf<UUserWidget>> ClassViews;

    /* Containt associations between Content struct and View widget used to present it */
    UPROPERTY(EditAnywhere, Category = "Settings")
    TMap<UScriptStruct*, TSubclassOf<UUserWidget>> StructViews;

private:
    void SetViewWidget(UUserWidget* Widget);

    UPROPERTY()
    TScriptInterface<IDLViewFactory> ViewFactory;

    UPROPERTY()
    FDLViewContent CurrentContent;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentViewWidget;

    TSharedPtr<SBox> MyBox;
};
