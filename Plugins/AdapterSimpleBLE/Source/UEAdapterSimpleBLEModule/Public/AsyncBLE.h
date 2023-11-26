
#pragma once
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
THIRD_PARTY_INCLUDES_START
#include "UEAdapterSimpleBLE/include/simpleble/SimpleBLE.h"
THIRD_PARTY_INCLUDES_END

enum stateBLE{
    IDLE = 0,
    CHECKING_BLUETOOTH = 1,
    COLLECTING_ADAPTERS = 2,
    SCANNING = 3,
    CONNECTING = 4,
    READING = 5,
    ERROR = 10

};
class UEADAPTERSIMPLEBLEMODULE_API FAsyncBLE : public FRunnable{

public:

    FAsyncBLE();
    ~FAsyncBLE();
    virtual uint32_t Run() override;
    virtual bool Init() override{
        return true;
    }
    virtual void Exit() override;

    virtual void Stop() override{

    }
    FRunnableThread*    Thread;
    // UBLEManager*        Manager;
    SimpleBLE::Adapter *adapter = nullptr;
    SimpleBLE::Peripheral *device = nullptr;
    FString state_name;
    bool bRunBLE = true;
    bool bBlueToothEnabled = false;
    bool bInputReady = false;
    bool bOutputReady = false;
    int ScanDuration = 500;
    stateBLE state = stateBLE::IDLE;
    bool bStateInitialized = false;
    FString ServiceUUID;
    FString CharacteristicUUID;
    std::vector<SimpleBLE::Peripheral> devices;
};
