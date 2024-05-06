// Paso 1

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define IZDA_PIN 14
#define ROTAR_PIN 12
#define DCHA_PIN 13
#define ABAJO_PIN 26
#define SOLTAR_PIN 25

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, U8X8_PIN_NONE);

void nuevaPartida();

void setup() {
    // Inicializar pantalla
    u8g2.begin();      

    // Inicializar botonera
    pinMode(IZDA_PIN, INPUT_PULLUP);
    pinMode(ROTAR_PIN, INPUT_PULLUP);
    pinMode(DCHA_PIN, INPUT_PULLUP);
    pinMode(ABAJO_PIN, INPUT_PULLUP);
    pinMode(SOLTAR_PIN, INPUT_PULLUP);

    // Inicializar generador aleatorio
    randomSeed(analogRead(0));  

    // Comenzar partida
    nuevaPartida();
}

void loop() {
    // Vaciar pantalla
    u8g2.clearBuffer();

    // Dibujar pantalla
    u8g2.sendBuffer();

    delay(50);
}

void nuevaPartida() {
}