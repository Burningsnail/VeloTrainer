#include "AsyncBLE.h"

void on_device_found(simpleble_adapter_t adapter, simpleble_peripheral_t peripheral, void* userdata){
    if(userdata == nullptr){
        return;
    }
    ((FAsyncBLE*)userdata)->state_name = FString("Device found");
    TSharedPtr<simpleble_peripheral_t, ESPMode::ThreadSafe> new_peripheral = MakeShared<simpleble_peripheral_t, ESPMode::ThreadSafe>(peripheral);
    //((FAsyncBLE*)userdata)->devices.Add( new_peripheral );
    ((FAsyncBLE*)userdata)->devices_ptrs.Add( peripheral );
};

void on_scan_stopped(simpleble_adapter_t adapter, void* userdata){
    if(userdata == nullptr){
        return;
    }
    ((FAsyncBLE*)userdata)->state_name = FString("Stop scanning");
    ((FAsyncBLE*)userdata)->bStateInitialized = false;
    ((FAsyncBLE*)userdata)->state = stateBLE::IDLE;
    ((FAsyncBLE*)userdata)->bOutputReady = true;

}

void on_notify( simpleble_uuid_t service, simpleble_uuid_t characteristic,
                                             const uint8_t* data, size_t data_length, void* userdata ){
    //TMap<FString, TArray<uint8> > CharacteristicData;                                                
    ((TArray<uint8>*)userdata)->Empty();
    for( unsigned int i =0; i < data_length; i++ ){
        ((TArray<uint8>*)userdata)->Add(data[i]);
    }
    
}
// void on_connected( simpleble_peripheral_t peripheral, void* userdata ){
//     ((FAsyncBLE*) userdata)->bStateInitialized = false;
//     ((FAsyncBLE*) userdata)->state = stateBLE::IDLE;
//     ((FAsyncBLE*) userdata)->bOutputReady = true;     
// }
FAsyncBLE::FAsyncBLE(){
        bRunBLE = true;
        UE_LOG(LogTemp, Warning, TEXT("new thread"));
        Thread = FRunnableThread::Create(this, TEXT("AsyncBLE Thread"));
}
FAsyncBLE::~FAsyncBLE(){
    // this->bRunBLE = false;
    if (Thread != nullptr)
    {
        Thread->Kill(true);
        delete Thread;
    }

}
uint32_t FAsyncBLE::Run(){

    //std::vector< SimpleBLE::Peripheral > devices;
    //std::uintptr_t ptr_i;
    
    HasStopped.AtomicSet( false );

    while( this->bRunBLE ){
        
        BLETick();
        // while( !bInputReady ){
        //     FPlatformProcess::Sleep(1.0f);
        //     continue;
        // }

        // //do state only once and then wait
        // if( bStateInitialized){
        //     FPlatformProcess::Sleep(1.0f);
        //     continue;
        // }
        // size_t              count_found_devices = 0;
        // simpleble_err_t     err;
        // simpleble_uuid_t    service;
        // simpleble_uuid_t    characteristic;
        // TArray< uint8 > dataContainer;

        // switch (state)
        // {
        // case stateBLE::IDLE:
        //     /* code */
        //     bStateInitialized = true;
            
        //     break;
        // case stateBLE::CHECKING_BLUETOOTH:
        //     //state_name = FString("BlueTooth checking");
        //     bOutputReady = false;
        //     //check BlueTooth      
        //     if( !bBlueToothEnabled ){
        //         //state_name = FString("BlueTooth is disabled");

        //         bBlueToothEnabled = simpleble_adapter_is_bluetooth_enabled();
                
        //         if(bBlueToothEnabled){
        //             //next step
        //             state_name = FString("BlueTooth enabled");
        //             bStateInitialized = false;
        //             state = stateBLE::IDLE;
        //             bOutputReady = true;

        //         }else{
        //             state_name = FString("BlueTooth is disabled");
        //             bStateInitialized = false;
        //             state = stateBLE::IDLE;
        //             bOutputReady = true;
        //         }
        //         //wait bluetooth check
        //         continue;
        //     }else{
        //         state_name = FString("BlueTooth enabled");
        //         bStateInitialized = false;
        //         state = stateBLE::IDLE;
        //         bOutputReady = true;
        //     }

        //     break;
        // case stateBLE::COLLECTING_ADAPTERS:
        //     bOutputReady = false;
        //     bStateInitialized = true;
        //     UE_LOG(LogTemp, Warning, TEXT("pre IF"));
        //     if(adapter_ptr == nullptr){
                
        //         UE_LOG(LogTemp, Warning, TEXT("get Adapters"));
        //         size_t adapter_count = simpleble_adapter_get_count();
                
        //         UE_LOG(LogTemp, Warning, TEXT("if Empty"));
        //         if( !adapter_count ){
        //             state_name = FString("no BLE adapters found");
        //             bStateInitialized = false;
        //             state = stateBLE::IDLE;
        //             bOutputReady = true;
        //             //state_name = FString("Adapter list empty");
        //             continue;
        //         }

        //         //state_name = FString("Adapter list");

        //         UE_LOG(LogTemp, Warning, TEXT("new Adapter at 0"));
        //         // if( adapter_list.empty() ){
        //         //     UE_LOG(LogTemp, Warning, TEXT("adapter_list is 0"));
        //         // }
        //         adapter_ptr = simpleble_adapter_get_handle(0);
        //         //adapter = MakeShared<simpleble_adapter_t, ESPMode::ThreadSafe>(adapter_ptr);
        //         BLEAdapterIdentifier = FString(simpleble_adapter_identifier(adapter_ptr));    
        //     }
        //     UE_LOG(LogTemp, Warning, TEXT("if Adapter is null"));
        //     // FPlatformProcess::Sleep(3.0f);

        //     if( adapter_ptr != nullptr ){
        //         //adapter was found
        //         state_name = FString("BLE adapter found");
        //         bStateInitialized = false;
        //         state = stateBLE::IDLE;
        //         bOutputReady = true;

        //     }else{
        //         //adpater pointer is null - no adapters are found
        //         state_name = FString("BLE adapter not found");
        //         bStateInitialized = false;
        //         state = stateBLE::IDLE;
        //         bOutputReady = true;
        //     }
        //     //next step

        //     break;

        // case stateBLE::SCANNING:
        //     bOutputReady = false;
        //     bStateInitialized = true;
            
        //     //scan
        //     //Manager->startScan();
        //     UE_LOG(LogTemp, Warning, TEXT("set Callback for scan"));
        //     //this->devices.Empty();
        //     this->devices_ptrs.Empty();
            
        //     state_name = FString("Scan");

        //     if( adapter_ptr == nullptr ){
        //         state_name = FString("Scan failed invalid");
        //         bStateInitialized = false;
        //         state = stateBLE::IDLE;
        //         bOutputReady = true;
        //         break;
        //     }
        //     err = simpleble_adapter_set_callback_on_scan_found(     adapter_ptr, 
        //                                                             on_device_found, 
        //                                                             this
        //     );
        //     if( err ){
        //         state_name = FString("scan found set callback failure");
        //     }
        //     err = simpleble_adapter_set_callback_on_scan_stop(      adapter_ptr,
        //                                                             on_scan_stopped,
        //                                                             this
        //     );
        //     if( err ){
        //         state_name = FString("scan stop set callback failure");
        //     }
        //     err = simpleble_adapter_scan_for(adapter_ptr, ScanDuration);

        //     break;


        // case stateBLE::CONNECTING:
        //     bStateInitialized = true;

        //     this->state_name = FString("Connecting to deive");
        //     // simpleble_peripheral_set_callback_on_connected(
        //     //     this->device_ptr,
        //     //     on_connected, 
        //     //     this
        //     // );
        //     if( this->device_ptr == nullptr ){
        //         state_name = FString("Device pointer failure");
        //         bStateInitialized = false;
        //         state = stateBLE::IDLE;
        //         bOutputReady = true;
        //         break;
        //     }
        //     err = simpleble_peripheral_connect( this->device_ptr );
        //     // error
        //     if( err ){
        //         state_name = FString("Connect failure");
        //     }
        //     bStateInitialized = false;
        //     state = stateBLE::IDLE;
        //     bOutputReady = true;
        //     // this->state_name = FString("Connected");        
            
        //     break;


        // case stateBLE::READING:
        //     bStateInitialized = true;
            
        //     CharacteristicBytes.Add(CharacteristicUUID, dataContainer);
        //     memcpy(service.value,           StringCast<ANSICHAR>(*ServiceUUID).Get(),           SIMPLEBLE_UUID_STR_LEN);
        //     memcpy(characteristic.value,    StringCast<ANSICHAR>(*CharacteristicUUID).Get(),    SIMPLEBLE_UUID_STR_LEN);

        //     simpleble_peripheral_notify(device_ptr, 
        //                                 service,  
        //                                 characteristic,
        //                                 on_notify,
        //                                 (void*) CharacteristicBytes.Find( CharacteristicUUID ));
            
        //     bStateInitialized = false;
        //     state = stateBLE::IDLE;
        //     bOutputReady = true;  

        // default:
        //     FPlatformProcess::Sleep(1.0f);
        //     continue;
        // }
        
    }

    if( device_ptr != nullptr ){
        //delete device_ptr;
        simpleble_peripheral_release_handle( device_ptr );
    }
    // if( devices_ptrs.IsValid() ){
    //     for( int i = 0; i < devices_ptrs.Num(); i++ ){
    //         if(  devices_ptrs[i] != nullptr ){
    //             simpleble_peripheral_release_handle( devices_ptrs[i] );
    //         }
    //         //delete devices_ptrs[i];

    //     }
    // }
    if( adapter_ptr != nullptr ){
        simpleble_adapter_release_handle( adapter_ptr );
    }
    //delete adapter_ptr;

    HasStopped.AtomicSet( true );
    return 0;

    // FPlatformProcess::Sleep(1.0f);
    // return 0;

}
void FAsyncBLE::Exit(){

    bRunBLE = false;
    // if( adapter ){
    //     //delete adapter;
    // }
    // if( device ){
    //     //delete device;
    // }
}

