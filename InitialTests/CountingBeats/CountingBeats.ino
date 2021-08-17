
#define beat 2400 //Valor medio del pico del pulso: 2400 a 12 bits
#define MinTimebeat 300  //Se considera un tiempo minimo entre latidos para validar el tiempo

//Counting beats using millis function
unsigned long tiempo1=0; //Used to save the current time
unsigned long tiempo2=0; //Used to save the time when a beat is produced
unsigned int value_beats; //Valor de latidos
unsigned int maximo;


int pulsos=0; //Conteo de pulsos
//muestreo de 3 latidos para realizar el promedio
unsigned long time_beats[3]={1200, 1200, 1200};
unsigned long meanTime;
unsigned long BPM;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("setup ECG SERIAL VIEW");
  Serial.println("ECG, 4000");
  pinMode(2, INPUT); //Se configura detección de LO-
  pinMode(4, INPUT); //Se configura detección de LO+
}

void loop() {
  tiempo2=millis(); //Tiempo actual
  Lectura_ECG();
  Recoger_Pulsos();
  Mostrar_Pulsos();
  //Serial.print(','); //Lectura ECG
  //Serial.println(4000); //Lectura ECG
  
}


void Lectura_ECG(){
    if((digitalRead(2)==1)||(digitalRead(2)==1)){
    Serial.println('!');
    }
  else{
    value_beats=analogRead(15);
    Recoger_Pulsos();
    }

  Mostrar_Pulsos();
  delay(10); //Retardo de 10 ms entre lecturas
  }

void Mostrar_Pulsos(){
  if (millis() >= tiempo2 + 2000){ //Mostrar pulsos cada 2 seg
    for(int i=0;i<3;i++){
      
      meanTime+=time_beats[i];
      }
      meanTime=meanTime/3;
      BPM=(1/meanTime)*60;
      Serial.print("BPM: ");
      Serial.println(BPM);
      tiempo2=millis(); //Actualizo el tiempo
    }
  }
void Recoger_Pulsos(){
  if ((value_beats>beat)&&(tiempo1==0)){
    Serial.println("OK");
    Serial.println(millis());
    tiempo1=millis();//Primer Pulso reconocido
    }
  if((value_beats>beat)&&((millis()-tiempo1)>=MinTimebeat)){
    if(pulsos==3){
      pulsos=0;
      }
    else{
      time_beats[pulsos]=tiempo2-tiempo1;
      }
    pulsos++;
    }
  }
