
#pragma once
#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <../ThirdParty/UEAdapterSimpleBLE/include/simpleble/SimpleBLE.h>
THIRD_PARTY_INCLUDES_END

#include "BLEManager.generated.h"

UDELEGATE(BlueprintCallable, Category = "BLE")
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FonSpeedChange, float, new_speed);

UDELEGATE(BlueprintCallable, Category = "BLE")
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScanFoundDevice);

UDELEGATE(BlueprintCallable, Category = "BLE")
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeviceConnect);

UDELEGATE(BlueprintCallable, Category = "BLE")
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyRead, TArray<uint8>, data );

UCLASS(BlueprintType, Blueprintable)
class UEADAPTERSIMPLEBLE_API UBLEManager: public UObject{
    GENERATED_BODY()
    public:

        UBLEManager();
		UFUNCTION(BlueprintCallable, Category = "BLE")
		void isBluetoothEnabled( bool& result );

        //void BeginDestroy override;

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void isScanning( bool& result, bool& result_valid );

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void collectAdapter();

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void connectRoutine();

		UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
		bool bluetoothEnabled;

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void BLERoutine();

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void StartScan();

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void StopScan();

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void DeviceDisconnect();

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void ConnectCurrentDevice();

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void GetDeviceServices( TArray<FString>& ServicesUUIDs );

        UFUNCTION(BlueprintCallable, Category = "BLE")
		void Subscribe( const FString ServiceUUID, const FString CharacteristicUUID);

		UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
		int state;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
		TArray<FString> UUIDs;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        FString device_name;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        FString state_name;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        TArray<FString> services_array;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        TArray<FString> characteristics_array;

        UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "BLE" )
        TArray<FString> characteristics_data;

        SimpleBLE::Adapter adapter;
        // // SimpleBLE::Peripheral device_object;
        // //std::shared_ptr< SimpleBLE::Safe::Peripheral > device;
        SimpleBLE::Peripheral device;

        //Dispatched
        UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "BLE" )
        FonSpeedChange onSpeedChange;

        UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "BLE" )
        FOnScanFoundDevice onScanFoundDevice;
        
        UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "BLE" )
        FOnDeviceConnect onDeviceConnect;

        UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "BLE" )
        FOnNotifyRead onNotifyRead;
};