#pragma once
#include "CoreMinimal.h"

#include "AsyncBLE.h"
#include "Templates/SharedPointer.h"
#include "BLEAdapter.generated.h"

// UDELEGATE(BlueprintCallable, Category = "BLE")
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FonSpeedChange, float, new_speed);

// UDELEGATE(BlueprintCallable, Category = "BLE")
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScanFoundDevice);

// UDELEGATE(BlueprintCallable, Category = "BLE")
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeviceConnect);

// UDELEGATE(BlueprintCallable, Category = "BLE")
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyRead, TArray<uint8>, data );

UCLASS(BlueprintType, Blueprintable)
class UEADAPTERSIMPLEBLEMODULE_API UBLEAdapter: public UObject{
    GENERATED_BODY()
    public:
        UBLEAdapter();
        
        UFUNCTION(BlueprintCallable, Category = "BLE")
		void InitRoutine();

        // UFUNCTION(BlueprintCallable, Category = "BLE")
		// void CheckBlueTooth( bool& succes );

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void PerformBLEProcedure( FString procedure, bool& success);

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void UpdateState();
        
        UFUNCTION(BlueprintCallable, Category = "BLE")
		void IsAdapterInitialized(bool& initialized);

        UFUNCTION(BlueprintCallable, Category = "BLE")
        void CheckAdapterPointer(bool& pointerIsOk);

        UFUNCTION(BlueprintCallable, Category = "BLE")
        void SelectDevice( int index );

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        FString state_name;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        bool outputReady;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        int state;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        UBLEDevice* device;
        
        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        int devicesCount;

		TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe> AsyncBLE;
};