//SE INCLUYEN LAS LIBRERIAS
#include <WiFi.h>
#include <PubSubClient.h> //Libreria enfocada a la comunicacion  mqtt
#include <Wire.h>

//Se definen las variables para la deteccion de pulsos
#define factor 0.85
float maximo = 0.0;   // para almacenar valor maximo 
int minimoEntreLatidos = 300; // 300 mseg. de tiempo minimo entre latidos
float valorAnterior = 500;  // para almacenar valor previo
int latidos = 0;    // contador de cantidad de latidos
int valorLeido;
long valorEnviar;

//Creamos la instancia cliente
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg =0;
char msg[50]; 
int value=0;


//USUARIO PARA CONECTARNOS AL SERVIDOR******************
const char* ssid= "TVORO_CASTILLO"; //Nombre del WiFi
const char* password= "0701088775"; //Contraseña del WiFi

//DIRECCION IP del MQTT Broker
const char* mqtt_server = "192.168.1.27";
//const uint16_t mqtt_port =1883;


void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando mediciones");  // texto a mostrar
  setup_wifi();
  client.setServer(mqtt_server,1883); //Permitir la conexion con el broker
  //client.setCallback(callback);//En caso de permitir una suscripcion, recibe la trama de datos por el callback
  
  //Detecciones de referencia de electrodos (Nos permite saber si los electrodos se encuentran conectados)
  pinMode(2, INPUT); //Se configura detección de LO-
  pinMode(4, INPUT); //Se configura detección de LO+
}
//SETUP WIFI-----------------------------------------------------
//Permite la conexion del esp32 a internet y devuelve la ip proporcionada por el router
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
  WiFi.mode(WIFI_STA); //Coriige el error de la conexion al servidor MQTT
}
//SETUP WIFI-----------------------------------------------------
//CALLBACK-----------------------------------------------------
//Realiza la conexion MQTT para recibir datos (Subscriber) (Broker->ESP32)
/*void callback(char* topic, byte* message, unsigned int llength){
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
    //Se agrega las salidas a controlar que se desee
}
*/ 
//CALLBACK-----------------------------------------------------
//RECONNECT-----------------------------------------------------
//Para observar si mi conexion MQTT es correcta
void reconnect(){
  while(!client.connected()){
    Serial.print("Intentando establecer conexion MQTT con el servidor...");
    String clientId ="Esp8266Client-";
    clientId+=String(random(0xffff),HEX);
    if(client.connect("ESP32Client")){
      Serial.print(clientId.c_str());
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

//El void loop REALIZA CONEXION MQTT COMO PUBLICADOR (ESP32->Broker)
void loop() {
  //Verificamos en cada iteracion si nos encontramos conectado al broker
  if(!client.connected()){
    reconnect();
    }
  client.loop();

  //Realizamos el envio de datos cada n milisegundos
  long now = millis();
  if(now-lastMsg >50){ //50 ms (tiempo de muestreo = n)
    lastMsg=now;
    //SEÑALES QUE SE DESEA ENVIAR AL BROKER
    char BeatString[8]; //Encargada de enviar valores RPM
    //char ECGString[8]; //Encargada de enviar la señal del electrocardiograma
    
  static unsigned long tiempoLPM = millis();  // tiempo Latidos Por Minuto con
            // valor actual devuelto por millis()
  static unsigned long entreLatidos = millis(); // tiempo entre Latidos con
            // valor actual devuelto por millis()

  valorLeido = analogRead(15);    // lectura de entrada analogica 15 pin del ESP32
  
  float valorFiltrado = factor * valorAnterior + (1 - factor) * valorLeido; // filtro pasa bajos
  float cambio = valorFiltrado - valorAnterior;   // diferencia entre valor filtrado y
              // valor anterior
  valorAnterior = valorFiltrado;    // actualiza valor anterior con valor filtrado

  int BPM = 0;
  if((digitalRead(2)==1)||(digitalRead(4)==1)){
    //Serial.println("Conecte los electrodos porfavor");
    client.publish("esp32/electrodos","desconectado"); //TOPICO esp32/ECG
    delay(500);
    }
  else{
   //valorEnviar=valorAnterior*(1);
   //dtostrf(valorEnviar,1,2,ECGString);
    //client.publish("esp32/ECG",ECGString); //TOPICO esp32/Estado electrodos
          if ((cambio >= maximo) && (millis() > entreLatidos + minimoEntreLatidos)) {  // si cambio es
          // es mayor o igual a maximo y pasaron al menos 300 mseg.
          maximo = cambio;      // actualiza maximo con valor de cambo
          entreLatidos = millis();    // actualiza variable entreLatidos con millis()
          latidos++;        // incrementa latidos en uno
          client.publish("esp32/electrodos","conectado"); //TOPICO esp32/Estado electrodos
              }
    maximo = maximo * 1;   // carga maximo como el 97 por ciento de su propio
          // valor para dejar decaer y no perder pulsos

    if (millis() >= tiempoLPM + 5000) {    // si transcurrieron al menos 5 segundos
      //Serial.print("Latidos por minuto: "); // muestra texto
      Serial.println(latidos*12);    // muestra variable latidos multiplicado por 12
      dtostrf(latidos*12,1,2,BeatString);
      client.publish("esp32/beats",BeatString); //TOPICO esp32/latidos
      latidos = 0;        // coloca contador de latidos en cero
      tiempoLPM = millis();     // actualiza variable con valor de millis()
        }
      //delay(5);
    }
  }
}
