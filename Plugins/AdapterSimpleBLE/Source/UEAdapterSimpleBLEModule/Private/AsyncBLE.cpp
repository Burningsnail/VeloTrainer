#include "AsyncBLE.h"

void on_device_found(simpleble_adapter_t adapter, simpleble_peripheral_t peripheral, void* userdata){
    ((FAsyncBLE*)userdata)->state_name = FString("Device found");
    TSharedPtr<simpleble_peripheral_t, ESPMode::ThreadSafe> new_peripheral = MakeShared<simpleble_peripheral_t, ESPMode::ThreadSafe>(peripheral);
    ((FAsyncBLE*)userdata)->devices.Add( new_peripheral );
    ((FAsyncBLE*)userdata)->devices_ptrs.Add( peripheral );
};

void on_scan_stopped(simpleble_adapter_t adapter, void* userdata){

    ((FAsyncBLE*)userdata)->state_name = FString("Stop scanning");
    ((FAsyncBLE*)userdata)->bStateInitialized = false;
    ((FAsyncBLE*)userdata)->state = stateBLE::IDLE;
    ((FAsyncBLE*)userdata)->bOutputReady = true;

}

FAsyncBLE::FAsyncBLE(){
        bRunBLE = true;
        UE_LOG(LogTemp, Warning, TEXT("new thread"));
        Thread = FRunnableThread::Create(this, TEXT("AsyncBLE Thread"));
}
// FAsyncBLE::~FAsyncBLE(){
//     this->bRunBLE = false;
// }
uint32_t FAsyncBLE::Run(){

    //std::vector< SimpleBLE::Peripheral > devices;
    //std::uintptr_t ptr_i;
    while( this->bRunBLE ){

        while( !bInputReady ){
            FPlatformProcess::Sleep(1.0f);
            continue;
        }

        //do state only once and then wait
        if( bStateInitialized){
            FPlatformProcess::Sleep(1.0f);
            continue;
        }
        size_t count_found_devices = 0;
        simpleble_err_t err;

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
                continue;
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
            if(adapter == nullptr){
                
                UE_LOG(LogTemp, Warning, TEXT("get Adapters"));
                size_t adapter_count = simpleble_adapter_get_count();
                
                UE_LOG(LogTemp, Warning, TEXT("if Empty"));
                if( !adapter_count ){
                    state_name = FString("no BLE adapters found");
                    bStateInitialized = false;
                    state = stateBLE::IDLE;
                    bOutputReady = true;
                    //state_name = FString("Adapter list empty");
                    continue;
                }

                //state_name = FString("Adapter list");

                UE_LOG(LogTemp, Warning, TEXT("new Adapter at 0"));
                // if( adapter_list.empty() ){
                //     UE_LOG(LogTemp, Warning, TEXT("adapter_list is 0"));
                // }
                adapter_ptr = simpleble_adapter_get_handle(0);
                adapter = MakeShared<simpleble_adapter_t, ESPMode::ThreadSafe>(adapter_ptr);
                BLEAdapterIdentifier = FString(simpleble_adapter_identifier(adapter_ptr));    
            }
            UE_LOG(LogTemp, Warning, TEXT("if Adapter is null"));
            // FPlatformProcess::Sleep(3.0f);

            if( adapter_ptr == nullptr ){
                //ptr_i = reinterpret_cast<std::uintptr_t>(adapter);
                //state_name = FString("Adapter: ") ;//+ FString::FromInt(ptr_i);
                state_name = FString("BLE adapter found");
                bStateInitialized = false;
                state = stateBLE::IDLE;
                bOutputReady = true;

            }else{
                //get once more
                //state_name = FString("Adapter not found");
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
            this->devices.Empty();
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
            // if( err ){
            //     state_name = FString("Scan failed");
            //     bStateInitialized = false;
            //     state = stateBLE::IDLE;
            //     bOutputReady = true;
            //     break;
            // }



            //FPlatformProcess::Sleep((float)ScanDuration / 1000.f);
            //count results
            //count_found_devices = simpleble_adapter_scan_get_results_count(adapter.Get());
            // for( int i = 0; i< count_found_devices; i++){
            //     TSharedPtr<simpleble_peripheral_t, ESPMode::ThreadSafe> new_peripheral = MakeShared<simpleble_peripheral_t, ESPMode::ThreadSafe>(simpleble_adapter_scan_get_results_handle(adapter.Get(), i));
            //     this->devices.Add( new_peripheral );
            // }

            // state_name = FString("Scan has been stopped. Found ") + FString::FromInt( this->devices.Num() ) + FString(" devices.");
            // bStateInitialized = false;
            // state = stateBLE::IDLE;
            // bOutputReady = true;

            break;
        case stateBLE::CONNECTING:
            bStateInitialized = true;

            this->state_name = FString("Connecting to deive");
            simpleble_peripheral_connect(device.Get());
            this->state_name = FString("Connected"); 

            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;
            // FPlatformProcess::Sleep(1.0f);
            // if(!&devices[i]){
            //     continue;
            // }
            // if( !devices[i].is_paired() ){
            //     //this->state_name = FString("Not paired");
            //     FPlatformProcess::Sleep(1.0f);
            //     continue;  
            // }
            // //this->state_name = FString("paired");
            // FPlatformProcess::Sleep(1.0f);
            // }catch( SimpleBLE::Exception::BaseException err ){
            //     //this->state_name = FString("Error device ") + FString(err.what());
            //     FPlatformProcess::Sleep(1.0f);
            //     continue;
            // }
            // devices[i].set_callback_on_connected( [ this, &devices, &i ] () {

            //     this->state_name = FString("Conneced to ") + FString(devices[i].address().value_or("no-address").c_str() );
            //     FPlatformProcess::Sleep(1.0f);
            //     auto services = peripheral.services();
            //     if( !services.has_value() ){
            //         this->state_name = FString("no services ");
            //         periphedevices[i]ral.disconnect();
            //         return;
            //     }
            //     this->state_name = FString("services ");
            //     this->state_name.AppendInt( services->size() );
            //     for (auto service : services.value() ) {
            //         FPlatformProcess::Sleep(5.0f);
            //         this->state_name = FString("Service found: ") + FString(service.uuid().c_str());
            //         if( this->ServiceUUID == FString( service.uuid().c_str() ) ){
            //             this->device = &device[i]; 
            //             this->state_name = FString("Service found");
            //         }
            //         //ServicesUUIDs.Add( FString( service.uuid().c_str() ) );
            //     }
            //     devices[i].disconnect();
            //     FPlatformProcess::Sleep(1.0f);
            // } );

               
            
            break;

        default:
            FPlatformProcess::Sleep(1.0f);
            continue;
        }
        
    }
    return 0;

}
void FAsyncBLE::Exit(){

    bRunBLE = false;
    if( adapter ){
        //delete adapter;
    }
    if( device ){
        //delete device;
    }
}