// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class SGraphPin;
class UEdGraphPin;

class FDLDataStructNameGraphPanelPinFactory : public FGraphPanelPinFactory
{
    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InPin) const override;
};
