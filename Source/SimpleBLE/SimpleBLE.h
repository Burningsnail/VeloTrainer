#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

class FSimpleBLE : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};