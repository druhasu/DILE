// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Mvvm/BaseViewModel.h"

/*
  Helper struct that counts how many times each property changed.
  It can also be used as a placeholder View to trigger SubscriptionStatusChanged on a ViewModel
 */
struct FPropertyChangeCounter
{
    FPropertyChangeCounter(UBaseViewModel* InViewModel)
    {
        ViewModel = InViewModel;
        ViewModel->Subscribe(UBaseViewModel::FPropertyChangedDelegate::FDelegate::CreateRaw(this, &FPropertyChangeCounter::OnPropertyChanged));
    }

    ~FPropertyChangeCounter()
    {
        if (ViewModel.IsValid())
        {
            ViewModel->Unsubscribe(this);
        }
    }

    int32 operator[](const FViewModelPropertyBase* Prop) const
    {
        return Changes.FindRef(Prop);
    }

    void OnPropertyChanged(const FViewModelPropertyBase* Prop)
    {
        Changes.FindOrAdd(Prop) += 1;
    }

    TMap<const FViewModelPropertyBase*, int32> Changes;
    TWeakObjectPtr<UBaseViewModel> ViewModel;
};
