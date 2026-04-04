// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Modules/ModuleManager.h"

#include "PropertyEditorModule.h"

#include "Systems/NamedStruct/Impl/NamedStructRegistry.h"
#include "Systems/NamedStruct/DLDataStructNamePropertyCustomization.h"
#include "Systems/NamedStruct/DLDataStructNameGraphPanelPinFactory.h"

class FDILEEditorModule : public IModuleInterface
{
public:
    using ThisClass = FDILEEditorModule;

    void StartupModule() override
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        for (auto StructType : FNamedStructRegistry::Get().GetAllNameStructs())
        {
            PropertyModule.RegisterCustomPropertyTypeLayout(StructType->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDLDataStructNamePropertyCustomization::Make, StructType));
            PropertyTypeNames.Add(StructType->GetFName());
        }

        DataStructNameGraphPanelPinFactory = MakeShared<FDLDataStructNameGraphPanelPinFactory>();
        FEdGraphUtilities::RegisterVisualPinFactory(DataStructNameGraphPanelPinFactory);
    }

    void ShutdownModule() override
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        for (const FName& StructName : PropertyTypeNames)
        {
            PropertyModule.UnregisterCustomPropertyTypeLayout(StructName);
        }
        PropertyTypeNames.Reset();

        FEdGraphUtilities::UnregisterVisualPinFactory(DataStructNameGraphPanelPinFactory);
        DataStructNameGraphPanelPinFactory.Reset();
    }

private:
    TArray<FName> PropertyTypeNames;
    TSharedPtr<FDLDataStructNameGraphPanelPinFactory> DataStructNameGraphPanelPinFactory;
};

IMPLEMENT_MODULE( FDILEEditorModule, DILEEditor );
