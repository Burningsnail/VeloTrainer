#include "BLEAdapter.h"

UBLEAdapter::UBLEAdapter(){
}
void UBLEAdapter::InitRoutine(){
    try{
        AsyncBLE = MakeShared<FAsyncBLE, ESPMode::ThreadSafe>();
        AsyncBLE->ScanDuration  = 10000;
        AsyncBLE->state = stateBLE::CHECKING_BLUETOOTH;
        AsyncBLE->bStateInitialized = false;
        AsyncBLE->ServiceUUID = FString("");
        AsyncBLE->CharacteristicUUID = FString("");
        AsyncBLE->bInputReady = true;
    }catch(...){
        UE_LOG(LogTemp, Warning, TEXT("BLE Thread error"));
    }
}
void UBLEAdapter::PerformBLEProcedure( FString procedure, bool& success){
    success = false;
    if(AsyncBLE->state != stateBLE::IDLE){
        success = false;
        return;
    }
    //disable state machine
    AsyncBLE->bInputReady = false;
    //switch mode state machine
    if( procedure == "Check BlueTooth" ){
        AsyncBLE->state = stateBLE::COLLECTING_ADAPTERS;
    }else if(procedure == "Collect Adapters"){
        AsyncBLE->state = stateBLE::COLLECTING_ADAPTERS;
    }else if(procedure == "Scan"){
        AsyncBLE->state = stateBLE::SCANNING;
    }else{
       AsyncBLE->state = stateBLE::ERROR;
       success = false;
       return;
    }
    
    //resume state machine
    AsyncBLE->bStateInitialized = false;
    AsyncBLE->bInputReady = true;
    
    //wait for results
    success = true;
}
void UBLEAdapter::UpdateState(){
    //check if BLE thread is runnung
    if( !AsyncBLE ){
        return;
    }
    state = (int)AsyncBLE->state;
    if( state != stateBLE::IDLE ){
        return;
    }
    state_name = AsyncBLE->state_name;
    outputReady = AsyncBLE->bOutputReady;
    devicesCount = AsyncBLE->devices_ptrs.Num();
}
void UBLEAdapter::CheckAdapterPointer(bool& pointerIsOk){
    pointerIsOk = false;
    if( !AsyncBLE.IsValid() ){
        return;
    }
    state = (int)AsyncBLE->state;
    if( state != stateBLE::IDLE ){
        return;
    }
    if( !AsyncBLE->bOutputReady){
        return;
    }
    pointerIsOk =  AsyncBLE->adapter.IsValid();
}
void UBLEAdapter::IsAdapterInitialized(bool& initialized){

}
void UBLEAdapter::SelectDevice( int index ){
    if( !AsyncBLE.IsValid() ){
        return;
    }
    state = (int)AsyncBLE->state;
    if( state != stateBLE::IDLE ){
        return;
    }
    if(AsyncBLE->devices.Num() <= index ){
        return;
    }
    AsyncBLE->device = AsyncBLE->devices[index];
    device = NewObject<UBLEDevice>();
    device->Device = AsyncBLE->device;
    device->Device_ptr = AsyncBLE->devices_ptrs[index];
}