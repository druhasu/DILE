// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLViewModelFactory.generated.h"

class UBaseViewModel;

UINTERFACE(MinimalApi)
class UDLViewModelFactory : public UInterface { GENERATED_BODY() };

/*
 Creates ViewModels for given View and Key object (usually Model class)
 */
class DILE_API IDLViewModelFactory
{
    GENERATED_BODY()

public:
    /* Creates ViewModel for a given View object. Returns pointer to a ViewModel and tells whether new instance was created */
    virtual TTuple<UBaseViewModel*, bool> CreateViewModel(TSubclassOf<UBaseViewModel> ViewModelClass, UObject* Key) = 0;
};
