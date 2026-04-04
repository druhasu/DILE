// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DI/IInjectorProvider.h"

#include "DLInjectorProvider.generated.h"

class UWorld;
class APlayerController;
class APlayerState;
class AAIController;
class AActor;
class AActorComponent;
class UUserWidget;
class UWidget;

UCLASS()
class UDLInjectorProvider : public UObject, public IInjectorProvider
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IInjector>&& InInjector);

    // Begin IInjectorProvider
    virtual TScriptInterface<IInjector> GetInjector(UObject* InjectTarget) const override;
    // End IInjectorProvider

private:
    using FGetterFunction = UObject* (*)(UObject* Obj);

    struct FGetter
    {
        UClass* ObjectClass;
        FGetterFunction Function;
    };

private:
    static UObject* GetForWorld(UWorld* World);
    static UObject* GetForPlayerController(APlayerController* Controller);
    static UObject* GetForPlayerState(APlayerState* PlayerState);
    static UObject* GetForAIController(AAIController* Controller);
    static UObject* GetForActor(AActor* Actor);
    static UObject* GetForActorComponent(UActorComponent* ActorComponent);
    static UObject* GetForUserWidget(UUserWidget* UserWidget);
    static UObject* GetForWidget(UWidget* Widget);
    static UObject* GetForUObject(UObject* Object);

    template<typename TClass>
    void AddGetter(UObject* (*Function)(TClass*));

    FGetterFunction FindGetterFunction(UClass* Class) const;

private:
    UPROPERTY()
    TScriptInterface<IInjector> DefaultInjector;

    TArray<FGetter, TFixedAllocator<9>> Getters;
};
