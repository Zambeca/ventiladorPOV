
#define CANTIDAD_ELEMENTOS(x) (sizeof (x) / sizeof (*(x)))

int zambeca[] = {
  1,1,0,0,0,0,1,
  1,1,1,0,0,0,1,
  1,0,0,1,0,0,1,
  1,0,0,0,1,1,1,
  1,0,0,0,0,1,1,
  0,0,0,0,0,0,0,
  1,1,1,1,1,1,0,
  0,0,0,1,0,0,1,
  0,0,0,1,0,0,1,
  1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,1,0,0,
  1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,
  1,0,0,1,0,0,1,
  1,0,0,1,0,0,1,
  0,1,1,0,1,1,0,
  0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,
  1,1,0,1,0,1,1,
  1,1,0,1,0,1,1,
  1,1,0,1,0,1,1,
  0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,
  1,1,0,0,0,1,1,
  1,0,0,0,0,0,1,
  1,0,0,0,0,0,1,
  0,0,0,0,0,0,0,
  1,1,1,1,1,1,0,
  0,0,0,1,0,0,1,
  0,0,0,1,0,0,1,
  1,1,1,1,1,1,0,
  0,0,0,0,0,0,0
};

#define NUMERO_LEDS 7

int ledArray[NUMERO_LEDS] = {8,7,6,5,4,3,2};

int irPin=0;

float filaDelay=1200; //En microsegundos, con 3500 se puede ver si uno da vueltas con la mano

int totalElementos = 0;

unsigned long tiempoActual=0;
unsigned long tiempoPasado=0;

int totalFilas=0;
int j=0;
int i=0;

void setup() { 

  for(int i = 0; i < NUMERO_LEDS; i++){
    pinMode(ledArray[i],OUTPUT);
  }
  
  Serial.begin(9600);
  Serial.print("www.zambeca.cl");
  
  totalElementos = CANTIDAD_ELEMENTOS(zambeca);
  totalFilas = totalElementos/NUMERO_LEDS;
 
}

unsigned long tiempoIRActual=0;
unsigned long tiempoIRPasado=0;

enum IR {
  BLANCO,
  NEGRO
} estadoIR=BLANCO;

unsigned long delayIR=500; //microsegundos
unsigned long tiempoIRUltimaVuelta=0;

void loop() {

  tiempoActual=micros();
  tiempoIRActual=tiempoActual;

  if(((tiempoIRActual-tiempoIRPasado)>delayIR)) { 
    int ir=analogRead(irPin);

    if((estadoIR==BLANCO) && (ir > 800)) { //llegue a la marca
      estadoIR=NEGRO;
      float rpmicros=1.0/(tiempoIRActual-tiempoIRUltimaVuelta); //vueltas por micro segundo
      
      //Actualizo el filaDelay de acuerdo a la velocidad real;

      //Quiero 2 mm de ancho de letra
      filaDelay=0.002/(2.0*3.1415*0.08*rpmicros);

      tiempoIRUltimaVuelta=tiempoIRActual;
    }
    if((estadoIR==NEGRO) && (ir < 800)) { //llegue a la marca
      estadoIR=BLANCO;
    }
    tiempoIRPasado=tiempoIRActual;
  }

  if((tiempoActual-tiempoPasado)>(filaDelay)){
    
    if(j<totalFilas){
      for(int i = 0; i<NUMERO_LEDS; i++) {
	digitalWrite(ledArray[i], zambeca[i+j*NUMERO_LEDS]);
      }
      j++;
    }else {
      j=0;
      delayMicroseconds(3*filaDelay); //dejo un espacio de tres columnas
    }

    tiempoPasado=tiempoActual;
  }
  

}


