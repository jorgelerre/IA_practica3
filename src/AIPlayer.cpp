# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;
    
    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.
    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    //valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            thinkAleatorio(c_piece, id_piece, dice);
            break;
        case 1:
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);
            break;
        case 2:
            thinkFichaMasAdelantada(c_piece, id_piece, dice);
            break;
        case 3:
            thinkMejorOpcion(c_piece, id_piece, dice);
            break;
        case 4:
            valor = busquedaMinimax(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, Valoracion1);
            /*
            if(valor == gana){
                thinkMejorOpcion(c_piece, id_piece, dice);
            }
            */
            break;
        case 5:
            valor = Poda_AlfaBeta(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_ALFABETA-2, c_piece, id_piece, dice, alpha, beta, Valoracion1);
            /*
            if(valor == gana){
                thinkMejorOpcion(c_piece, id_piece, dice);
            }
            */
            break;
    }
     cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    
}

void AIPlayer::thinkAleatorio(color & c_piece, int & id_piece, int & dice) const {
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente 
    //como lo que se muestran al final de la función.
    
    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores, 
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(c_piece, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if(current_pieces.size() > 0){
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
    else{
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
    }
}


void AIPlayer::thinkAleatorioMasInteligente(color & c_piece, int & id_piece, int & dice) const {
    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);

    //Vemos que fichas podemos mover con dichos dados
    vector<int> current_dices_usables;
    for(int i = 0; i < current_dices.size(); i++){
        //Se obtiene el vector de fichas que se pueden mover para el dado elegido
        current_pieces = actual->getAvailablePieces(c_piece, current_dices[i]);

        //Si se puede mover ficha para el dado actual, añadimos el dado a current_dices_usables
        if(current_pieces.size() > 0)
            current_dices_usables.push_back(current_dices[i]);
    }

    //Si no tengo ningun dado con el que mover ficha, saltamos turno
    if(current_dices_usables.size() == 0){
        dice = current_dices[rand() % current_dices.size()];
        id_piece = SKIP_TURN;
    }
    //En otro caso, elijo un dado de forma aleatoria de entre los que pueden mover ficha
    else{
        dice = current_dices_usables[rand() % current_dices_usables.size()];

        //Se obtiene el vector de fichas que se pueden mover para el dado elegido
        current_pieces = actual->getAvailablePieces(c_piece, dice);

        //Movemos una de las fichas movibles al azar
        id_piece = current_pieces[rand() % current_pieces.size()];
    }

}

void AIPlayer::thinkFichaMasAdelantada(color & c_piece, int & id_piece, int & dice) const {
    //Elijo el dado haciendo lo mismo que el jugador anterior
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);

    //Ahora, en vez de mover una ficha al azar, movemos la que esté más adelantada.

    vector<int> current_pieces = actual->getAvailablePieces(c_piece, dice);

    int id_ficha_mas_adelantada = -1;
    int min_distancia_meta = 999;
    for(int i = 0; i < current_pieces.size(); i++){
        int distancia_meta = actual->distanceToGoal(c_piece, current_pieces[i]);
        if(distancia_meta < min_distancia_meta){
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = current_pieces[i];
        }
    }

    if(id_ficha_mas_adelantada == -1){
        id_piece = SKIP_TURN;
    }
    else{
        id_piece = id_ficha_mas_adelantada;
    }

}

void AIPlayer::thinkMejorOpcion(color & c_piece, int & id_piece, int & dice) const{
    //Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.

    // generateNextMove va iterando cobre cada hijo. Le paso la acción del último movimiento sobre
    // el que he iterado y me devolverá el siguiente. Inicialmente, cuando aún no he hecho ningún
    // movimiento, lo inicializo así.
    color last_c_piece = none;
    int last_id_piece = -1;
    int last_dice = -1;

    // Cuando ya haya recorrido todos los hijos, la función devuelve el estado actual --> Condición de parada

    Parchis siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);

    bool hay_solucion = false;

    while(!(siguiente_hijo == *actual) && !hay_solucion){
        if(siguiente_hijo.isEatingMove() || //Si comemos
           siguiente_hijo.isGoalMove() ||   //Si ficha llega a meta
           (siguiente_hijo.gameOver() && siguiente_hijo.getWinner() == this->jugador)   //Si ganamos
        ){ 
            hay_solucion = true;
        }
        else{
            siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);
        }
    }

    //Si encontramos una accion solucion, se guarda en las variables pasadas por referencia
    if(hay_solucion){
        c_piece = last_c_piece;
        id_piece = last_id_piece;
        dice = last_dice;
    }
    //Si no, llamamos al metodo que adelanta la ficha más adelantada.
    else{
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }

}

