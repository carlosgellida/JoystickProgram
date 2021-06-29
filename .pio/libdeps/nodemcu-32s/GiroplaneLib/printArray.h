void *print3Darray(float arr[3]){
    int elements = 3 ; 
    for(int i = 0; i < elements; i++){
        if(i == 0){
            Serial.print("[") ; 
            Serial.print(arr[i]) ; 
            Serial.print(" ") ; 
        } else if(i == (elements -1)){
            Serial.print(arr[i]) ; 
            Serial.println("]") ; 
        } else{
            Serial.print(arr[i]) ; 
            Serial.print(" ") ; 
        }
    }
}