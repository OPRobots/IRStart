#include <IRremote.h>
#include <EEPROM.h>

const int RECV_PIN = 1;

IRrecv irrecv(RECV_PIN);
decode_results results;

#define SALIDA 0
#define BOTON 2
#define LED_ON 3
#define LED_OFF 4

bool lectura_boton = 0;
bool modulo_ready = 0;
bool salida = 0;
byte contador = 0;
unsigned long comando_ready = 0;
unsigned long comando_on = 0;
unsigned long comando_off = 0;
unsigned long recivido = 0;

void setup()
{
  pinMode(SALIDA, OUTPUT);
  pinMode(BOTON, INPUT);
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_OFF, OUTPUT);
  irrecv.enableIRIn();

  EEPROM.get(100, comando_ready);
  EEPROM.get(200, comando_on);
  EEPROM.get(300, comando_off);
}

void loop() {
  lectura_boton = digitalRead(BOTON);
  if (irrecv.decode(&results)) {
    recivido = (results.value);
    if (recivido == 4294967295) {
      recivido = 0;
    }
    irrecv.resume();
  }


  if (lectura_boton == true) {
    delay(30000);    // Este delay tan alto es debidoa que en attiny no conseguí hacer funcionar "millis()" y los delays duran 30 veces menos.
    if (digitalRead(BOTON) == true) {
      contador = 1;
    }
  }

switch (contador) {

case 0:  //Codigo de funcionamiento encender y apagar con el mando normalmente

  if (modulo_ready == false) {
    digitalWrite(LED_ON, HIGH);
    digitalWrite(LED_OFF, HIGH);
    digitalWrite(SALIDA, LOW);
  } else if (salida == true) {
    digitalWrite(SALIDA, HIGH);
    digitalWrite(LED_ON, HIGH);
    digitalWrite(LED_OFF, LOW);
  } else if (salida == false) {
    digitalWrite(SALIDA, LOW);
    digitalWrite(LED_ON, LOW);
    digitalWrite(LED_OFF, HIGH);
  }

  if (recivido > 0) {
    if (comando_ready == recivido) {
      modulo_ready = !modulo_ready;
      salida = false;
    } else if (modulo_ready == true && comando_off == recivido) {
      salida = false;
    } else if (modulo_ready == true && comando_on == recivido) {
      salida = true;
    }
  }
  break;
case 1:  //Del case 1 en adelante codigo de grabado de señal de mando
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(20000);
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(20000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(20000);
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(20000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(20000);
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(20000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(20000);
  contador = 2;
  break;

case 2:
  if (recivido > 0 ) {
    comando_ready = recivido;
    EEPROM.put( 100, recivido );
    recivido = 0;
    contador = 3;
  }
  break;
case 3:
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(50000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  contador = 4;
  break;
case 4:
  if (recivido > 0 && recivido != comando_ready) {
    comando_on = recivido;
    EEPROM.put( 200, recivido );
    recivido = 0;
    contador = 5;
  }
  break;
case 5:
  digitalWrite(LED_ON, HIGH);
  delay(50000);
  digitalWrite(LED_ON, LOW);
  contador = 6;
  break;
case 6:
  if (recivido > 0 && recivido != comando_ready && recivido != comando_on) {
    comando_off = recivido;
    EEPROM.put( 300, recivido );
    recivido = 0;
    contador = 7;
  }
  break;
case 7:
  digitalWrite(LED_OFF, HIGH);
  delay(50000);
  digitalWrite(LED_OFF, LOW);
  contador = 8;
  break;
case 8:
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(8000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(8000);
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(8000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(8000);
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(8000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(8000);
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, HIGH);
  delay(8000);
  digitalWrite(LED_ON, LOW);
  digitalWrite(LED_OFF, LOW);
  delay(8000);
  modulo_ready == false;
  contador = 0;
  break;
}

recivido = 0;
}

