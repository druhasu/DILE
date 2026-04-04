// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Backends/MoverNetworkPredictionLiaison.h"

#include "DLMoverNetworkPredictionLiaison.generated.h"

UCLASS()
class UDLMoverNetworkPredictionLiaisonComponent : public UMoverNetworkPredictionLiaisonComponent
{
    GENERATED_BODY()

public:
    void InitializeComponent() override;
};
