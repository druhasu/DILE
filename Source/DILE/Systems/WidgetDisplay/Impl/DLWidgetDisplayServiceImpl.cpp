// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWidgetDisplayServiceImpl.h"
#include "Systems/ContentPresentation/DLViewContent.h"
#include "Systems/PlayerContext/IDLPlayerControllerService.h"
#include "Systems/Streamable/IDLStreamableService.h"
#include "Systems/ViewFactory/IDLViewFactory.h"
#include "Systems/WidgetSlots/IDLWidgetSlot.h"
#include "Systems/WidgetSlots/IDLWidgetSlotsService.h"
#include "Utils/Widgets/DLUserWidget.h"
#include "Utils/DLEnsure.h"

#include "GameFramework/PlayerController.h"
#include "Widgets/SViewport.h"

void UDLWidgetDisplayServiceImpl::InitDependencies(
    TScriptInterface<IDLStreamableService> InStreamableService,
    TScriptInterface<IDLViewFactory> InViewFactory,
    TScriptInterface<IDLWidgetSlotsService> InWidgetSlotsService,
    TScriptInterface<IDLPlayerControllerService> InPlayerControllerService
)
{
    StreamableService = InStreamableService;
    ViewFactory = InViewFactory;
    WidgetSlotsService = InWidgetSlotsService;
    PlayerControllerService = InPlayerControllerService;
}

void UDLWidgetDisplayServiceImpl::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
    Super::AddReferencedObjects(InThis, Collector);

    UDLWidgetDisplayServiceImpl* CastedThis = CastChecked<UDLWidgetDisplayServiceImpl>(InThis);
    for (FDLViewContent* Content : CastedThis->ContentToKeep)
    {
        Content->AddStructReferencedObjects(Collector);
    }
}

TDLTask<UDLUserWidget*> UDLWidgetDisplayServiceImpl::DisplayWidgetImpl(FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content, FForceLatentCoroutine)
{
    ContentToKeep.Add(&Content); // required to keep Content from being garbage collected

    TSubclassOf<UDLUserWidget> ViewClass = Cast<UClass>(co_await StreamableService->AsyncLoad(WidgetClass.ToSoftObjectPath()));
    DL_ENSURE_CORETURN(ViewClass != nullptr, nullptr);

    TScriptInterface<IDLWidgetSlot> WidgetSlot = co_await WidgetSlotsService->WaitForSlot(SlotTag);
    UDLUserWidget* Widget = ViewFactory->CreateView(ViewClass, Content);
    ContentToKeep.RemoveSingleSwap(&Content, EAllowShrinking::No);

    WidgetSlot->AddWidget(Widget); // intentionally no co_await here
    if (Widget->NeedsMouseInput())
    {
        Widget->OnNativeDestruct.AddUObject(this, &ThisClass::OnWidgetDestructed);
        if (WidgetsWithMouse.Add(Widget) == 0)
        {
            EnableMouseInput();
        }
    }

    co_return Widget;
}

FDLTask UDLWidgetDisplayServiceImpl::CloseWidgetImpl(FGameplayTag SlotTag, FForceLatentCoroutine)
{
    TScriptInterface<IDLWidgetSlot> WidgetSlot = co_await WidgetSlotsService->WaitForSlot(SlotTag);
    DL_ENSURE_CORETURN(!"Not Implemented properly");
}

void UDLWidgetDisplayServiceImpl::OnWidgetDestructed(UUserWidget* Widget)
{
    Widget->OnNativeDestruct.RemoveAll(this);
    WidgetsWithMouse.RemoveSingleSwap((UDLUserWidget*)Widget);
    if (WidgetsWithMouse.Num() == 0)
    {
        DisableMouseInput();
    }
}

void UDLWidgetDisplayServiceImpl::EnableMouseInput()
{
    APlayerController* PlayerController = PlayerControllerService->GetPlayerController();
    ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

    FInputModeUIOnly InputMode;

    PlayerController->SetInputMode(InputMode);
    PlayerController->SetShowMouseCursor(true);

    LocalPlayer->GetSlateOperations().ReleaseMouseCapture();
}

void UDLWidgetDisplayServiceImpl::DisableMouseInput()
{
    APlayerController* PlayerController = PlayerControllerService->GetPlayerController();
    ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

    FInputModeGameOnly InputMode;

    PlayerController->SetInputMode(InputMode);
    PlayerController->SetShowMouseCursor(false);

    if (UGameViewportClient* GameViewportClient = LocalPlayer->ViewportClient)
    {
        TSharedPtr<SViewport> ViewportWidget = GameViewportClient->GetGameViewportWidget();
        if (ViewportWidget.IsValid())
        {
            LocalPlayer->GetSlateOperations().UseHighPrecisionMouseMovement(ViewportWidget.ToSharedRef());
        }
    }
}
