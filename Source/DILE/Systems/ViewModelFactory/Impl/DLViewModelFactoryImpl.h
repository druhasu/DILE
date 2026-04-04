// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ViewModelFactory/IDLViewModelFactory.h"

#include "DLViewModelFactoryImpl.generated.h"

class IInjector;

UCLASS()
class DILE_API UDLViewModelFactoryImpl : public UObject, public IDLViewModelFactory
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IInjector> InInjector);

    TTuple<UBaseViewModel*, bool> CreateViewModel(TSubclassOf<UBaseViewModel> ViewModelClass, UObject* Key) override;

private:
    void OnGarbageCollected();

    UPROPERTY()
    TScriptInterface<IInjector> Injector;

    UPROPERTY()
    TMap<TWeakObjectPtr<UObject>, TWeakObjectPtr<UBaseViewModel>> ViewModels;
};
