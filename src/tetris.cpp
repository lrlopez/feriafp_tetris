// Paso 7

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

/// Estado del juego
int pieza;      // Pieza actual
int rotacion;   // Rotación actual
int posFila;    // Fila actual
int posColumna; // Columna actual

int puntos;     // Puntos obtenidos
int numLineas;     // Líneas completadas

unsigned long tiempo; // Último avance de línea
int tiempoCaida;      // Tiempo de avance de línea

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

int puedeColocarseBloque(int columna, int fila) {
  if (columna < 0 || columna > 9) return 0;
  if (fila < 0 || fila > 19) return 0;
  if (pozo[columna][fila] == 1) return 0;
  return 1;
}

int puedeColocarsePieza(int columna, int fila, int pieza, int rotacion) {
  return
    puedeColocarseBloque(columna + piezas[pieza][rotacion][0][0], fila + piezas[pieza][rotacion][0][1]) &&
    puedeColocarseBloque(columna + piezas[pieza][rotacion][1][0], fila + piezas[pieza][rotacion][1][1]) &&
    puedeColocarseBloque(columna + piezas[pieza][rotacion][2][0], fila + piezas[pieza][rotacion][2][1]) &&
    puedeColocarseBloque(columna + piezas[pieza][rotacion][3][0], fila + piezas[pieza][rotacion][3][1]);
}

int pulsadoMoverIzquierda() {
    return digitalRead(IZDA_PIN) == LOW;
}

int pulsadoMoverDerecha() {
    return digitalRead(DCHA_PIN) == LOW;
}

int pulsadoBajar() {
    return digitalRead(ABAJO_PIN) == LOW;
}

int pulsadoRotar() {
    return digitalRead(ROTAR_PIN) == LOW;
}

int pulsadoSoltar() {
    return digitalRead(SOLTAR_PIN) == LOW;
}

void dibujaFondo() {
    // Dibujar marco
    u8g2.drawFrame(0, 3, 64, 124);
}

void dibujaPozo() {
    int fila, columna;
    for (columna = 0; columna < 10; columna++) {
        for (fila = 0; fila < 20; fila++) {
            if (pozo[columna][fila]) {
                int x = 2 + columna * 6;
                int y = 5 + fila * 6;
                dibujaBloque(x, y, 2);
            }
        }
    }
}

void piezaAleatoria() {
  pieza = random(7);
  rotacion = random(4);
  posFila = 0;
  posColumna = 3;

  tiempo = 0;
}

void nuevaPartida() {
  limpiaTablero();
  piezaAleatoria();
  numLineas = 0;
  puntos = 0;
  tiempoCaida = 1000;
}

void colocarPieza(int columna, int fila, int pieza, int rotacion) {
    for (int i = 0; i < 4; i++) {
        pozo[columna + piezas[pieza][rotacion][i][0]][fila + piezas[pieza][rotacion][i][1]] = 1;
    }
}

void comprobarLineas() {
    int lineas = 0;
    int puntosSegunLineas[] = { 0, 100, 300, 500, 800 };

    for (int fila = 19; fila >= 0; fila--) {
        int hayLinea = 1;
        for (int columna = 0; columna < 10; columna++) {
            if (pozo[columna][fila] != 1) {
                hayLinea = 0;
                break;
            }
        }
        if (hayLinea) {
            lineas++;
            // Bajar las líneas del pozo que haya por encima
            for (int filaInicial = fila - 1; filaInicial >= 0; filaInicial--) {
                for (int columna = 0; columna < 10; columna++) {
                    pozo[columna][filaInicial + 1] = pozo[columna][filaInicial];
                }
            }
            for (int columna = 0; columna < 10; columna++) {
                pozo[columna][0] = 0;
            }
            fila++;
        }
    }

    if (lineas > 0) {
        // Sumar puntuación
        puntos += puntosSegunLineas[lineas];
        numLineas += lineas;

        // Acelerar caída según las líneas conseguidas
        tiempoCaida -= lineas * 50;
    }
}

void finDeJuego() {
    u8g2.setFontMode(1);
    u8g2.setFont(u8g2_font_helvR08_tr);
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 54, 64, 26);
    u8g2.setDrawColor(1);
    u8g2.drawButtonUTF8(32, 70, U8G2_BTN_SHADOW1|U8G2_BTN_HCENTER|U8G2_BTN_BW2, 0, 2, 2, "Fin de juego");
    u8g2.sendBuffer();
    u8g2.setFontMode(0);
    delay(5000);
    nuevaPartida();
}

void loop() {
    unsigned long actual = millis();

    // Vaciar pantalla
    u8g2.clearBuffer();

    // Dibujar marco de fondo
    dibujaFondo();

    // Dibujar pozo
    dibujaPozo();
    
    // Dibujar puntuación
    u8g2.setFont(u8g2_font_u8glib_4_hf);
    char texto[40];
    sprintf(texto, "Puntos: %d (%d)", puntos, numLineas);
    u8g2.drawStr(0, 4, texto);

    int ultFila = posFila;
    int ultColumna = posColumna;
    int ultRotacion = rotacion;

    if (pulsadoRotar()) {
        rotacion = (rotacion + 1) % 4;
    }

    if (pulsadoMoverIzquierda()) posColumna--;
    if (pulsadoMoverDerecha()) posColumna++;
    if (pulsadoBajar()) posFila++;

    if (!puedeColocarsePieza(posColumna, posFila, pieza, rotacion)) {
        posFila = ultFila;
        posColumna = ultColumna;
        rotacion = ultRotacion;
    } else {
      if (pulsadoSoltar()) {
          do {
              posFila++;
          } while (puedeColocarsePieza(posColumna, posFila, pieza, rotacion));
          posFila--;
      }
    }

    if (actual > tiempo + tiempoCaida) {
        posFila++;
        tiempo = actual;
        // No puede colocarse al avanzar: fijar en el pozo
        if (!puedeColocarsePieza(posColumna, posFila, pieza, rotacion)) {
            colocarPieza(posColumna, posFila - 1, pieza, rotacion);
            comprobarLineas();
            piezaAleatoria();
            if (!puedeColocarsePieza(posColumna, posFila, pieza, rotacion)) {
                finDeJuego();
            }
        }
    }

    // Dibujar pieza prueba
    dibujaPieza(posColumna, posFila, pieza, rotacion);

    // Dibujar pantalla
    u8g2.sendBuffer();

    delay(5);
}