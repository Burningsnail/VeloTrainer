#pragma once
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "AsyncBLE.h"
THIRD_PARTY_INCLUDES_START
#include "UEAdapterSimpleBLE/include/simpleble_c/simpleble.h"
THIRD_PARTY_INCLUDES_END
#include "BLEDevice.generated.h"

class FAsyncBLE;

UCLASS(BlueprintType, Blueprintable)
class UEADAPTERSIMPLEBLEMODULE_API UBLEDevice: public UObject{
    GENERATED_BODY()
    public:
        UBLEDevice();

        UFUNCTION(BlueprintCallable, Category = "BLE")
        void CheckDevicePointer(bool& pointerIsOk);

        UFUNCTION(BlueprintCallable, Category = "BLE")
        void UpdateDeviceInfo();

        UFUNCTION(BlueprintCallable, Category = "BLE")
        void UpdateServices();

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        FString name;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        FString address;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        bool connectable;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        bool paired;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        bool isConnected;
        
        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        TArray<FString> Services;

        //void Init( TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe>  asyncManager , TSharedPtr<SimpleBLE::Peripheral, ESPMode::ThreadSafe> peripheral );

        TSharedPtr<simpleble_peripheral_t, ESPMode::ThreadSafe>   Device;
        simpleble_peripheral_t Device_ptr;
        //TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe>               AsyncBLE;
};