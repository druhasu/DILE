// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "DLViewContent.h"

#include "IDLContentPresenter.generated.h"

UINTERFACE(MinimalApi)
class UDLContentPresenter : public UInterface { GENERATED_BODY() };

/*
  Interface to an Object that can display arbitrary Content.

  Implement this interface on ViewModel or Widget that may receive additional contextual data.
 */
class DILE_API IDLContentPresenter
{
    GENERATED_BODY()

public:
    static void SetContent(UObject* Presenter, const FDLViewContent& Content)
    {
        if (ensure(Presenter != nullptr) && Presenter->Implements<UDLContentPresenter>())
        {
            IDLContentPresenter::Execute_K2_SetContent(Presenter, Content);
        }
    }

protected:
    /* Requests to display UStruct content. Returns whether content was actually displayed */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Set Content as Struct"))
    void K2_SetContent(const FDLViewContent& Content);

    /* Override this method in C++ class */
    virtual void K2_SetContent_Implementation(const FDLViewContent& Content) {}
};
