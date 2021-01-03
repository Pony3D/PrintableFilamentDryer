/*###################################################################################################################################
 *  Programa realizado por Damian Martinez Sanchez para el secador de filamento imprimible.
 *  Modificar variables de <CONFIGURACION MANUAL> en funcion del sistema de PCB y sensores utilizados para el secador.
 *  Diseñado para Arduino NANO v.3
 ###################################################################################################################################*/
 
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1); // Parametros pantalla Oled

const unsigned char bmp1 [] PROGMEM = {
  0x00, 0x00, 0x07, 0xe0, 0x07, 0xe0, 0x01, 0x80, 0x01, 0x80, 0x30, 0x0c, 0x30, 0x0c, 0x3d, 0xbc, 
  0x3d, 0xbc, 0x30, 0x0c, 0x31, 0x8c, 0x01, 0x80, 0x07, 0xe0, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bmp2 [] PROGMEM = {
  0x00, 0x00, 0x08, 0x10, 0x18, 0x18, 0x38, 0x1c, 0x7c, 0x3e, 0x0c, 0x30, 0x00, 0x00, 0x01, 0x80, 
  0x01, 0x80, 0x00, 0x00, 0x0c, 0x30, 0x7c, 0x3e, 0x38, 0x1c, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00
};


const int DHTPIN = 4; //Sensor de Lecturas DHT
const int relay = 2; // Rele de la Resistencia termica de 12v
const int fan = 5; // Ventilador PCB 2
const int coldled = 8; //Led Frio - B
const int warmled = 7; //Led Caliente - A
const int store = 11; //Boton Almacenar - G
const int Bmenos = 10; //Boton menos - F
const int Bmas = 9; //Boton mas - E



//########################################################################################
//################################ <CONFIGURACION MANUAL> ################################
//########################################################################################

 
#define DHTTYPE DHT22 // Definir tipo de sensor DHT11 o DHT22
int AONOFF = 1; // 1 = Ventilador encendido 100% // 0 = Ventilador encendido al 50%
int Tmax = 55; // Temperatura maxima (excederse en la temperatura maxima podria dañar componentes expuestos al calor directo).
int Tmin = 23; // Temperatura minima (no menos a la temperatura ambiente).


/////////////////////////////// <FIN CONFIGURACION MANUAL> ///////////////////////////////

 
DHT dht(DHTPIN, DHTTYPE);
float TMP = dht.readTemperature();
float HUM = dht.readHumidity();
int Hcontrol = 12; // Humedad de secado por defecto (El filamento debe estar entre un 10 y 13% de humedad)
float T = 2.55;  // Multiplicador velocidad del ventilador de 12v.
int pulso;
int ultimo_pulso;
int Tcontrol = EEPROM.read(3);
int Tbucle = 0; // Control de bucle principal
int letargo = 0; // Control de bucle secundario

void setup() {

  Serial.begin(9600);
  dht.begin();
  pinMode(fan, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(coldled, OUTPUT);
  pinMode(warmled, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(store, INPUT);
  pinMode(Bmenos, INPUT);
  pinMode(Bmas, INPUT);
  
// Iniciar Oled, si falla, mostrar error. Repetir bucle. Led alarma 2 destellos por segundo.
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se encuentra la pantalla");
    for(;;);
 }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  display.println("Cargando variables...");
  display.display();
  delay(500);

// Comprobar si esta grabado el control de arranque (posicion 0 de la memoria), si no, grabar variables en memoria.

  if (EEPROM.read(0) != 1) { // Control de arranque para grabar por primera y unica vez las variables cuando se instale el programa
      EEPROM.write(0, 0); 
      EEPROM.put(1, AONOFF);
      EEPROM.put(3, 30);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 25);
  display.println("Leyendo sensores...");
  display.display();  
  delay(500);

// Comprobar lecturas del DHT. Si falla mostrar error, repetir bucle. Led alarma 1 destello por segundo.  
 
  if (isnan(dht.readTemperature()) || isnan(dht.readHumidity())) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 35);
  display.println("Error al leer DHT.");
  display.display(); 
   for(;;); // Si falla la lectura detiene el programa
  } else {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 35);
  display.println("DHT OK.");
  display.display();  
  delay(1000);
  }
}

