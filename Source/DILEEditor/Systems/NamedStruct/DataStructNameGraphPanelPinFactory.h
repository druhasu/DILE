// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class SGraphPin;
class UEdGraphPin;

class FDataStructNameGraphPanelPinFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InPin) const override;
};
