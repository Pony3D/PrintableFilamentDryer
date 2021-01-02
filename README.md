# Secador de Filamento Electronico.

Version imprimible de un secador de filamento para impresora 3D.
Modelo optimizado para imprimir en impresora tamaño standar con cama de 220x220, como Ender3.
Es necesario Arduino NANO y conocimientos basicos de electronica.

¿Como funciona?

El secador consta de varias partes que se unen entre si mediante tornillos, ademas de una parte electronica programada en Arduino.
Es recomendable utilizar cables de 1 a 1.3mm maximo de diametro. La electronica es muy simple y cada cable o componente esta marcado con letras y posiciones.

Si se mantiene pulsado el boton +, o el boton -, durante un segundo, se entra en el menu de temperatura, donde se puede configurar la temperatura maxima a alcanzar.
Pulsando repetidas veces sobre + o -, selecionamos la temperatura y pulsando el boton S (Almacenar o Stored), se graba la temperatura de control en la memoria, para el proximo arranque.
El elemento calefactable y el ventilador se encienden si la humedad no esta por debajo del 12%, en el momento que se excede la temperatura maxima grabada en la memoria el elemento calefactable se apaga, no el ventilador.
En el momento que se alcanza un valor de humedad del 12% o inferior, el ventilador y el elemento calefactable se detienen y se muestra, en pantalla, el mensaje de "Secado OK, o DRY OK", y el filamento esta listo para imprimir.
La potencia del elemento calefactable es baja, entorno 14W, por lo que el tiempo de secado del filamento puede ser entorno las 4 horas.
Es recomendable tapar los agujeros exteriores de los tornillos para mantener la temperatura y humedad correcta del interior.


Componentes necesarios:
- Adaptador de corriente 220v/12v (1.5 a 3A max). https://es.aliexpress.com/item/32859196804.html
- Elemento calefactable de polyamida (12v 14W). https://es.aliexpress.com/item/4000603792416.html
- Interruptor. https://es.aliexpress.com/item/32627544552.html
- Rodamientos de bolas MR106ZZ (6x10x3). https://es.aliexpress.com/item/4000013719586.html
- Ventilador silencioso 40x40x10 12v: https://www.amazon.es/gp/product/B01HTFG9RG/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1
- 0.96" I2C OLED 2 colores. https://es.aliexpress.com/item/1005001392369794.html
- DHT22 (No utilizar DHT11). https://es.aliexpress.com/item/32716996619.html
- Arduino NANO. https://es.aliexpress.com/item/32788387125.html
- Leds de colores: https://es.aliexpress.com/item/32240262628.html
- Conector DC005: https://es.aliexpress.com/item/4000034340966.html
- Rele 5vdc Arduino (sin PCB): https://es.aliexpress.com/item/33032279721.html
- Pulsadores largos: https://es.aliexpress.com/item/32857436208.html



Componentes electronicos PCB 3:
- Power led: 1 Lila o Verde, color a elegir.
- Resistencia: 1K (x2)
- Resistencia: 10K (x2)
- Resistencia: 470 Ohms (x1)
- Resistencia: 150 Ohms (x1)
- Diodo: 14N007 (x3)
- Transistor: BC328 (x1)
- Transistor: BC337 (x1)
- Mosfet: 7805 (x1)
- Condensador ceramico: 100nf (x2)
- Condensador polarizado: 100uf (x3)
- Rele (tipico arduino) 5v 10A.

Componentes electronicos Panel de Control:
- Leds: x1 Rojo, x1 Naranja, x1 Azul.
- Resistencia: 220 Ohms (x2)
- Resistencia: 10 Ohms (x3)
- Pulsadores: (x3)
- Pantalla OLED 0.96": (x1)

LEDS Panel de Control:
- COLD Led: Azul
- HOT Led: Rojo
- LED3 (thermal led): Naranja




Codigo arduino en Español y Ingles, archivos Gerber y Fritzing, diagramas electronicos, componentes necesarios y mas:
https://github.com/Pony3D/PrintableFilamentDryer


¿Donde fabricar las placas PCB?: 
Por ejemplo; https://jlcpcb.com/

