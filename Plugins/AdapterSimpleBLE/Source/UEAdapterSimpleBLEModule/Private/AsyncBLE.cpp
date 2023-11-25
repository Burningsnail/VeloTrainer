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
    while( this->bRunBLE ){
        FPlatformProcess::Sleep(1.0f);
        //check BlueTooth
        
        if( !bBlueToothEnabled ){
            state_name = FString("BlueTooth is disabled");

            bBlueToothEnabled = SimpleBLE::Safe::Adapter::bluetooth_enabled()? true : false;
            
            if(bBlueToothEnabled){
                state_name = FString("BlueTooth enabled");
            }
            continue;
        }

        
        
        if( adapter == nullptr ){

            auto adapter_list = SimpleBLE::Adapter::get_adapters();
            FPlatformProcess::Sleep(1.0f);
            // if( !adapter_list.has_value() ){
            //     state_name = FString("No Bluetooth Adapters");
            //     return;
            // };


            if( adapter_list.empty() ){
                state_name = FString("Adapter list empty");
                continue;
            }

            state_name = FString("Adapter list");

            FPlatformProcess::Sleep(1.0f);

            adapter = new SimpleBLE::Adapter(adapter_list.at(0));
            
            // for( int i  = 0; i<adapter_list.size(); i++  ){
            //     //auto ptr_i = reinterpret_cast<std::uintptr_t>(&adapter_list.at(i));

            //     //FString(adapter->identifier().c_str() );
            //     //state_name += FString("[") + FString::FromInt(i) + FString( "]" ) + FString::FromInt(ptr_i);
            // }

        }
        FPlatformProcess::Sleep(1.0f);
        if( !devices.size() ){
           

            auto ptr_i = reinterpret_cast<std::uintptr_t>(adapter);
            state_name = FString("Adapter: ") + FString::FromInt(ptr_i);

            //scan
            //Manager->startScan();
            adapter->set_callback_on_scan_found([this, &devices](SimpleBLE::Peripheral peripheral) {

                if(device != nullptr){
                    delete( device );
                }
                device = new SimpleBLE::Peripheral( peripheral );
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

        

            adapter->set_callback_on_scan_start([]() {  });
            adapter->set_callback_on_scan_stop([]() {  });
            state_name = FString("Scanning for ") + FString::FromInt( ScanDuration );
            adapter->scan_for(ScanDuration);
        }else{
            FPlatformProcess::Sleep(1.0f);
            state_name = FString("Found ") + FString::FromInt( devices.size() ) + FString(" devices: ");

            for( int i = 0; i<devices.size(); i++ ){
                SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( devices[i] );
                state_name += FString( safe_device.is_connectable() ? "true":"false" )
                                    + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() );
            }
        }
        //

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