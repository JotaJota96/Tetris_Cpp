#include "Juego.h"

Juego::Juego(){
    this->tablero = new Tablero(ALTO, ANCHO);
    this->posTablero = new Coordenada(BORDE_IZQUIERDO+2,BORDE_SUPERIOR+1);

    OcultarCursor();
}

void Juego::jugarPartida(){

    cambiarColor(2);
    cout << endl;
    cout << "Informacion: Utilize las siguientes teclas para jugar" << endl;
    cambiarColor(11);
    cout << "- Flechas del teclado para mover las piezas en todas las direcciones." << endl;
    cout << "- 'a' y 'd' para rotar las piezas." << endl;
    cout << "- 's' para fijar la pieza en la posicion actual." << endl;
    cout << "- 'w' para alternar entre las proximas piezas." << endl;
    cout << endl;

    cambiarColor(2);
    cout << "Objetivo del juego:" << endl;
    cambiarColor(11);
    cout << "Lograr dejar el espacio de juego totalmente vacio, es decir, sin ningun resto de piezas" << endl;
    cout << "(Aun no se ha implementado la verificacion de si se logro el objetivo)" << endl;
    cout << endl;

    cambiarColor(15);
    system("PAUSE");
    system("CLS");


    mostrarBordes();
    int puntuacion = 0;
    tablero->nuevaPieza();
    OcultarCursor();

    dibujarPieza();
    mostrarPuntaje(puntuacion);
    mostrarColaDePiezas();

    while (true){
        if (kbhit()){
            char tecla = getch();
            if (tecla == -32) continue;

            borrarPieza();

            switch (tecla) {
            case FLECHA_ARRIBA:
                tablero->piezaMover(ARRIBA);
                break;
            case FLECHA_ABAJO:
                tablero->piezaMover(ABAJO);
                break;
            case FLECHA_DERECHA:
                tablero->piezaMover(DERECHA);
                dibujarPieza();
                break;
            case FLECHA_IZQUIERDA:
                tablero->piezaMover(IZQUIERDA);
                break;
            case TECLA_ROTAR_DERECHA:
                tablero->piezaRotar(true);
                break;
            case TECLA_ROTAR_iZQUIERDA:
                tablero->piezaRotar(false);
                break;
            case TECLA_CAMBIAR_PIEZA:
                tablero->alternarPieza();
                mostrarColaDePiezas();
                break;
            case TECLA_FIJAR_PIEZA:
                dibujarPieza();
                tablero->fijarPieza();

                int lineasEliminadas = tablero->eliminarFilasCompletas();
                /* incremento de puntaje por lineas eliminadas
                 * si se elimina  1 linea:  +2
                 * si se eliminan 2 lineas: +5
                 * si se eliminan 3 lineas: +8
                 * si se eliminan 4 lineas: +13
                 *
                 * Se puede generalizar con la formula:
                 * f(n) = 2n + n - 1
                 * */
                if (lineasEliminadas != 0){
                    puntuacion += lineasEliminadas *2 + lineasEliminadas -1;
                    mostrarTablero();
                    mostrarPuntaje(puntuacion);
                }

                if ( ! tablero->nuevaPieza()){
                    mostrarGameOver();
                    return;
                }
                mostrarColaDePiezas();
                break;
            }
            dibujarPieza();
        }
    }
}

////////////////////////////////////////////////////////////////////
void Juego::dibujarPieza(){
    Coordenada* pCord = this->tablero->getCoordenada();
    Pieza* p = this->tablero->getPieza();
    int dimPieza = p->getDimensiones();

    cambiarColor(p->getTipo());

    for (int y = 0; y < dimPieza; y++){
        for (int x = 0; x < dimPieza; x++){
            if (p->existeEn(x, y)){
                GoToXY(BORDE_IZQUIERDO+2 + (pCord->getX()+x)*2, BORDE_SUPERIOR+1 + pCord->getY()+y);
                printf("%c%c", CHAR_PIEZA, CHAR_PIEZA);
            }
        }
    }

    GoToXY(0, BORDE_INFERIOR+1);
    cambiarColor(15);
}

void Juego::borrarPieza(){
    Coordenada* pCord = this->tablero->getCoordenada();
    Pieza* p = this->tablero->getPieza();
    int dimPieza = p->getDimensiones();

    for (int y = 0; y < dimPieza; y++){
        for (int x = 0; x < dimPieza; x++){
            if (p->existeEn(x, y)){
                GoToXY(BORDE_IZQUIERDO+2 + (pCord->getX()+x)*2, BORDE_SUPERIOR+1 + pCord->getY()+y);
                printf("%c%c", 32, 32);
            }
        }
    }
    GoToXY(0, BORDE_INFERIOR+1);
}

