// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ViewFactory/IDLViewFactory.h"
#include "Utils/DLFunctionMock.h"

#include "DLViewFactoryMock.generated.h"

UCLASS()
class UDLViewFactoryMock : public UObject, public IDLViewFactory
{
    GENERATED_BODY()

public:
    UObject* CreateView(UClass* ViewClass, const FDLViewContent& Content) override
    {
        DL_MOCK_FUNCTION_BODY(nullptr, CreateView, ViewClass, Content);
    }

    DL_MOCK_FUNCTION_VARIABLE(CreateView);
};
