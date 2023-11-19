#include "BLEManager.h"
#include "UObject/UObjectBaseUtility.h"
THIRD_PARTY_INCLUDES_START
#include <../ThirdParty/UEAdapterSimpleBLE/include/simpleble/SimpleBLE.h>
THIRD_PARTY_INCLUDES_END

enum state_BLE{
    idle,
    scanning,
    connected,
    subscribe,
    found,
    error
};

void UBLEManager::isBluetoothEnabled( bool& result ){
    result = SimpleBLE::Safe::Adapter::bluetooth_enabled()? true : false;
    bluetoothEnabled = result;
}
void UBLEManager::collectAdapter(){

    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    // if( !adapter_list.has_value() ){
    //     state_name = FString("No Bluetooth Adapters");
    //     return;
    // };
    if( adapter_list.empty() ){
        state_name = FString("No Bluetooth Adapters");
        state = state_BLE::error;
        return;
    }
    adapter = adapter_list.at(0);
    state_name = FString("Adapter found");
}

void UBLEManager::BLERoutine(){
    
    switch( state ){
        case idle:
            // state_name = FString("Idle");
            // state = state_BLE::scanning;//start scan
            // state_name = FString("Scanning");
            // if(!adapter.scan_is_active()){
            //     adapter.set_callback_on_scan_found([this](SimpleBLE::Safe::Peripheral peripheral) {
            //         device = peripheral;
            //         this->device_name = FString(  device.identifier().value_or("no id").c_str() );
            //         this->state_name = FString("Device Found: ") + this->device_name;
            //         state = state_BLE::found;

            //     });
            //     adapter.scan_for( 5000 );
            // }
        break;
        case scanning:
            state_name = FString("Scanning");

        break;
    }


}
void UBLEManager::StartScan(){

    adapter.set_callback_on_scan_found([this](SimpleBLE::Peripheral peripheral) {
        // SimpleBLE::Peripheral* new_device = new SimpleBLE::Peripheral();
        // (*new_device) = peripheral;
        // device = std::make_shared<SimpleBLE::Safe::Peripheral> ( SimpleBLE::Safe::Peripheral (*new_device) );
        device = peripheral;
        SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( peripheral );
        this->device_name = FString( safe_device.is_connectable() ? "true":"false" )
                            + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() )
                            + FString(" address: ")  + FString(  safe_device.address().value_or("no address").c_str() );
        // device = peripheral;
        // this->device_name = FString("connectable: " ) + FString( device.is_connectable() ? "true":"false" ) 
        //                     + FString("device: " ) + FString(  peripheral.identifier().value_or("no identifier").c_str() ) + FString(" ") 
        //                     + FString(  peripheral.address().value_or("no address").c_str() );

        this->state_name = FString("Device Found: ") + this->device_name;
        state = state_BLE::found;
        //std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;
        //from trainer code
        //const SimpleBLE::BluetoothUUID address = "6e40fec1-b5a3-f393-e0a9-e50e24dcca9e";//from internet https://github.com/abellono/tacx-ios-bluetooth-example/blob/master/BleTrainerControl/BTLEConstants.h
        const SimpleBLE::BluetoothUUID characteristic_velocity = "6E40FEC2-B5A3-F393-E0A9-E50E24DCCA9E";//from our code

        this->onScanFoundDevice.Broadcast();
        return;

        // SimpleBLE::Safe::Peripheral( device ).set_callback_on_connected([this, characteristic_velocity]() {
        //     this->state_name = FString("Connected");
        //     auto services = device.services();
        //     if( !services.has_value() ){
        //         this->state_name = FString("no services ");
                
        //         return;
        //     }
        //     this->state_name = FString("services ");
        //     this->state_name.AppendInt( services->size() );
            
        //     for (auto service : services.value() ) {
        //         for (auto characteristic : service.characteristics()) {

        //         }
        //     }
        //     adapter.scan_stop();
        // });
        
        // this->state_name = device.is_connectable() ? FString("connectable") : FString("not connectable");
        // // device.connect();
        // if( device.is_connectable() ){
        //     device.connect();
        // }
        // // if( !device.is_connected() ){
        // //     this->state_name = FString("Not Connected");
        // // }


        

    });

    

    adapter.set_callback_on_scan_start([]() {  });
    adapter.set_callback_on_scan_stop([]() {  });

    adapter.scan_for(100);
    state_name = FString("Scanning");

}
UBLEManager::UBLEManager(){
    // SimpleBLE::Adapter new_adapter;
    // adapter = SimpleBLE::Safe::Adapter( new_adapter );
    // UObjectBaseUtility::AddToRoot();
}