void Juego::mostrarColaDePiezas(){
    GoToXY(BORDE_DERECHO+3, BORDE_SUPERIOR+4);
    printf("Alternativas:");

    for (int y = 0; y < 8; y++){
        for (int x = 0; x < 4; x++){
            GoToXY(BORDE_DERECHO+4 +x*2, BORDE_SUPERIOR+6 + y);
            printf("%c%c", 32, 32);
        }
    }

    int t = 0;
    Pieza** cola = tablero->getColaDePiezas(t);
    int dimPieza;
    int j = 0;

    for (int i = t-1; i > 0; i--){
        cambiarColor(cola[i]->getColor());
        dimPieza = cola[i]->getDimensiones();

        for (int y = 0; y < dimPieza; y++){
            for (int x = 0; x < dimPieza; x++){
                if (cola[i]->existeEn(x, y)){
                    GoToXY(BORDE_DERECHO+4 +x*2, BORDE_SUPERIOR+6 + j);
                    printf("%c%c", CHAR_PIEZA, CHAR_PIEZA);
                }
            }
            j++;
        }
        j++;
    }
    GoToXY(0, BORDE_INFERIOR+1);
    cambiarColor(15);
}

////////////////////////////////////////////////////////////////////
void Juego::mostrarBordes(){
    cambiarColor(15);

    GoToXY(BORDE_IZQUIERDO, BORDE_SUPERIOR); printf(" %c", 220);
    GoToXY(BORDE_DERECHO,   BORDE_SUPERIOR); printf("%c ", 220);
    GoToXY(BORDE_IZQUIERDO, BORDE_INFERIOR); printf(" %c", 223);
    GoToXY(BORDE_DERECHO,   BORDE_INFERIOR); printf("%c ", 223);

    for (int i = BORDE_IZQUIERDO+2; i < BORDE_DERECHO; i+=2){
        GoToXY(i, BORDE_SUPERIOR); printf("%c%c", 219, 219);
        GoToXY(i, BORDE_INFERIOR); printf("%c%c", 219, 219);
    }
    for (int i = BORDE_SUPERIOR+1; i < BORDE_INFERIOR; i++){
        GoToXY(BORDE_IZQUIERDO, i); printf("%c%c", 219, 219);
        GoToXY(BORDE_DERECHO,   i); printf("%c%c", 219, 219);
    }
    GoToXY(BORDE_IZQUIERDO, BORDE_INFERIOR+1);
}
void Juego::mostrarTablero(){
    for (int y = tablero->getAlto()-1; y >= 0; y--){
    //for (int y = 0; y < tablero->getAlto(); y++){
        for (int x = 0; x < tablero->getAncho(); x++){
            GoToXY(posTablero->getX()+(x*2), posTablero->getY()+y);
            if (tablero->get(x,y) != 0){
                cambiarColor(tablero->get(x,y));
                printf("%c%c", 219, 219);
                //Sleep(20);
            }else{
                printf("%c%c", 32, 32);
            }
        }
    }
    cambiarColor(15);
    GoToXY(0, BORDE_INFERIOR+1);
}

void Juego::mostrarGameOver(){
    cambiarColor(4);
    GoToXY(BORDE_IZQUIERDO+2+(ANCHO/2)*2-5, BORDE_SUPERIOR+1+(ALTO/2)-1);
    printf(" GAME OVER ");
    cambiarColor(15);
    GoToXY(0, BORDE_INFERIOR+1);
}

void Juego::mostrarPuntaje(int puntaje){
    cambiarColor(15);
    GoToXY(BORDE_DERECHO+3, BORDE_SUPERIOR+2);
    printf("Puntaje: %d", puntaje);
    GoToXY(0, BORDE_INFERIOR+1);
}

////////////////////////////////////////////////////////////////////
void Juego::GoToXY(int x, int y){		// Coloca el cursor en la ordenada indicada
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = static_cast<short>(x);
    dwPos.Y = static_cast<short>(y);;
    SetConsoleCursorPosition(hCon, dwPos);
}
void Juego::OcultarCursor(){			// Oculta el cursor
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}
void Juego::cambiarColor(int color){
    // Necesita #include <windows.h>
    HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}

