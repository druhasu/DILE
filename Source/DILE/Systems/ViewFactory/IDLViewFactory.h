// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLViewFactory.generated.h"

struct FDLViewContent;

UINTERFACE(MinimalApi)
class UDLViewFactory : public UInterface { GENERATED_BODY() };

/*
 Creates View with given class and performs ViewModel initialization for it
 */
class DILE_API IDLViewFactory
{
    GENERATED_BODY()

public:
    template <typename T>
    T* CreateView(TSubclassOf<T> ViewClass, const FDLViewContent& Content)
    {
        return (T*)CreateView(*ViewClass, Content);
    }

protected:
    virtual UObject* CreateView(UClass* ViewClass, const FDLViewContent& Content) = 0;
};
