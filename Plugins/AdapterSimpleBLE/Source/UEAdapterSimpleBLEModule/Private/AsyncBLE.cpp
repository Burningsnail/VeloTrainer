#include "AsyncBLE.h"

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

        try{
        //do state only once and then wait
        if( bStateInitialized){
            FPlatformProcess::Sleep(1.0f);
            continue;
        }

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

                bBlueToothEnabled = SimpleBLE::Safe::Adapter::bluetooth_enabled()? true : false;
                
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
                auto adapter_list = SimpleBLE::Adapter::get_adapters();
                
                UE_LOG(LogTemp, Warning, TEXT("if Empty"));
                if( adapter_list.empty() ){
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
                adapter = MakeShared<SimpleBLE::Adapter, ESPMode::ThreadSafe>(adapter_list.at(0));
                
            }
            UE_LOG(LogTemp, Warning, TEXT("if Adapter is null"));
            // FPlatformProcess::Sleep(3.0f);

            if(adapter != nullptr){
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
            adapter->set_callback_on_scan_found([this](SimpleBLE::Peripheral peripheral) {

                // if(device != NULL){
                //     //delete( device );
                // }
                
                //device = new SimpleBLE::Peripheral( peripheral );
                // TSharedPtr<UBLEDevice, ESPMode::ThreadSafe> new_device = MakeShared<UBLEDevice, ESPMode::ThreadSafe>();
                TSharedPtr<SimpleBLE::Peripheral, ESPMode::ThreadSafe> new_peripheral = MakeShared<SimpleBLE::Peripheral, ESPMode::ThreadSafe>(peripheral);
                // TSharedPtr< FAsyncBLE, ESPMode::ThreadSafe > shared_asyncBLE = MakeShareable<FAsyncBLE>(this);
                // //new_device->Init( shared_asyncBLE , new_peripheral);
                // new_device->AsyncBLE = shared_asyncBLE;

                this->devices.Add( new_peripheral );
                // SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( peripheral );
                // FString device_name = FString( safe_device.is_connectable() ? "true":"false" )
                //                     + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() )
                //                     + FString(" address: ")  + FString(  safe_device.address().value_or("no address").c_str() );

                //this->state_name = FString("device Found");

                //std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;
                //from trainer code
                //const SimpleBLE::BluetoothUUID address = "6e40fec1-b5a3-f393-e0a9-e50e24dcca9e";//from internet https://github.com/abellono/tacx-ios-bluetooth-example/blob/master/BleTrainerControl/BTLEConstants.h
                // const SimpleBLE::BluetoothUUID characteristic_velocity = "6E40FEC2-B5A3-F393-E0A9-E50E24DCCA9E";//from our code

                // this->onScanFoundDevice.Broadcast();
            });
            UE_LOG(LogTemp, Warning, TEXT("set Callback for scan start"));
            adapter->set_callback_on_scan_start([this]() { 
                            //this->state_name = FString("Scanning for ") + FString::FromInt( this->ScanDuration );
                            this->devices.Empty();
            });
            UE_LOG(LogTemp, Warning, TEXT("set Callback for stop"));
            adapter->set_callback_on_scan_stop([this]() { 
                state_name = FString("Scan has been stopped. Found ") + FString::FromInt( this->devices.Num() ) + FString(" devices.");
                bStateInitialized = false;
                state = stateBLE::IDLE;
                bOutputReady = true;
                    //next step
                    // if(devices.size()){
                    //     this->bStateInitialized = false;
                    //     this->state = stateBLE::CONNECTING;
                    // }else{
                    //     //new scan
                    //     this->bStateInitialized = false;
                    //     this->state = stateBLE::SCANNING;
                    // }
                    // //this->state_name = FString("Found ") + FString::FromInt( devices.size() ) + FString(" devices: ");

                    // for( int i = 0; i<devices.size(); i++ ){
                    //     SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( devices[i] );
                    //     //this->state_name += FString( safe_device.is_connectable() ? "true":"false" )
                    //     //                    + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() );
                    // }


            });
            UE_LOG(LogTemp, Warning, TEXT("start scan"));
            adapter->scan_for(ScanDuration);

            break;
        case stateBLE::CONNECTING:
            bStateInitialized = true;
            //FPlatformProcess::Sleep(5.0f);

            try{
            for( int i=0; i<devices.Num();i++ ){
                // try{
                // auto peripheral = SimpleBLE::Peripheral(devices[i]);
                // bool address = false;
                // try{
                //     //this->state_name = FString("Connecting to ") + FString( peripheral.address().c_str() );
                //     address = true;
                // }catch(...){
                //     //this->state_name = FString("Connecting to device");
                //     address = false;
                // }
                
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

                try{
                    //devices[i].connect();
                    }
                catch( SimpleBLE::Exception::BaseException err ){
                    //this->state_name = FString("Error empty ") + FString(err.what());
                }
                FPlatformProcess::Sleep(5.0f);
                
            }
            }catch( SimpleBLE::Exception::BaseException err ){
                //this->state_name = FString("Error common ") + FString(err.what());
            }
            break;

        default:
            FPlatformProcess::Sleep(1.0f);
            continue;
        }
        }catch( SimpleBLE::Exception::BaseException err ){

            //this->state_name = FString("Error whole ") + FString(err.what());
            state_name = FString("Error ") + FString(err.what());
            bStateInitialized = false;
            state = stateBLE::IDLE;
            bOutputReady = true;
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