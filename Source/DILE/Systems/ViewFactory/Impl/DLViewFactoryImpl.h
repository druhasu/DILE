// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ViewFactory/IDLViewFactory.h"

#include "DLViewFactoryImpl.generated.h"

class IInjector;
class IDLPlayerControllerService;
class IDLViewModelFactory;

UCLASS()
class UDLViewFactoryImpl : public UObject, public IDLViewFactory
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IInjector> InInjector,
        TScriptInterface<IDLPlayerControllerService> InPlayerControllerService,
        TScriptInterface<IDLViewModelFactory> InViewModelFactory);

protected:
    UObject* CreateView(UClass* ViewClass, const FDLViewContent& Content) override;

private:
    template <typename TView>
    void TryInitViewModel(TView* View, UObject* ContentKey, const FDLViewContent& Content);

    UPROPERTY()
    TScriptInterface<IInjector> Injector;

    UPROPERTY()
    TScriptInterface<IDLPlayerControllerService> PlayerControllerService;

    UPROPERTY()
    TScriptInterface<IDLViewModelFactory> ViewModelFactory;
};
