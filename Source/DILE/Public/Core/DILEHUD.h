// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "DI/InjectOnConstruction.h"
#include "DI/IInjector.h"
#include "DILEHUD.generated.h"

/*
 * Base class for game HUD.
 */
UCLASS()
class ADILEHUD : public AHUD, public FInjectOnConstruction
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IInjector>&& InInjector);

    void BeginPlay() override;

protected:
    /* Class of widget to create from the start of the game */
    UPROPERTY(EditDefaultsOnly)
    TSoftClassPtr<UUserWidget> StartupWidgetClass;
};
