#include "BLEManager.h"

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

    auto adapter_list = SimpleBLE::Safe::Adapter::get_adapters();
    if( !adapter_list.has_value() ){
        state_name = FString("No Bluetooth Adapters");
        return;
    };
    if( adapter_list->empty() ){
        state_name = FString("No Bluetooth Adapters");
        state = state_BLE::error;
        return;
    }
    SimpleBLE::Adapter* new_adapter = new SimpleBLE::Adapter();
    (*new_adapter) = adapter_list->at(0);
    adapter = new SimpleBLE::Safe::Adapter( (*new_adapter) );
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
            //         this->device = peripheral;
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
    if( !adapter ){
        this->state_name = FString("No adapter");
    }
    adapter->set_callback_on_scan_found([this](SimpleBLE::Safe::Peripheral peripheral) {

        SimpleBLE::Peripheral* new_device = new SimpleBLE::Peripheral();
        (*new_device) = peripheral;
        this->device = new SimpleBLE::Safe::Peripheral( (*new_device) );
        
        this->device_name = FString("Some Device");
        // this->device = peripheral;
        // this->device_name = FString("connectable: " ) + FString( this->device->is_connectable() ? "true":"false" ) 
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

        this->device->set_callback_on_connected([this, characteristic_velocity]() {
            this->state_name = FString("Connected");
            auto services = this->device->services();
            if( !services.has_value() ){
                this->state_name = FString("no services ");
                
                return;
            }
            this->state_name = FString("services ");
            this->state_name.AppendInt( services->size() );
            
            for (auto service : services.value() ) {
                for (auto characteristic : service.characteristics()) {

                }
            }

        });
        
        this->state_name = this->device->is_connectable() ? FString("connectable") : FString("not connectable");
        // this->device->connect();
        if( this->device->is_connectable() ){
            this->device->connect();
        }
        // if( !this->device.is_connected() ){
        //     this->state_name = FString("Not Connected");
        // }
        
        adapter->scan_stop();

        

    });
    adapter->set_callback_on_scan_start([]() {  });
    adapter->set_callback_on_scan_stop([]() {  });

    adapter->scan_start();
    state_name = FString("Scanning");

}
UBLEManager::UBLEManager(){

}

void UBLEManager::StopScan(){
    if( adapter ){
        adapter->scan_stop();
    }
}


void UBLEManager::DeviceDisconnect(){
    if( device ){
        device->disconnect();
    }
}

void UBLEManager::ConnectCurrentDevice(){
    this->device->set_callback_on_connected([this]() {
            this->state_name = FString("Connected"); 
            this->onDeviceConnect.Broadcast();
        });
    this->device->connect();
}

void UBLEManager::GetDeviceServices( TArray<FString>& ServicesUUIDs ){
    auto services = this->device->services();
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

    this->device->notify( TCHAR_TO_UTF8(*ServiceUUID), TCHAR_TO_UTF8(*CharacteristicUUID), 
        [&](SimpleBLE::ByteArray bytes){
            TArray<uint8> data;
            for( int i=0;i<bytes.size();i++ ){
                data.Add(bytes[i]);
            }
            this->onNotifyRead.Broadcast( data );
        } 
    );

}