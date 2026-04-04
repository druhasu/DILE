// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLViewModelFactoryImpl.h"

#include "DI/IInjector.h"
#include "Mvvm/BaseViewModel.h"

void UDLViewModelFactoryImpl::InitDependencies(TScriptInterface<IInjector> InInjector)
{
    Injector = InInjector;

    FCoreUObjectDelegates::GetPostGarbageCollect().AddUObject(this, &ThisClass::OnGarbageCollected);
}

TTuple<UBaseViewModel*, bool> UDLViewModelFactoryImpl::CreateViewModel(TSubclassOf<UBaseViewModel> ViewModelClass, UObject* Key)
{
    if (!ensure(ViewModelClass != nullptr))
    {
        return { nullptr, false };
    }

    UBaseViewModel* Result = nullptr;
    bool bCreated = false;

    TWeakObjectPtr<UBaseViewModel>* FoundPtr = ViewModels.Find(Key);
    if (FoundPtr != nullptr)
    {
        Result = FoundPtr->Get();
    }

    if (Result == nullptr)
    {
        Result = NewObject<UBaseViewModel>(GetOuter(), ViewModelClass);
        bCreated = true;

        Injector->Inject(Result);

        if (Key != nullptr)
        {
            // don't track ViewModels with nullptr Key
            ViewModels.Emplace(Key, Result);
        }
    }

    return { Result, bCreated };
}

void UDLViewModelFactoryImpl::OnGarbageCollected()
{
    // remove all entries where either Key or Value is no longer valid
    for (auto It = ViewModels.CreateIterator(); It; ++It)
    {
        if (!It.Key().IsValid() || !It.Value().IsValid())
        {
            It.RemoveCurrent();
        }
    }
}