ESTE ES UN PROYECTO OPEN SOURCE, POR LO QUE CUALQUIER ELEMENTO ELECTRONICO O DE PROGRAMACION PUEDE SER MODIFICADO Y MEJORADO, EN REALIDAD, ESPERO QUE LO MEJOREIS ;)

NOTA: Las fotografias del modelo expuesta representan un prototipo sobre el que se hicieron las ultimas mejoras, puede ser distinto del modelo final.





///////////////////////////////// ENGLISH TRANSLATION //////////////////////////////////////

# Electronic Filament Dryer.

Printable version of a filament dryer for 3D printer.
Model optimized for printing on a standard size printer with a 220x220 bed, such as Ender3.
Arduino NANO and basic knowledge of electronics are necessary.

How does it work?

The dryer consists of several parts that are joined together by screws, in addition to an electronic part programmed in Arduino.
It is recommended to use cables with a maximum diameter of 1 to 1.3mm. The electronics is very simple and each wire or component is marked with letters and positions.

If you hold down the + button, or the - button, for one second, you enter the temperature menu, where you can set the maximum temperature to be reached.
Pressing repeatedly on + or -, we select the temperature and pressing the S button (Store or Stored), the control temperature is recorded in the memory, for the next start.
The heating element and the fan turn on if the humidity is not below 12%, when the maximum temperature recorded in the memory is exceeded, the heating element turns off, not the fan.
As soon as a humidity value of 12% or less is reached, the fan and the heating element stop and the message "Drying OK, or DRY OK" is displayed on the screen, and the filament is ready to print. .
The power of the heating element is low, around 14W, so the drying time of the filament can be around 4 hours.
It is recommended to cover the outer screw holes to maintain the correct temperature and humidity inside.


Required components:
- 220v / 12v power adapter (1.5 to 3A max). https://es.aliexpress.com/item/32859196804.html
- Polyamide heating element (12v 14W). https://es.aliexpress.com/item/4000603792416.html
- Switch. https://es.aliexpress.com/item/32627544552.html
- MR106ZZ ball bearings (6x10x3). https://es.aliexpress.com/item/4000013719586.html
- Silent fan 40x40x10 12v: https://www.amazon.es/gp/product/B01HTFG9RG/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1
- 0.96 "I2C OLED 2 colors. Https://es.aliexpress.com/item/1005001392369794.html
- DHT22 (Do not use DHT11). https://es.aliexpress.com/item/32716996619.html
- Arduino NANO. https://es.aliexpress.com/item/32788387125.html
- Color LEDs: https://es.aliexpress.com/item/32240262628.html
- DC005 connector: https://es.aliexpress.com/item/4000034340966.html
- 5vdc Arduino relay (without PCB): https://es.aliexpress.com/item/33032279721.html
- Long pushbuttons: https://es.aliexpress.com/item/32857436208.html



Electronic components PCB 3:
- Power led: 1 Purple or Green, color to choose.
- Resistance: 1K (x2)
- Resistance: 10K (x2)
- Resistance: 470 Ohms (x1)
- Resistance: 150 Ohms (x1)
- Diode: 14N007 (x3)
- Transistor: BC328 (x1)
- Transistor: BC337 (x1)
- Mosfet: 7805 (x1)
- Ceramic capacitor: 100nf (x2)
- Polarized capacitor: 100uf (x3)
- Relay (typical arduino) 5v 10A.

Electronic components Control Panel:
- Leds: x1 Red, x1 Orange, x1 Blue.
- Resistance: 220 Ohms (x2)
- Resistance: 10 Ohms (x3)
- Buttons: (x3)
- 0.96 "OLED screen: (x1)

LEDS Control Panel:
- COLD Led: Blue
- HOT Led: Red
- LED3 (thermal led): Orange




Arduino code in Spanish and English, Gerber and Fritzing files, electronic diagrams, necessary components and more:
https://github.com/Pony3D/PrintableFilamentDryer


Where to make PCB boards ?:
For example; https://jlcpcb.com/

THIS IS AN OPEN SOURCE PROJECT, SO ANY ELECTRONIC OR PROGRAMMING ELEMENTS CAN BE MODIFIED AND IMPROVED, IN REALITY, I HOPE YOU WILL IMPROVE IT;)

NOTE: The photographs of the exposed model represent a prototype on which the last improvements were made, it may be different from the final model.
