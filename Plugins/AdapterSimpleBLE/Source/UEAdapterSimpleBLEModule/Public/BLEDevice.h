#pragma once
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "AsyncBLE.h"
THIRD_PARTY_INCLUDES_START
#include "UEAdapterSimpleBLE/include/simpleble/SimpleBLE.h"
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

        //void Init( TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe>  asyncManager , TSharedPtr<SimpleBLE::Peripheral, ESPMode::ThreadSafe> peripheral );

        TSharedPtr<SimpleBLE::Peripheral, ESPMode::ThreadSafe>   Device;
        //TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe>               AsyncBLE;
};