double AIPlayer::busquedaMinimax(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double (*heuristic)(const Parchis &, int)) const{
    //Variables donde guardaremos el estado del hijo actualmente tratado
    color last_c_piece = none;
    int last_id_piece = -1;
    int last_dice = -1;

    //Variables donde guardaremos las soluciones provisionales locales
    color mejor_color;
    int mejor_piece;
    int mejor_dice;
    int mejor_h;
    
    //Si estamos a la profundidad suficiente, operamos directamente sobre el estado
    if(profundidad_max == profundidad){
        mejor_h = heuristic(actual, jugador);
        //cout << "Nodo frontera... / h = " << mejor_h << endl;
    }
    //Si no estamos a la suficiente profundidad
    else{
        //cout << "Nodo interno..." << endl;
        
        //Tomamos el primer hijo del nodo
        Parchis siguiente_hijo = actual.generateNextMove(last_c_piece, last_id_piece, last_dice);
        
        if(siguiente_hijo.gameOver()){  //Si la partida acaba en ese nodo
            //Tomamos la solución
            c_piece = last_c_piece;
            id_piece = last_id_piece;
            dice = last_dice;

            if(siguiente_hijo.getWinner() == jugador){  //Y ganamos
                return gana;
            }
            else{
                return pierde;
            }
        }

        else{   //Si no ha terminado la partida
            //Calculamos su valor h llamando recursivamente a la función
            int h = busquedaMinimax(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, heuristic);
            //Tomamos el primer hijo como solucion de referencia
            c_piece = last_c_piece;
            id_piece = last_id_piece;
            dice = last_dice;
            mejor_h = h;
            
            //cout << "Generado primer hijo" << endl;

            while(!(siguiente_hijo == actual)){   //Recorremos los hijos del nodo

                if(actual.getCurrentPlayerId() == jugador){    //Si es un nodo max
                    //Guardamos la jugada como mejor si su heurística es la mejor
                    //cout << "Valorando nodo max / p = " << profundidad << " y mejor_h = " << mejor_h << endl;
                    
                    if(h > mejor_h){
                        //cout << "Nuevo mejor nodo con h = " << h << endl;
                        c_piece = last_c_piece;
                        id_piece = last_id_piece;
                        dice = last_dice;
                        mejor_h = h;
                    }
                }
                else{   //Si es un nodo min
                    //cout << "Valorando nodo min / p = " << profundidad << " y mejor_h = " << mejor_h << endl;
                    if(h < mejor_h){
                        //cout << "Nuevo mejor nodo con h = " << h << endl;
                        c_piece = last_c_piece;
                        id_piece = last_id_piece;
                        dice = last_dice;
                        mejor_h = h;
                    }
                }

                //Generamos siguiente hijo
                siguiente_hijo = actual.generateNextMove(last_c_piece, last_id_piece, last_dice);
                h = busquedaMinimax(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, heuristic);
            }
        }
    }
    
    //cout << "Saliendo de p = " << profundidad << " con h = " << mejor_h << endl;
    
    return mejor_h;
}


double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const{
    //Variables donde guardaremos el estado del hijo actualmente tratado
    color last_c_piece = none;
    int last_id_piece = -1;
    int last_dice = -1;

    //Variables donde guardaremos las soluciones provisionales locales
    color mejor_color;
    int mejor_piece;
    int mejor_dice;
    int h;
    
    //Si estamos a la profundidad frontera, operamos directamente sobre el estado
    if(profundidad_max == profundidad){
        h = heuristic(actual, jugador);
        cout << "Nodo frontera... / h = " << h << endl;
        return h;
    }
    //Si no estamos a la suficiente profundidad
    else{
        cout << "Nodo interno p = " << profundidad << endl;
        //Parada en nodos internos
        char a;
        //cin >> a ;
        //Tomamos el primer hijo del nodo
        Parchis siguiente_hijo = actual.generateNextMoveDescending(last_c_piece, last_id_piece, last_dice);
        
        //Si la partida acaba en ese nodo, lo tratamos como un nodo frontera
        if(siguiente_hijo.gameOver()){  
            //Tomamos la solución
            c_piece = last_c_piece;
            id_piece = last_id_piece;
            dice = last_dice;

            if(siguiente_hijo.getWinner() == jugador){
                return gana;
            }
            else{
                return pierde;
            }
        }
        else{   //Si no ha terminado la partida
            //Calculamos su valor h llamando recursivamente a la función
            int h = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, alpha, beta, heuristic);
            //Tomamos el primer hijo como solucion de referencia
            c_piece = last_c_piece;
            id_piece = last_id_piece;
            dice = last_dice;
            
            //cout << "Generado primer hijo" << endl;

            while(!(siguiente_hijo == actual) && alpha < beta){   //Recorremos los hijos del nodo

                if(actual.getCurrentPlayerId() == jugador){    //Si es un nodo max
                    //Guardamos la jugada como mejor si su heurística es la mejor
                    cout << "Valorando nodo max / p = " << profundidad;
                    cout << "Alpha = " << alpha << "\tBeta = " << beta << endl;
                    
                    if(h > alpha){
                        cout << "Nuevo mejor alpha = " << h << endl;
                        c_piece = last_c_piece;
                        id_piece = last_id_piece;
                        dice = last_dice;
                        alpha = h;  //Actualizamos valor de alpha
                    }
                }
                else{   //Si es un nodo min
                    cout << "Valorando nodo max / p = " << profundidad;
                    cout << "Alpha = " << alpha << "\tBeta = " << beta << endl;
                    if(h < beta){
                        cout << "Nuevo mejor beta = " << h << endl;
                        c_piece = last_c_piece;
                        id_piece = last_id_piece;
                        dice = last_dice;
                        beta = h;   //Actualizamos valor de beta
                    }
                }

                //Generamos siguiente hijo
                siguiente_hijo = actual.generateNextMoveDescending(last_c_piece, last_id_piece, last_dice);
                h = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, alpha, beta, heuristic);
            }
        }
    }
    
    //cout << "Saliendo de p = " << profundidad << " con h = " << mejor_h << endl;
    if(actual.getCurrentPlayerId() == jugador){    //Si es un nodo max
        return alpha;
    }
    else{   //Si es un nodo min
        return beta;
    }
    
}


double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::Valoracion1(const Parchis &estado, int jugador)
{

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        //int puntuacion_color = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                puntuacion_jugador += (73 - estado.distanceToGoal(c, j)) / 2;
                
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_jugador -= 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal){
                    puntuacion_jugador += 10;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                puntuacion_oponente += (73 - estado.distanceToGoal(c, j)) / 2;

                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_oponente -= 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal){
                    puntuacion_oponente += 10;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}