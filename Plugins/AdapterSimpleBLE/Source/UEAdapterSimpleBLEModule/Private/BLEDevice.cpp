#include "BLEDevice.h"

UBLEDevice::UBLEDevice(){

}

void UBLEDevice::CheckDevicePointer(bool& pointerIsOk){
    pointerIsOk =   Device != NULL? true: false;
}
void UBLEDevice::UpdateDeviceInfo(){
    if( !Device.IsValid() ){
        return;
    }
    // if( AsyncBLE == NULL ){
    //     return;
    // }
    // if( AsyncBLE->state != stateBLE::IDLE ){
    //     return;
    // }
    // if( !AsyncBLE->bOutputReady){
    //     return;
    // }
    try{
        name            =   FString( Device->identifier().c_str() );
        if( !Device.IsValid() ){
            return;
        }
        address         =   FString( Device->address().c_str() );
        if( Device->initialized() ){
            if( !Device.IsValid() ){
                return;
            }
            connectable     =   Device->is_connectable();
            if( !Device.IsValid() ){
                return;
            }
            paired          =   Device->is_paired();
            if( !Device.IsValid() ){
                return;
            }
            isConnected     =   Device->is_connected();
        }
    }catch( SimpleBLE::Exception::BaseException err){
        name = FString(err.what());
    }
}
// void UBLEDevice::Init(  TSharedPtr<FAsyncBLE, ESPMode::ThreadSafe>  asyncManager , TSharedPtr<SimpleBLE::Peripheral, ESPMode::ThreadSafe> peripheral ){
//     //AsyncBLE = asyncManager;
//     Device = peripheral;
// }