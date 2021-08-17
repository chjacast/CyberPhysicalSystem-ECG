//Definimos la variable en donde vamos a almacenar nuestros tiempos:
unsigned long tiempo;
unsigned long tiempo2=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
tiempo=millis();


if(tiempo-tiempo2>=2000){
  tiempo2=tiempo;
  Serial.print("Tiempo transcurrido: ");
  Serial.println(tiempo);
  }


}
