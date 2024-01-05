#include "BLEAdapter.h"

UBLEAdapter::UBLEAdapter(){
}
void UBLEAdapter::InitRoutine(){
    try{
        AsyncBLE                        = MakeShared< FAsyncBLE, ESPMode::ThreadSafe >();
        AsyncBLE->ScanDuration          = 10000;
        AsyncBLE->state                 = stateBLE::CHECKING_BLUETOOTH;
        AsyncBLE->bStateInitialized     = false;
        AsyncBLE->ServiceUUID           = FString("");
        AsyncBLE->CharacteristicUUID    = FString("");
        AsyncBLE->bInputReady           = true;
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
    }else if(procedure == "Read"){
        AsyncBLE->state = stateBLE::READING;
    }else if(procedure == "Connect"){
        AsyncBLE->state = stateBLE::CONNECTING;
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
    pointerIsOk =  AsyncBLE->adapter_ptr != nullptr;//.IsValid();
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
    // if(AsyncBLE->devices.Num() <= index ){
    //     return;
    // }
    //AsyncBLE->device = AsyncBLE->devices[index];
    if(AsyncBLE->devices_ptrs[index] == nullptr ){
        return;
    }
    AsyncBLE->device_ptr = AsyncBLE->devices_ptrs[index];
    device = NewObject<UBLEDevice>();
    //device->Device = AsyncBLE->devices[index];
    device->Device_ptr = AsyncBLE->devices_ptrs[index];
}
void UBLEAdapter::GetDeviceData(FString characteristicUUID,TArray< uint8 >& data){
    if( !AsyncBLE.IsValid() ){
        return;
    }
    if( AsyncBLE->CharacteristicBytes.Find(characteristicUUID) == nullptr ){
        return;
    }
    data = *(AsyncBLE->CharacteristicBytes.Find(characteristicUUID));
}
void UBLEAdapter::PrepareService(FString serviceUUID,FString characteristicUUID){
    if( !AsyncBLE.IsValid() ){
        return;
    }
    AsyncBLE->ServiceUUID         = serviceUUID;            
    AsyncBLE->CharacteristicUUID  = characteristicUUID;
}
void UBLEAdapter::EndThread(){
    if( !AsyncBLE.IsValid() ){
        return;
    }
    AsyncBLE->bRunBLE = false;
}
void UBLEAdapter::BeginDestroy(){

    if( AsyncBLE.IsValid() ){

        AsyncBLE->bRunBLE = false;

        
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //! Wait here until JoyThread is verified as having stopped!
        //!
        //! This is a safety feature, that will delay PIE EndPlay or
        //! closing of the game while complex calcs occurring on the 
        //! thread have a chance to finish
        //!
        while( !AsyncBLE->HasStopped )
        {
            FPlatformProcess::Sleep(0.1);
        }

        AsyncBLE.Reset();

    }
    AsyncBLE = nullptr;
    Super::BeginDestroy();

}