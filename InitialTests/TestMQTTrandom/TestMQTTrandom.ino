//SE INCLUYEN LAS LIBRERIAS
#include <WiFi.h>
#include <PubSubClient.h> //Libreria enfocada a la comunicacion  mqtt

//Creamos la instancia cliente
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg =0;
//char msg[50]; 

//Valor 
int numaleatorio;

//USUARIO PARA CONECTARNOS AL SERVIDOR******************
const char* ssid= "TVORO_CASTILLO"; //Nombre del WiFi
const char* password= "0701088775"; //Contraseña del WiFi

//DIRECCION IP del MQTT Broker
const char* mqtt_server = "192.168.1.27";
//SETUP WIFI-----------------------------------------------------
//Permite la conexion del esp32 a internet y devuelve la ip proporcionada por el router
void setup_wifi(){
  delay(100); 
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
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando mediciones");  // texto a mostrar
  setup_wifi();
  client.setServer(mqtt_server,1883); //Permitir la conexion con el broker

}

//RECONNECT-----------------------------------------------------
//Para observar si mi conexion MQTT es correcta
void reconnect(){
  while(!client.connected()){
    Serial.print("Intentando establecer conexion MQTT con el servidor...");
    String clientId ="Esp32Client-";
    clientId+=String(random(0xffff),HEX);
    if(client.connect(clientId.c_str())){
      Serial.print("Conectado a: ");
      Serial.println(mqtt_server);
      //Se escriben todas los subtopicos a subscribirse
      //client.subscribe("esp32/output1"); //TOPICO output1
      //client.subscribe("esp32/output2"); //TOPICO output2
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
void loop() {
  if(!client.connected()){
    reconnect();
    }
  client.loop();
  
  //Realizamos el envio de datos cada n milisegundos
  long now = millis();
  if(now-lastMsg > 50){ //50 ms (tiempo de muestreo = n)
    lastMsg=now;
    numaleatorio= random(1,20);
    char numrandom[8]; //Encargada de enviar valores
    
    dtostrf(numaleatorio,1,2,numrandom);
    Serial.print("Numero: "); 
    Serial.println(numrandom);
    client.publish("esp32/valor",numrandom);
  }


  
  Serial.print("Hola mundo\n");
  delay(1000);
}
