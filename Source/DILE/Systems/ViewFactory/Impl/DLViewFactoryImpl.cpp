// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLViewFactoryImpl.h"
#include "Systems/ContentPresentation/IDLContentPresenter.h"
#include "Systems/PlayerContext/IDLPlayerControllerService.h"
#include "Systems/ViewModelFactory/IDLViewModelFactory.h"
#include "Utils/DLEnsure.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "DI/IInjector.h"
#include "Mvvm/BaseViewModel.h"
#include "Mvvm/MvvmBlueprintLibrary.h"

void UDLViewFactoryImpl::InitDependencies(
    TScriptInterface<IInjector> InInjector,
    TScriptInterface<IDLPlayerControllerService> InPlayerControllerService,
    TScriptInterface<IDLViewModelFactory> InViewModelFactory)
{
    Injector = InInjector;
    PlayerControllerService = InPlayerControllerService;
    ViewModelFactory = InViewModelFactory;
}

UObject* UDLViewFactoryImpl::CreateView(UClass* ViewClass, const FDLViewContent& Content)
{
    DL_ENSURE_RETURN(ViewClass != nullptr, nullptr);

    UObject* ContentKey = Content.HasObject() ? Content.GetObject() : nullptr;

    // depending on the Class of a View, we need to create it differently
    if (ViewClass->IsChildOf<UUserWidget>())
    {
        UUserWidget* ViewWidget = CreateWidget<UUserWidget>(PlayerControllerService->GetPlayerController(), ViewClass);
        TryInitViewModel(ViewWidget, ContentKey, Content);
        return ViewWidget;
    }
    else if (ViewClass->IsChildOf<AActor>())
    {
        AActor* ViewActor = GetWorld()->SpawnActor(ViewClass);
        TryInitViewModel(ViewActor, ContentKey, Content);
        return ViewActor;
    }
    else
    {
        ensureMsgf(!"Unsupported View class", TEXT("Requested ViewClass: %s"), *ViewClass->GetName());
    }

    return nullptr;
}

template <typename TView>
void UDLViewFactoryImpl::TryInitViewModel(TView* View, UObject* ContentKey, const FDLViewContent& Content)
{
    // Inject dependencies before checking anything else
    Injector->Inject(View);

    TSubclassOf<UBaseViewModel> ViewModelClass = UMvvmBlueprintLibrary::GetViewModelClass(View->GetClass());

    // check whether View has any associated ViewModel class
    if (ViewModelClass != nullptr)
    {
        // check whether View already has ViewModel set - provided by call to Inject above
        // if not - we need to create ViewModel object and set it to the View
        if (UMvvmBlueprintLibrary::GetViewModel(View) == nullptr)
        {
            // create or reuse the ViewModel
            auto [ViewModel, bNew] = ViewModelFactory->CreateViewModel(ViewModelClass, ContentKey);
            if (bNew)
            {
                // if new ViewModel was created (not reused) - set Content to it
                IDLContentPresenter::SetContent(ViewModel, Content);
            }

            // when ViewModel is fully initialized, set it to the View
            UMvvmBlueprintLibrary::SetViewModel(View, ViewModel);
        }
    }
    else
    {
        // View does not need a ViewModel, try setting Content directly to it
        IDLContentPresenter::SetContent(View, Content);
    }
}
