// Paso 3

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define IZDA_PIN 14
#define ROTAR_PIN 12
#define DCHA_PIN 13
#define ABAJO_PIN 26
#define SOLTAR_PIN 25

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, U8X8_PIN_NONE);

uint8_t pozo[10][20] = {};

const uint8_t piezas[7][4][4][2] = {
    {
            // I
            {{1,0},{1,1},{1,2},{1,3}},
            {{0,1},{1,1},{2,1},{3,1}},
            {{1,0},{1,1},{1,2},{1,3}},
            {{0,1},{1,1},{2,1},{3,1}}
    },
    {
            // J
            {{0,2},{1,0},{1,1},{1,2}},
            {{0,1},{1,1},{2,1},{2,2}},
            {{1,0},{1,1},{1,2},{2,0}},
            {{0,0},{0,1},{1,1},{2,1}}
    },
    {
            // L
            {{1,0},{1,1},{1,2},{2,2}},
            {{0,1},{1,1},{2,0},{2,1}},
            {{0,0},{1,0},{1,1},{1,2}},
            {{0,1},{0,2},{1,1},{2,1}}
    },
    {
            // O
            {{0,0},{0,1},{1,0},{1,1}},
            {{0,0},{0,1},{1,0},{1,1}},
            {{0,0},{0,1},{1,0},{1,1}},
            {{0,0},{0,1},{1,0},{1,1}}
    },
    {
            // S
            {{0,1},{1,0},{1,1},{2,0}},
            {{0,0},{0,1},{1,1},{1,2}},
            {{0,2},{1,1},{1,2},{2,1}},
            {{1,0},{1,1},{2,1},{2,2}}
    },
    {
            // T
            {{0,1},{1,1},{1,2},{2,1}},
            {{1,0},{1,1},{1,2},{2,1}},
            {{0,1},{1,0},{1,1},{2,1}},
            {{0,1},{1,0},{1,1},{1,2}}
    },
    {
            // Z
            {{0,0},{1,0},{1,1},{2,1}},
            {{0,1},{0,2},{1,0},{1,1}},
            {{0,1},{1,1},{1,2},{2,2}},
            {{1,1},{1,2},{2,0},{2,1}}
    }
};

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

void dibujaBloque(int x, int y, int tipo) {
    u8g2.drawFrame(x, y, 6, 6);
    if (tipo == 0) {
        u8g2.drawBox(x + 2, y + 2, 2, 2);
    } else {
        for (int i = 1; i <= 4; i++) {
            u8g2.drawPixel(x + 1 + i % 2, y + i);
            u8g2.drawPixel(x + 3 + i % 2, y + i);
        }
    }
}

void dibujaPieza(int columna, int fila, int pieza, int rotacion) {
    int x = 2 + columna * 6;
    int y = 5 + fila * 6;
    for (int i = 0; i < 4; i++) {
        dibujaBloque(x + piezas[pieza][rotacion][i][0] * 6, y + piezas[pieza][rotacion][i][1] * 6, 0);
    }
}

void limpiaTablero() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
          pozo[i][j] = 0;
        }
    }
}

void dibujaFondo() {
    // Dibujar marco
    u8g2.drawFrame(0, 3, 64, 124);
}

void nuevaPartida() {
    limpiaTablero();
}

void loop() {
    // Vaciar pantalla
    u8g2.clearBuffer();

    // Dibujar marco de fondo
    dibujaFondo();
    
    // Dibujar puntuación
    u8g2.setFont(u8g2_font_u8glib_4_hf);
    u8g2.drawStr(0, 4, "Puntos: 0 (0)");

    // Dibujar pieza prueba
    dibujaPieza(0, 0, 2, 1);

    // Dibujar pantalla
    u8g2.sendBuffer();

    delay(50);
}