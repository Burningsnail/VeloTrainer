#include "BLEDevice.h"

UBLEDevice::UBLEDevice(){

}

void UBLEDevice::CheckDevicePointer(bool& pointerIsOk){
    pointerIsOk =   Device != NULL? true: false;
}
void UBLEDevice::UpdateDeviceInfo(){
    // if( !Device.IsValid() ){
    //     return;
    // }
    if( Device_ptr == nullptr ){
        return;
    }
    name            =   FString( simpleble_peripheral_identifier(Device_ptr) );

    address         =   FString( simpleble_peripheral_address(Device_ptr) );


    simpleble_err_t error_result_connectable = simpleble_peripheral_is_connectable(Device_ptr,&connectable);
    simpleble_err_t error_result_paired = simpleble_peripheral_is_paired(Device_ptr,&paired);
    simpleble_err_t error_result_connected = simpleble_peripheral_is_connected(Device_ptr,&isConnected);

    // name += FString(error_result_connectable ? "-connectable":"-no connectable");
    // name += FString(error_result_paired ? "-paired":"-no paired");
    // name += FString(error_result_connected ? "-connected":"-no connected");

}
void UBLEDevice::UpdateServices(){

    int count  = simpleble_peripheral_services_count( Device_ptr );
    for( int i =0; i< count; i++ ){
        simpleble_service_t device_new;
        simpleble_peripheral_services_get( Device_ptr, i, &device_new );
        Services.Add( device_new.uuid.value );
    }
    
}
// void UBLEDevice::UpdateDeviceData(){
//     Data = AsyncBLE->Bytes;
// }
// void UBLEDevice::Init(  TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe>  asyncManager , TSharedPtr<SimpleBLE::Peripheral, ESPMode::ThreadSafe> peripheral ){
//     //AsyncBLE = asyncManager;
//     Device = peripheral;
// }