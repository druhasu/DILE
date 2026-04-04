// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"

#include "DLPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/*
 * Player controller Class for all Game modes
 */
UCLASS()
class ADLPlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    /** Constructor */
    ADLPlayerController();

protected:

    /** Input Mapping Contexts */
    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    /** Gameplay initialization */
    virtual void BeginPlay() override;

    /** Input mapping context setup */
    virtual void SetupInputComponent() override;
};