void UBLEManager::StopScan(){
    try{    
        adapter.scan_stop();
    }catch( SimpleBLE::Exception::BaseException error ){
        this->state_name = FString( "Error" );
    }
}


void UBLEManager::DeviceDisconnect(){
    // device.disconnect();

}

void UBLEManager::ConnectCurrentDevice(){
    device.set_callback_on_connected([this]() {
            this->state_name = FString("Connected"); 
            //this->onDeviceConnect.Broadcast();
        });
    device.connect();
}

void UBLEManager::GetDeviceServices( TArray<FString>& ServicesUUIDs ){
    auto services = SimpleBLE::Safe::Peripheral( device ).services();
    if( !services.has_value() ){
        this->state_name = FString("no services ");
        
        return;
    }
    this->state_name = FString("services ");
    this->state_name.AppendInt( services->size() );
    for (auto service : services.value() ) {
        ServicesUUIDs.Add( FString( service.uuid().c_str() ) );
    }

}

void UBLEManager::Subscribe( const FString ServiceUUID, const FString CharacteristicUUID){

    device.notify( TCHAR_TO_UTF8(*ServiceUUID), TCHAR_TO_UTF8(*CharacteristicUUID), 
        [&](SimpleBLE::ByteArray bytes){
            TArray<uint8> data;
            for( int i=0;i<bytes.size();i++ ){
                data.Add(bytes[i]);
            }
            this->onNotifyRead.Broadcast( data );
        } 
    );

}
void UBLEManager::isScanning( bool& result, bool& result_valid ){
    result = false;
    auto result_scan_active = SimpleBLE::Safe::Adapter( adapter ).scan_is_active();
    result_valid = result_scan_active.has_value();
    if(result_valid){
        result = result_scan_active.value();
    }
}
void UBLEManager::connectRoutine(){
    // try{
    //     auto adapter_list = SimpleBLE::Adapter::get_adapters();
    //     // if( !adapter_list.has_value() ){
    //     //     state_name = FString("No Bluetooth Adapters");
    //     //     return;
    //     // };
    //     if( adapter_list.empty() ){
    //         state_name = FString("No Bluetooth Adapters");
    //         state = state_BLE::error;
    //         return;
    //     }
    //     auto adapter = adapter_list.at(0);
    //     state_name = FString("Adapter found");
        
    //     adapter.set_callback_on_scan_found([this](SimpleBLE::Peripheral peripheral) {
    //         // SimpleBLE::Peripheral* new_device = new SimpleBLE::Peripheral();
    //         // (*new_device) = peripheral;
    //         // device = std::make_shared<SimpleBLE::Safe::Peripheral> ( SimpleBLE::Safe::Peripheral (*new_device) );

    //         SimpleBLE::Safe::Peripheral safe_device = SimpleBLE::Safe::Peripheral( peripheral );
    //         this->device_name = FString( safe_device.is_connectable() ? "true":"false" )
    //                             + FString(" device: " ) + FString(  safe_device.identifier().value_or("no identifier").c_str() )
    //                             + FString(" address: ")  + FString(  safe_device.address().value_or("no address").c_str() );
    //         // device = peripheral;
    //         // this->device_name = FString("connectable: " ) + FString( device.is_connectable() ? "true":"false" ) 
    //         //                     + FString("device: " ) + FString(  peripheral.identifier().value_or("no identifier").c_str() ) + FString(" ") 
    //         //                     + FString(  peripheral.address().value_or("no address").c_str() );

    //         this->state_name = FString("Device Found: ") + this->device_name;
    //         state = state_BLE::found;
    //         //std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;
    //         //from trainer code
    //         //const SimpleBLE::BluetoothUUID address = "6e40fec1-b5a3-f393-e0a9-e50e24dcca9e";//from internet https://github.com/abellono/tacx-ios-bluetooth-example/blob/master/BleTrainerControl/BTLEConstants.h
    //         const SimpleBLE::BluetoothUUID characteristic_velocity = "6E40FEC2-B5A3-F393-E0A9-E50E24DCCA9E";//from our code

    //         this->onScanFoundDevice.Broadcast();
    //         return;
    //     });

        

    //     adapter.set_callback_on_scan_start([]() {  });
    //     adapter.set_callback_on_scan_stop([]() {  });

    //     adapter.scan_for(100);
    //     state_name = FString("Scanning");
    // }catch( SimpleBLE::Exception::BaseException err ){
    //     state_name = FString("Error");
    // }

}