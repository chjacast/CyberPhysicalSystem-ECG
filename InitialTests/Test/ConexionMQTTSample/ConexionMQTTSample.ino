#include <WiFi.h>
#include <PubSubClient.h> //Libreria enfocada a la comunicacion 
#include <Wire.h>

#define ledPin1 2 //ledPin GPIO2
#define ledPin2 4 //ledPin GPIO4
#define ledPin3 15 //ledPin GPIO15

#define ledPin4 16 //LedPIN GPIO16

//Creamos la instancia cliente
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg =0;
char msg[50];
int value=0;

//El ejemplo utiliza señales de temperatura y humedad utilizando com I2C
float temperature =0;
float humidity =0;
//----PWM----
int freq = 5000;
int ledChannel=0;
int resolution=8;

//USUARIO PARA CONECTARNOS AL SERVIDOR******************
const char* ssid= "TVORO_CASTILLO"; //Nombre del WiFi
const char* password= "07044433945"; //Contraseña del WiFi

//DIRECCION IP del MQTT Broker
const char* mqtt_server = "192.168.1.27";



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,1883); //Permitir la conexion con el broker
  client.setCallback(callback);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  //PIN PARA USO PWM
  ledcSetup(ledChannel,freq,resolution);
  ledcAttachPin(ledPin4,ledChannel);
}
//SETUP WIFI-----------------------------------------------------
void setup_wifi(){
  delay(10); 
  Serial.println();
  Serial.print("Conectando a ....");
  Serial.println("ssid");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi conexion establecida exitosa!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//SETUP WIFI-----------------------------------------------------
//CALLBACK-----------------------------------------------------
//Realiza la conexion MQTT para recibir datos (Subscriber) (Broker->ESP32)
void callback(char* topic, byte* message, unsigned int llength){
  Serial.print("Mensaje recibido en topic: ");
  Serial.println(topic);
  Serial.print(", Message: ");
  String messageTemp;
  for (int i=0;i<llength;i++){
    Serial.print((char)message[i]);
    messageTemp +=(char)message[i];
    }
  Serial.println();

  //INTERACCION DEPENDIENDO DE LOS TOPICS
  //OUTPUT1
  if(String(topic) == "esp32/output1"){
    Serial.print("Cambio de salida: ");
    if(messageTemp="on"){
      Serial.println("on");
      digitalWrite(ledPin1,HIGH);
      }
    else if(messageTemp="off"){
      Serial.println("off");
      digitalWrite(ledPin1,LOW);
      }
    }
  //OUTPUT2
  if(String(topic) == "esp32/output2"){
    Serial.print("Cambio de salida: ");
    if(messageTemp="on"){
      Serial.println("on");
      digitalWrite(ledPin2,HIGH);
      }
    else if(messageTemp="off"){
      Serial.println("off");
      digitalWrite(ledPin2,LOW);
      }
    }
  //OUTPUT3
  if(String(topic) == "esp32/output3"){
    Serial.print("Cambio de salida: ");
    if(messageTemp="on"){
      Serial.println("on");
      digitalWrite(ledPin3,HIGH);
      }
    else if(messageTemp="off"){
      Serial.println("off");
      digitalWrite(ledPin3,LOW);
      }
    }
  //OUTPUT 4 (PARA USO DE PWM)
  if(String(topic) == "esp32/output3"){
    Serial.print("Cambio de salida PWM: ");
    Serial.println(messageTemp);
    ledcWrite(ledChannel,messageTemp.toInt());
    }
    //Se agrega las salidas a controlar que se desee
  }
//CALLBACK-----------------------------------------------------
//RECONNECT-----------------------------------------------------
//Para observar si mi conexion MQTT es correcta
void reconnect(){
  while(!client.connected()){
    Serial.print("Intentando establecer conexion MQTT con el servidor...");
    if(client.connect("ESP8266Client")){
      Serial.println("Conectado!");
      client.subscribe("esp32/output1"); //TOPICO output1
      client.subscribe("esp32/output2"); //TOPICO output2
      client.subscribe("esp32/output3"); //TOPICO output3
      client.subscribe("esp32/output4"); //TOPICO output4
      }
    else{
      Serial.print("Fallo de conexion, rc=");
      Serial.print(client.state());
      Serial.println(" Intente nuevamente en 5 seg.");
      delay(5000);
      }
   }
}
//RECONNECT-----------------------------------------------------
//VOID LOOP-----------------------------------------------------
//REALIZA CONEXION MQTT COMO PUBLICADOR (ESP32->Broker)
void loop() {
  if(!client.connected()){
    reconnect();
    }
  client.loop();

  long now = millis();
  if(now-lastMsg > 100){ //100 ms (tiempo de muestreo)
    lastMsg=now;
    //SEÑALES QUE SE DESEA ENVIAR AL BROKER
    //Temperatura
    temperature=analogRead(36)*(100.0/4095.0);
    
    //Conversion valor a Char Array
    char tempString[8];
    dtostrf(temperature,1,2,tempString);
    Serial.print("Temperatura: ");
    Serial.println(tempString);
    client.publish("esp32/temperature",tempString); //TOPICO esp32/temperature

    //Humedad
    humidity=289.78;

    //Conversion valor a Char Array
    char humString[8];
    dtostrf(humidity,1,2,humString);
    Serial.print("humedad: ");
    Serial.println(tempString);
    client.publish("esp32/humidity",humString); //TOPICO esp32/humidity
  }
}
