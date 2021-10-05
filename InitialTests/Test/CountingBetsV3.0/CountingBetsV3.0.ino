#define factor 0.85
float maximo = 0.0;   // para almacenar valor maximo 
int minimoEntreLatidos = 300; // 300 mseg. de tiempo minimo entre latidos
float valorAnterior = 500;  // para almacenar valor previo
int latidos = 0;    // contador de cantidad de latidos
int valorLeido;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando mediciones");  // texto a mostrar
  
  //Detecciones de referencia de electrodos
  pinMode(2, INPUT); //Se configura detección de LO-
  pinMode(4, INPUT); //Se configura detección de LO+
}

void loop() {
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
    Serial.println("Conecte los electrodos porfavor");
    delay(500);
    }
  else{
          if ((cambio >= maximo) && (millis() > entreLatidos + minimoEntreLatidos)) {  // si cambio es
          // es mayor o igual a maximo y pasaron al menos 300 mseg.
    maximo = cambio;      // actualiza maximo con valor de cambo
    entreLatidos = millis();    // actualiza variable entreLatidos con millis()
    latidos++;        // incrementa latidos en uno
  }
  else {        // condicion falsa del condicional
  }
  
  maximo = maximo * 0.9;   // carga maximo como el 97 por ciento de su propio
          // valor para dejar decaer y no perder pulsos

  if (millis() >= tiempoLPM + 5000) {    // si transcurrieron al menos 5 segundos
    Serial.print("Latidos por minuto: "); // muestra texto
    Serial.println(latidos*12);    // muestra variable latidos multiplicado por 12
    latidos = 0;        // coloca contador de latidos en cero
    tiempoLPM = millis();     // actualiza variable con valor de millis()
  }

  delay(50);        // demora entre lecturas de entrada analogica
  

}  
}
