// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLMoverNetworkPredictionLiaison.h"

void UDLMoverNetworkPredictionLiaisonComponent::InitializeComponent()
{
    // MoverComponent call InitializeComponent twice triggering the check inside UActorComponent::InitializeComponent
    // so we additionally check here to prevent that
    if (!HasBeenInitialized())
    {
        Super::InitializeComponent();
    }
}
