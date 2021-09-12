void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("setup");
  Serial.println("ECG, 4000");


  pinMode(2, INPUT); //Se configura detección de LO-
  pinMode(4, INPUT); //Se configura detección de LO+
}

void loop() {
  if((digitalRead(2)==1)||(digitalRead(2)==1)){
    Serial.println("Conecte electrodos Porfavor");
    delay(1000);
    }
  else{
    Serial.println(analogRead(15)); //Lectura ECG
    }
  
  delay(1);
  //Serial.print(','); //Lectura ECG
  //Serial.println(4000); //Lectura ECG

}
