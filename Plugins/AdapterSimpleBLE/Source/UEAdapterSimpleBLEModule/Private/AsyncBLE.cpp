#include "AsyncBLE.h"

FAsyncBLE::FAsyncBLE(){
        bRunBLE = true;
        Thread = FRunnableThread::Create(this, TEXT("AsyncBLE Thread"));
}
uint32_t FAsyncBLE::Run(){
    while( !bInputReady ){
        FPlatformProcess::Sleep(1.0f);
    }
    std::vector< SimpleBLE::Peripheral > devices;
    //std::uintptr_t ptr_i;
    while( this->bRunBLE ){
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
            state_name = FString("BlueTooth checking");
            FPlatformProcess::Sleep(5.0f);
            //check BlueTooth      
            if( !bBlueToothEnabled ){
                state_name = FString("BlueTooth is disabled");

                bBlueToothEnabled = SimpleBLE::Safe::Adapter::bluetooth_enabled()? true : false;
                
                if(bBlueToothEnabled){
                    //next step
                    bStateInitialized = false;
                    state = stateBLE::COLLECTING_ADAPTERS;
                    state_name = FString("BlueTooth enabled");
                }
                continue;
            }else{
                state_name = FString("BlueTooth enabled");
            }

            break;
        case stateBLE::COLLECTING_ADAPTERS:
            
            bStateInitialized = true;
            FPlatformProcess::Sleep(5.0f);

            if(adapter == nullptr){
                auto adapter_list = SimpleBLE::Adapter::get_adapters();
                FPlatformProcess::Sleep(1.0f);
                if( adapter_list.empty() ){
                    state_name = FString("Adapter list empty");
                    continue;
                }

                state_name = FString("Adapter list");

                FPlatformProcess::Sleep(3.0f);

                adapter = new SimpleBLE::Adapter(adapter_list.at(0));
                
            }
            FPlatformProcess::Sleep(1.0f);
            if(adapter != nullptr){
                //ptr_i = reinterpret_cast<std::uintptr_t>(adapter);
                state_name = FString("Adapter: ") ;//+ FString::FromInt(ptr_i);
                
                bStateInitialized = false;
                state = stateBLE::SCANNING;

            }else{
                //get once more
                state_name = FString("Adapter not found");
                bStateInitialized = false;
                state = stateBLE::COLLECTING_ADAPTERS;
            }
            //next step

            break;

        case stateBLE::SCANNING:
            
            bStateInitialized = true;
            FPlatformProcess::Sleep(5.0f);
            //scan
            //Manager->startScan();
            adapter->set_callback_on_scan_found([this, &devices](SimpleBLE::Peripheral peripheral) {

                if(device != nullptr){
                    delete( device );
                }
                //device = new SimpleBLE::Peripheral( peripheral );
                devices.push_back( peripheral );
                SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( peripheral );
                // FString device_name = FString( safe_device.is_connectable() ? "true":"false" )
                //                     + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() )
                //                     + FString(" address: ")  + FString(  safe_device.address().value_or("no address").c_str() );

                this->state_name = FString("device Found");

                //std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;
                //from trainer code
                //const SimpleBLE::BluetoothUUID address = "6e40fec1-b5a3-f393-e0a9-e50e24dcca9e";//from internet https://github.com/abellono/tacx-ios-bluetooth-example/blob/master/BleTrainerControl/BTLEConstants.h
                const SimpleBLE::BluetoothUUID characteristic_velocity = "6E40FEC2-B5A3-F393-E0A9-E50E24DCCA9E";//from our code

                // this->onScanFoundDevice.Broadcast();
                return;  
            });
            adapter->set_callback_on_scan_start([this]() { 
                            this->state_name = FString("Scanning for ") + FString::FromInt( this->ScanDuration );
                            
             });
            adapter->set_callback_on_scan_stop([this, &devices]() { 
                    //next step
                    if(devices.size()){
                        this->bStateInitialized = false;
                        this->state = stateBLE::CONNECTING;
                    }else{
                        //new scan
                        this->bStateInitialized = false;
                        this->state = stateBLE::SCANNING;
                    }
                    this->state_name = FString("Found ") + FString::FromInt( devices.size() ) + FString(" devices: ");

                    for( int i = 0; i<devices.size(); i++ ){
                        SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( devices[i] );
                        this->state_name += FString( safe_device.is_connectable() ? "true":"false" )
                                            + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() );
                    }


             });
            adapter->scan_for(ScanDuration);

            break;
        case stateBLE::CONNECTING:
            bStateInitialized = true;
            FPlatformProcess::Sleep(5.0f);

            for( int i=0; i<devices.size();i++ ){

                auto services = SimpleBLE::Safe::Peripheral( devices[i] ).services();
                if( !services.has_value() ){
                    this->state_name = FString("no services ");
                    
                    continue;
                }
                this->state_name = FString("services ");
                this->state_name.AppendInt( services->size() );
                for (auto service : services.value() ) {
                    FPlatformProcess::Sleep(5.0f);
                    this->state_name = FString("Service found: ") + FString(service.uuid().c_str());
                    if( this->ServiceUUID == FString( service.uuid().c_str() ) ){
                        this->device = &device[i]; 
                        this->state_name = FString("Service found");
                    }
                    //ServicesUUIDs.Add( FString( service.uuid().c_str() ) );
                }
            }

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
        delete adapter;
    }
    if( device ){
        delete device;
    }
}