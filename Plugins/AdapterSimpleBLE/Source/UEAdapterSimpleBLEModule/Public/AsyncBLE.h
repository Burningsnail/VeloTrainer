
#pragma once
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "BLEDevice.h"
THIRD_PARTY_INCLUDES_START
#include "UEAdapterSimpleBLE/include/simpleble_c/simpleble.h"
THIRD_PARTY_INCLUDES_END
/*
class to handle all BLE actions in a state machine
*/
class UBLEDevice;

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
    //~FAsyncBLE();
    uint32_t Run() override;
    virtual bool Init() override{
        return true;
    }
    virtual void Exit() override;

    virtual void Stop() override{

    }
    FRunnableThread*    Thread;
    // UBLEManager*        Manager;
    // TSharedPtr< simpleble_adapter_t, ESPMode::ThreadSafe > adapter = NULL;
    simpleble_adapter_t adapter_ptr = nullptr;
    // TSharedPtr< simpleble_peripheral_t, ESPMode::ThreadSafe > device = NULL;
    simpleble_peripheral_t device_ptr = nullptr;
    // TArray< TSharedPtr< simpleble_peripheral_t, ESPMode::ThreadSafe > > devices;
    TArray< simpleble_peripheral_t > devices_ptrs; //needed for collecting found devices
    FString state_name;
    bool bRunBLE = true;
    bool bBlueToothEnabled = false;
    bool bInputReady = false;
    bool bOutputReady = false;
    int ScanDuration = 500;
    stateBLE state = stateBLE::IDLE;
    bool bStateInitialized = false;
    FString ServiceUUID;            //target service
    FString CharacteristicUUID;     //target characteristic
    FString BLEAdapterIdentifier;
    TArray< uint8 > Bytes;          //received bytes
    TMap<FString, TArray<uint8> > CharacteristicBytes;
    //std::vector<SimpleBLE::Peripheral> devices;
};