void loop() {
  
  display.clearDisplay();

//###########################################################################
//################ INICIAR DE LECTURAS ###############################
//###########################################################################


    TMP = dht.readTemperature();
    HUM = dht.readHumidity();
    
//////////////////// FIN CONTADOR DE LECTURAS /////////////////////


//##########################################################################################
//############## INICIANDO PANTALLA PRINCIPAL OLED 128x64 2 zonas Bicolor ##################
//##########################################################################################
  if (HUM >= Hcontrol) {
   displaymenu();
   secando1();
  }
  else {
   displaymenu();
   secado();
  }
  
///////////////////////////// FIN Parte.1 PANTALLA PRINCIPAL /////////////////////////////

//########################################################################################## 
//######################### INICIANDO CONTROL AUTOMATICO DE SECADO #########################
//##########################################################################################

 if (HUM >= Hcontrol) { 
  digitalWrite(coldled, HIGH);
    if (AONOFF == 1) { // Determina la velocidad del ventilador, al 100% o al 50%, dependiendo del voltaje aplicado al BC337.
     digitalWrite(fan, HIGH);
  } else {
     digitalWrite(fan, LOW);
}
 }
 else if (HUM <= Hcontrol) {
  digitalWrite(coldled, LOW);
  digitalWrite(warmled, HIGH);
  digitalWrite(relay, LOW);
 }

 
  if (TMP >= Tmax) {
    digitalWrite(relay, LOW);
     } else {
     digitalWrite(relay, HIGH);
 }
 
  Serial.println(pulso);
  Serial.println(HUM);
  Serial.println(EEPROM.read(1));
 
///////////////////////////// FIN CONTROL AUTOMATICO DE SECADO /////////////////////////////////

//#######################################################################################
//######################### CONTROL DE ENTRADA DE MENUS #################################
//#######################################################################################

if (digitalRead(Bmas) == 1) { Tbucle = 1; }
if (digitalRead(Bmenos) == 1) { Tbucle = 1; }
if (digitalRead(store) == 1) { Tbucle = 3; } 
   
///////////////////// SUBIR TEMPERATURA ////////////////////////////

if (Tbucle == 1) { // Bucle mantener control de menus de temperatura en pantalla
  while (Tbucle == 1) {
  display.clearDisplay();
  
if (digitalRead(Bmas) == 1) { 
  while (digitalRead(Bmas) == 1) { letargo = 2; }
   while (letargo == 2) { // Bucle mantener menu subir temperatura
     
  if (digitalRead(Bmas) == 1) {
    while(digitalRead(Bmas) == 1){
  display.clearDisplay();
  display.fillTriangle(30, 18, 10, 58, 50, 58, SSD1306_WHITE);
  displaytemp();
  display.display();
      }
      if (Tcontrol < Tmax) {
           ++Tcontrol;
        }
   }
  else if (digitalRead(Bmas) == 0) {
  display.clearDisplay();
  display.drawTriangle(30, 18, 10, 58, 50, 58, SSD1306_WHITE);
  displaytemp();
  display.display(); 
   }
   if (digitalRead(Bmenos) == 1) { letargo = 1; } // Si se activa, salir del bucle subir y saltar al bucle bajar
   if (digitalRead(store) == 1) {
    grabardatos();
    letargo = 0;
    Tbucle = 0;
    }
  } // Fin bucle mantener menu subir temperatura 
}

///////////////////// BAJAR TEMPERATURA ////////////////////////////
    
if (digitalRead(Bmenos) == 1) {
  while (digitalRead(Bmenos) == 1) { letargo = 1; }
  while (letargo == 1) {
      
  if (digitalRead(Bmenos) == 1) {
    while(digitalRead(Bmenos) == 1){
  display.clearDisplay();
  display.fillTriangle(10, 18, 50, 18, 30, 58, SSD1306_WHITE);
  displaytemp();
  display.display();
      }
       if (Tcontrol > Tmin) {
            --Tcontrol;
        }  
   }
  else if (digitalRead(Bmenos) == 0) {
  display.clearDisplay();
  display.drawTriangle(10, 18, 50, 18, 30, 58, SSD1306_WHITE);
  displaytemp();
  display.display(); 
   }
   if (digitalRead(Bmas) == 1) { letargo = 2; }
   if (digitalRead(store) == 1) {
    grabardatos();
    letargo = 0;
    Tbucle = 0;
    }
  }
 }
}
}

///////////////////// FIN MENU CONTROL TEMPERATURA ////////////////////////////

////////////////////////// MENU CONFIGURACION VENTILADOR //////////////////////////////

if (Tbucle == 3) {
  while (digitalRead(store) == 1) {} 
  while (Tbucle == 3) { // Bucle mantener menu FAN en pantalla
      display.clearDisplay();
         menufan();
         display.display();
  if (digitalRead(Bmas) == 1) {
    while (digitalRead(Bmas) == 1) {} // Retorno del boton
           if (AONOFF < 1) {
            ++AONOFF;     
        }
        else { --AONOFF; }
       Serial.println(AONOFF);
  }

if (digitalRead(store) == 1) {
  while(digitalRead(store) == 1) {} // Retorno del boton
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5,50);
    display.println("Grabando EEPROM...");
    display.display();
    EEPROM.update(1, AONOFF);
    delay(2000);
    display.clearDisplay();
      Tbucle = 0;
   }
  }
 }

////////////////////////////// FIN MENU VENTILADOR //////////////////////////////////

  if (HUM >= Hcontrol) {
   displaymenu();
   secando2();
  }
  else {
   displaymenu();
   secado();
  }

 display.clearDisplay();
 
} // FIN DEL PROGRAMA

void grabardatos() {
  while (digitalRead(store) == 1) {}
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("EEPROM.");
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.print("Almacenada ");
    display.print("Temp. ");
    display.print(Tcontrol);
    display.cp437(true);
    display.write(167);
    display.display();
   EEPROM.update(3, Tcontrol);
  delay(2000);
  display.clearDisplay();
 }

void displaytemp() {
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Menu Temp.");
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(70, 30);
  display.println(Tcontrol);
}

void displaymenu() {
  display.clearDisplay();
  display.drawRect(0, 16, 128, 48, SSD1306_WHITE);
  display.drawLine(0, 39, 128, 39, SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5,20);
  display.print("Temp ");
  display.print(TMP);
//  display.cp437(true);
//  display.write(167);
  display.setCursor(5,43);
  display.print("Hume ");
  display.print(HUM);
 // display.cp437(true);
 // display.write(37);
}


void secando1() {
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Secando");
  display.drawBitmap(112, 0, bmp1, 16, 16, SSD1306_WHITE);
  display.display();
  delay(250);
}

void secando2() {
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Secando");
  display.drawBitmap(112, 0, bmp2, 16, 16, SSD1306_WHITE);
  display.display();
  delay(250);
}

void secado() {
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Secado OK.");
  display.display();
  delay(10);
}

void menufan() {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Ventilador");
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,25);
    display.print("Speed ");
    if (AONOFF == 1) {
    display.print("50");
    display.cp437(true);
    display.write(37);
    }
    else {
    display.print("100");
    display.cp437(true);
    display.write(37);
      }
}