void FAsyncBLE::BLETick(){
    while( !bInputReady ){
        FPlatformProcess::Sleep(1.0f);
        return;
    }

    //do state only once and then wait
    if( bStateInitialized){
        FPlatformProcess::Sleep(1.0f);
        return;
    }
    size_t              count_found_devices = 0;
    simpleble_err_t     err;
    simpleble_uuid_t    service;
    simpleble_uuid_t    characteristic;
    TArray< uint8 >     dataContainer;

    switch (state)
    {
    case stateBLE::IDLE:
        /* code */
        bStateInitialized = true;
        
        break;
    case stateBLE::CHECKING_BLUETOOTH:
        //state_name = FString("BlueTooth checking");
        bOutputReady = false;
        //check BlueTooth      
        if( !bBlueToothEnabled ){
            //state_name = FString("BlueTooth is disabled");

            bBlueToothEnabled = simpleble_adapter_is_bluetooth_enabled();
            
            if(bBlueToothEnabled){
                //next step
                state_name = FString("BlueTooth enabled");
                bStateInitialized = false;
                state = stateBLE::IDLE;
                bOutputReady = true;

            }else{
                state_name = FString("BlueTooth is disabled");
                bStateInitialized = false;
                state = stateBLE::IDLE;
                bOutputReady = true;
            }
            //wait bluetooth check
            return;
        }else{
            state_name = FString("BlueTooth enabled");
            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;
        }

        break;
    case stateBLE::COLLECTING_ADAPTERS:
        bOutputReady = false;
        bStateInitialized = true;
        UE_LOG(LogTemp, Warning, TEXT("pre IF"));
        if(adapter_ptr == nullptr){
            
            UE_LOG(LogTemp, Warning, TEXT("get Adapters"));
            size_t adapter_count = simpleble_adapter_get_count();
            
            UE_LOG(LogTemp, Warning, TEXT("if Empty"));
            if( !adapter_count ){
                state_name = FString("no BLE adapters found");
                bStateInitialized = false;
                state = stateBLE::IDLE;
                bOutputReady = true;
                //state_name = FString("Adapter list empty");
                return;
            }

            //state_name = FString("Adapter list");

            UE_LOG(LogTemp, Warning, TEXT("new Adapter at 0"));
            // if( adapter_list.empty() ){
            //     UE_LOG(LogTemp, Warning, TEXT("adapter_list is 0"));
            // }
            adapter_ptr = simpleble_adapter_get_handle(0);
            //adapter = MakeShared<simpleble_adapter_t, ESPMode::ThreadSafe>(adapter_ptr);
            BLEAdapterIdentifier = FString(simpleble_adapter_identifier(adapter_ptr));    
        }
        UE_LOG(LogTemp, Warning, TEXT("if Adapter is null"));
        // FPlatformProcess::Sleep(3.0f);

        if( adapter_ptr != nullptr ){
            //adapter was found
            state_name = FString("BLE adapter found");
            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;

        }else{
            //adpater pointer is null - no adapters are found
            state_name = FString("BLE adapter not found");
            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;
        }
        //next step

        break;

    case stateBLE::SCANNING:
        bOutputReady = false;
        bStateInitialized = true;
        
        //scan
        //Manager->startScan();
        UE_LOG(LogTemp, Warning, TEXT("set Callback for scan"));
        //this->devices.Empty();
        this->devices_ptrs.Empty();
        
        state_name = FString("Scan");

        if( adapter_ptr == nullptr ){
            state_name = FString("Scan failed invalid");
            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;
            break;
        }
        err = simpleble_adapter_set_callback_on_scan_found(     adapter_ptr, 
                                                                on_device_found, 
                                                                this
        );
        if( err ){
            state_name = FString("scan found set callback failure");
        }
        err = simpleble_adapter_set_callback_on_scan_stop(      adapter_ptr,
                                                                on_scan_stopped,
                                                                this
        );
        if( err ){
            state_name = FString("scan stop set callback failure");
        }
        err = simpleble_adapter_scan_for(adapter_ptr, ScanDuration);

        break;


    case stateBLE::CONNECTING:
        bStateInitialized = true;

        this->state_name = FString("Connecting to deive");
        // simpleble_peripheral_set_callback_on_connected(
        //     this->device_ptr,
        //     on_connected, 
        //     this
        // );
        if( this->device_ptr == nullptr ){
            state_name = FString("Device pointer failure");
            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;
            break;
        }
        err = simpleble_peripheral_connect( this->device_ptr );
        // error
        if( err ){
            state_name = FString("Connect failure");
        }
        bStateInitialized = false;
        state = stateBLE::IDLE;
        bOutputReady = true;
        // this->state_name = FString("Connected");        
        
        break;


    case stateBLE::READING:
        bStateInitialized = true;
        
        CharacteristicBytes.Add(CharacteristicUUID, dataContainer);
        memcpy(service.value,           StringCast<ANSICHAR>(*ServiceUUID).Get(),           SIMPLEBLE_UUID_STR_LEN);
        memcpy(characteristic.value,    StringCast<ANSICHAR>(*CharacteristicUUID).Get(),    SIMPLEBLE_UUID_STR_LEN);

        simpleble_peripheral_notify(device_ptr, 
                                    service,  
                                    characteristic,
                                    on_notify,
                                    (void*) CharacteristicBytes.Find( CharacteristicUUID ));
        
        bStateInitialized = false;
        state = stateBLE::IDLE;
        bOutputReady = true;  

    default:
        FPlatformProcess::Sleep(1.0f);
        return;
    }
}