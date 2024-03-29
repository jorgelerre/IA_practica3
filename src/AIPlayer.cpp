//./bin/Parchis --p1 Ninja 1 "Ninja" --p2 AI 6 "Poda" --no-gui

# include "AIPlayer.h"
# include "Parchis.h"

#define CASILLASCASAMETA 73

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta
bool verbose = false;

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

void imprimeHijos(const Parchis & p){
	color id_color = none;
	int id_piece = -1, dice = -1, contador_hijos = 1;
	Parchis sig = p.generateNextMove(id_color, id_piece, dice);
	while(!(p == sig)){
		cout << "Mov. " << contador_hijos << ":\tColor: " << id_color << "\tPieza:" << id_piece << "\tDado: " << dice << endl;
		contador_hijos++;
		sig = p.generateNextMove(id_color, id_piece, dice);
	}
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
        	Valoracion1_verbose(*actual, actual->getCurrentPlayerId());
            imprimeHijos(*actual);
            valor = busquedaMinimax(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, Valoracion1);
            cout << "Valoración tablero = " << valor << endl;
            break;
        case 5:
            Valoracion1_verbose(*actual, actual->getCurrentPlayerId());
            imprimeHijos(*actual);
            valor = Poda_AlfaBeta(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, Valoracion1);
            cout << "Valoración tablero = " << valor << endl;
            break;
        case 6:
            Valoracion1_verbose(*actual, actual->getCurrentPlayerId());
            //imprimeHijos(*actual);
            valor = Poda_AlfaBeta(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, Valoracion2);
            cout << "Valoración tablero = " << valor << endl;
            break;
        case 7:
        	verbose = true;
        	Valoracion3_verbose(*actual, actual->getCurrentPlayerId());
        	verbose = false;
        	valor = Poda_AlfaBeta(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, Valoracion3_verbose);
            cout << "Valoración tablero = " << valor << endl;
            break;
    }
    //cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    
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
    double mejor_h;
    
    //Si estamos a la profundidad suficiente, operamos directamente sobre el estado
    if(profundidad_max == profundidad || actual.gameOver()){
        mejor_h = heuristic(actual, jugador);
        //Si nos encontramos un nodo que gana la partida, dividimos el valor de la 
        //heuristica por la profundidad para priorizar nodos menos profundos.
        if(mejor_h == gana){
        	mejor_h /= profundidad;
        	cout << "Detectado nodo terminal premat. a p = " << profundidad << endl;
        }
        //cout << "Nodo frontera... / h = " << mejor_h << endl;
    }
    //Si no estamos a la suficiente profundidad
    else{
        //cout << "Nodo interno..." << endl;
        
        //Tomamos el primer hijo del nodo
        Parchis siguiente_hijo = actual.generateNextMove(last_c_piece, last_id_piece, last_dice);

    	//Calculamos su valor h llamando recursivamente a la función
	    double h = busquedaMinimax(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, heuristic);
	    if(h == gana){
		h /= (profundidad+1);
	    }
	    //Tomamos el primer hijo como solucion de referencia
	    c_piece = last_c_piece;
	    id_piece = last_id_piece;
	    dice = last_dice;
	    mejor_h = h;
	    
	    //cout << "Generado primer hijo" << endl;

		while(!(siguiente_hijo == actual)){   //Recorremos los hijos del nodo
			if(actual.getCurrentPlayerId() == jugador){    //Si es un nodo max
				//Guardamos la jugada como mejor si su heurística es la mejor
				if(h > mejor_h){
				//cout << "Nuevo mejor nodo con h = " << h << endl;
				c_piece = last_c_piece;
				id_piece = last_id_piece;
				dice = last_dice;
				mejor_h = h;
				}
			}
			else{   //Si es un nodo min
				//Guardamos la jugada como mejor si su heurística es la mejor para el contrincante
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
    double h;
    
    //Si estamos en un nodo hoja, evaluamos el tablero
    if(profundidad_max == profundidad || actual.gameOver()){
        h = heuristic(actual, jugador);
        //Si nos encontramos un nodo que gana la partida, dividimos el valor de la 
        //heuristica por la profundidad para priorizar nodos menos profundos.
        if(h == gana){
        	h /= (profundidad+1);
        	cout << "Detectado nodo terminal premat. a p = " << profundidad << endl;
        }
        //cout << "Nodo frontera... / h = " << mejor_h << endl;
        return h;
    }
    //Si no estamos a la suficiente profundidad
    else{
        //cout << "Nodo interno p = " << profundidad << endl;
        
        //Tomamos el primer hijo del nodo
        Parchis siguiente_hijo = actual.generateNextMoveDescending(last_c_piece, last_id_piece, last_dice);
        mejor_color = last_c_piece;
        mejor_piece = last_id_piece;
        mejor_dice = last_dice;
        
        //Calculamos su valor h llamando recursivamente a la función
        h = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, alpha, beta, heuristic);
        
        //Tomamos el primer hijo como solucion de referencia
        c_piece = last_c_piece;
        id_piece = last_id_piece;
        dice = last_dice;
        
        //cout << "Generado primer hijo" << endl;

        while(!(siguiente_hijo == actual) && alpha < beta){   //Recorremos los hijos del nodo

            if(actual.getCurrentPlayerId() == jugador){    //Si es un nodo max
                //Guardamos la jugada como mejor si su heurística es la mejor
                //cout << "Valorando nodo max / p = " << profundidad;
                //cout << "Alpha = " << alpha << "\tBeta = " << beta << endl;
                
                if(h > alpha){
                    //cout << "Nuevo mejor alpha = " << h << endl;
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                    alpha = h;  //Actualizamos valor de alpha
                }
            }
            else{   //Si es un nodo min
                //cout << "Valorando nodo max / p = " << profundidad;
                //cout << "Alpha = " << alpha << "\tBeta = " << beta << endl;
                if(h < beta){
                    //cout << "Nuevo mejor beta = " << h << endl;
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                    beta = h;   //Actualizamos valor de beta
                }
            }

            //Generamos siguiente hijo
            siguiente_hijo = actual.generateNextMoveDescending(last_c_piece, last_id_piece, last_dice);
            mejor_color = last_c_piece;
            mejor_piece = last_id_piece;
            mejor_dice = last_dice;
            h = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad+1, profundidad_max, mejor_color, mejor_piece, mejor_dice, alpha, beta, heuristic);
            if(h == gana){
                h /= (double) (profundidad+1);
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
                else if (estado.getBoard().getPiece(c, j).type == goal)
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
                else if (estado.getBoard().getPiece(c, j).type == goal)
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
    
    if(estado.gameOver()){
        // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
        if (ganador == jugador)
        {
            return gana;
        }
        else
        {
            return pierde;
        }
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        int puntuacion_color[2];
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, j);
                puntuacion_color[i] += (73 - distancia_meta) / 2;
                
                if(distancia_meta == 0){
                	puntuacion_color[i] += 20;	//Bonus por ficha en meta
                }
                else if(distancia_meta < 8){	//Bonus por ficha en camino final
                	puntuacion_color[i] += 1;
                }
                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += 3;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_color[i] -= 5;
                }
            }
        }
        
        /*
        if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[1] /= 2;
        }
        else{
        	puntuacion_color[0] /= 2;
        }
        */
        puntuacion_jugador = puntuacion_color[0] + puntuacion_color[1];
        
        

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        puntuacion_color[0] = puntuacion_color[1] = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, j);
                puntuacion_color[i] += (73 - distancia_meta) / 2;
                if(distancia_meta == 0){
                	puntuacion_color[i] += 10;
                }
				else if(distancia_meta < 8){
                	puntuacion_color[i] += 5;
                }
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += 3;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_color[i] -= 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal){
                    puntuacion_color[i] += 10;
                }
            }
        }
		
	/*
	if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[1] /= 2;
        }
        else{
        	puntuacion_color[0] /= 2;
        }
        */
        puntuacion_oponente = puntuacion_color[0] + puntuacion_color[1];
        
        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::Valoracion1_verbose(const Parchis &estado, int jugador)
{
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    
    if(estado.gameOver()){
        // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
        if (ganador == jugador)
        {
        	cout << "Juego acabado: Ganaste. GG" << endl;
            return gana;
        }
        else
        {
        	cout << "Juego acabado: Perdiste. GG" << endl;
            return pierde;
        }
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

		cout << "Valoracion del jugador bueno: " << endl;
        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        //int puntuacion_color = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            cout << "Color " << str(c) << ":" << endl;
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                cout << "Ficha " << j << endl;
                puntuacion_jugador += (73 - estado.distanceToGoal(c, j)) / 2;
                cout << "Distancia a meta: " << estado.distanceToGoal(c, j) << endl;
                cout << "Puntos por distancia a meta=" << (73 - estado.distanceToGoal(c, j)) / 2 << endl;
                
                if(estado.distanceToGoal(c, j) < 1){
                	cout << "10 puntos extra por estar en meta" << endl;
                	puntuacion_jugador += 10;
                }
                else if(estado.distanceToGoal(c, j) < 8){
                	cout << "5 puntos extra por estar en pasillo final" << endl;
                	puntuacion_jugador += 5;
                }
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (!estado.isSafePiece(c, j))
                {
                	cout << "3 puntos extra por estar en casilla segura" << endl;
                    puntuacion_jugador += 3;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                	cout << "Penalizacion de 5 puntos por estar en casa" << endl;
                    puntuacion_jugador -= 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal){
                	cout << "10 puntos extra por estar en meta" << endl;
                    puntuacion_jugador += 10;
                }
            }
        }
        
		cout << "Valoracion del jugador maligno que me quiere hacer danio :( : " << endl;
        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            cout << "Color " << str(c) << ":" << endl;
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
            	cout << "Ficha " << j << endl;
                //Valoro positivamente que la ficha esté cerca de la meta
                puntuacion_oponente += (73 - estado.distanceToGoal(c, j)) / 2;
                cout << "Distancia a meta: " << estado.distanceToGoal(c, j) << endl;
                cout << "Puntos por distancia a meta=" << (73 - estado.distanceToGoal(c, j)) / 2 << endl;
                if(estado.distanceToGoal(c, j) < 1){
                	cout << "10 puntos extra por estar en meta" << endl;
                	puntuacion_oponente += 10;
                }
				else if(estado.distanceToGoal(c, j) < 8){
                	cout << "5 puntos extra por estar en pasillo final" << endl;
                	puntuacion_oponente += 5;
                }
                if (!estado.isSafePiece(c, j))
                {
                	cout << "3 puntos extra por estar en casilla segura" << endl;
                    puntuacion_oponente += 3;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                	cout << "Penalizacion de 5 puntos por estar en casa" << endl;
                    puntuacion_oponente -= 5;
                }
                /* //no funciona hacer esa comprobación (sorprendentemente)
                else if (estado.getBoard().getPiece(c, j).type == goal){
                	cout << "10 puntos extra por estar en meta" << endl;
                    puntuacion_oponente += 10;
                }
                */
            }
        }
		cout << "***************Valoracion final = " << puntuacion_jugador - puntuacion_oponente << "***************" << endl; 
        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::Valoracion2(const Parchis &estado, int jugador)
{
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    
    if(estado.gameOver()){
        // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
        if (ganador == jugador)
        {
            return gana;
        }
        else
        {
            return pierde;
        }
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        int puntuacion_color[2];
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, j);
                puntuacion_color[i] += (73 - distancia_meta) / 2;
                
                if(distancia_meta == 0){
                	puntuacion_color[i] += 20;	//Bonus por ficha en meta
                }
                else if(distancia_meta < 8){	//Bonus por ficha en camino final
                	puntuacion_color[i] += 1;
                }
                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += 3;
                }
                //Valoro negativamente que la ficha esté en casa.
                if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_color[i] -= 5;
                }
                
                
            }
        }
        
        
        if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[1] /= 2;
        }
        else{
        	puntuacion_color[0] /= 2;
        }
        
        puntuacion_jugador = puntuacion_color[0] + puntuacion_color[1];
        
        

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        puntuacion_color[0] = puntuacion_color[1] = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                //Valoro positivamente que la ficha esté cerca de la meta
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, j);
                puntuacion_color[i] += (73 - distancia_meta) / 2;
                if(distancia_meta == 0){
                	puntuacion_color[i] += 10;
                }
				else if(distancia_meta < 8){
                	puntuacion_color[i] += 5;
                }
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += 3;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_color[i] -= 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal){
                    puntuacion_color[i] += 10;
                }
            }
        }
		
	
	if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[1] /= 2;
        }
        else{
        	puntuacion_color[0] /= 2;
        }
        
        puntuacion_oponente = puntuacion_color[0] + puntuacion_color[1];
        
        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

//Tenemos en cuenta TODOS los factores para ir despues ajustando el porcentaje en que
//influirá cada uno de ellos.
double AIPlayer::Valoracion3(const Parchis &estado, int jugador){
	int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    
    if(estado.gameOver()){
        // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
        if (ganador == jugador)
        {
        	//cout << "Victoria jugador" << endl;
            return gana;
        }
        else
        {
        	//cout << "Victoria oponente" << endl;
            return pierde;
        }
    }
    else
    {
        double coef_casa = -5,
        	coef_pasillo_final = 1,
        	coef_meta = 20,
        	coef_barrera = -3,
        	coef_segura = 3,
        	coef_ficha_comible = 5,
        	coef_distancia_meta = 0.5,
        	coef_balance_colores = 0.5;
        Box meta;
        meta.type = goal;
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        double puntuacion_jugador = 0;
        double puntuacion_color[2];
        puntuacion_color[0] = puntuacion_color[1] = 0;
        // Recorro colores de mi jugador.
        //cout << "******************************Puntuacion jugador******************************" << endl;
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            meta.col = c;
            //cout << "/*/*/*/*/Color " << str(c) << "\\*\\*\\*\\*\\"<< endl;
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
            	//cout << "////Pieza " << j << ":" << endl;
                
                Box casilla_actual = estado.getBoard().getPiece(c,j);
            	
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, casilla_actual);
                puntuacion_color[i] += (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta;
                //cout << "Distancia a meta = " << distancia_meta;
                //cout << "\tPuntuacion: " << (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta << endl;
                
                if(distancia_meta == 0){
                	puntuacion_color[i] += coef_meta;	//Bonus por ficha en meta
                	//cout << "Puntos por estar en meta = " << coef_meta << endl;
                }
                else if(distancia_meta < 8){	//Bonus por ficha en camino final
                	puntuacion_color[i] += coef_pasillo_final;
                	//cout << "Puntos por estar en pasillo final = " << coef_pasillo_final << endl;
                }
                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += coef_segura;
                    //cout << "Puntos por estar segura = " << coef_segura << endl;
                }
                //Valoro positivamente que hayan cerca y por delante fichas del otro jugador
                //(puede que en el futuro coma)
                //cout << "Comprobando fichas por delante..." << endl;
            	for (int dado = 1; dado <= 6; dado++){
            		Box casilla_delante = estado.computeMove(c, casilla_actual, dado);
            		//cout << "Pass compute con dado = " << dado << endl;
            		if(casilla_delante.type == normal){
            			vector<pair <color, int>> fichas_casilla = estado.boxState(casilla_delante);
            			//cout << "Pass boxState con dado = " << dado << endl;
            			for(int id_ficha = 0; id_ficha < fichas_casilla.size(); id_ficha++){
            				//cout << "Comprobando ficha " << id_ficha << endl;
            				pair <color, int> pieza = fichas_casilla[id_ficha];
            				if(pieza.first != my_colors[0] && pieza.first != my_colors[1]){
            					puntuacion_color[i] += coef_ficha_comible;
            					//cout << "Puntos por rival cercano (" << str(pieza.first) << "," << pieza.second << ") con dado " << dado << "= " << coef_segura << endl;
            				}
            			}
            		}
            	}
                //cout << "Fin fichas por delante..." << endl;
                //Valoro negativamente que la ficha esté en casa.
                if (casilla_actual.type == home)
                {
                    puntuacion_color[i] += coef_casa;
                    //cout << "Puntos por casilla en casa: " << coef_casa << endl;
                }
                //Valoro negativamente que hayan barreras de otros colores entre la pos. actual y meta.
                vector<color> barreras = estado.anyWall(casilla_actual, meta);
                for(int i = 0; i < barreras.size(); i++){
                	if(barreras[i] != c){
                		puntuacion_color[i] += coef_barrera;
                		//cout << "Puntos por barrera de color " << str(barreras[i]) << ": " << coef_barrera << endl;
                	}
                }
            }
            //cout << endl << "*************************************************************************************"<< endl;
        }
        
        //cout << "Puntuacion color 0: " << puntuacion_color[0] << endl;
        //cout << "Puntuacion color 1: " << puntuacion_color[1] << endl;
        //cout << "Coeficiente de ponderacion de colores = " << coef_balance_colores << endl;
        if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[0] *= coef_balance_colores;
        	puntuacion_color[1] *= (1-coef_balance_colores);
        }
        else{
        	puntuacion_color[0] *= (1-coef_balance_colores);
        	puntuacion_color[1] *= coef_balance_colores;
        }
        
        
        puntuacion_jugador = puntuacion_color[0] + puntuacion_color[1];
        //cout << "Puntuacion jugador = " << puntuacion_jugador << endl;
        //cout << endl << "*************************************************************************************"<< endl;
		//cout << "******************************Puntuacion oponente******************************" << endl;
        // Recorro todas las fichas del oponente
        double puntuacion_oponente = 0;
        puntuacion_color[0] = puntuacion_color[1] = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            meta.col = c;
            //cout << "/*/*/*/*/Color " << str(c) << "\\*\\*\\*\\*\\"<< endl;
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
            	//cout << "////Pieza " << j << ":" << endl;
                
                Box casilla_actual = estado.getBoard().getPiece(c,j);
            	
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, casilla_actual);
                puntuacion_color[i] += (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta;
                //cout << "Distancia a meta = " << distancia_meta;
                //cout << "\tPuntuacion: " << (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta << endl;
                
                if(distancia_meta == 0){
                	puntuacion_color[i] += coef_meta;	//Bonus por ficha en meta
                	//cout << "Puntos por estar en meta = " << coef_meta << endl;
                }
                else if(distancia_meta < 8){	//Bonus por ficha en camino final
                	puntuacion_color[i] += coef_pasillo_final;
                	//cout << "Puntos por estar en pasillo final = " << coef_pasillo_final << endl;
                }
                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += coef_segura;
                    //cout << "Puntos por estar segura = " << coef_segura << endl;
                }
                //Valoro positivamente que hayan cerca y por delante fichas del otro jugador
                //(puede que en el futuro coma)
                
            	//cout << "Comprobando fichas por delante..." << endl;
            	for (int dado = 1; dado <= 6; dado++){
            		Box casilla_delante = estado.computeMove(c, casilla_actual, dado);
            		if(casilla_delante.type == normal){
            			vector<pair <color, int>> fichas_casilla = estado.boxState(casilla_delante);
            			for(int id_ficha = 0; id_ficha < fichas_casilla.size(); id_ficha++){
            				pair <color, int> pieza = fichas_casilla[id_ficha];
            				if(pieza.first != op_colors[0] && pieza.first != op_colors[1]){
            					puntuacion_color[i] += coef_ficha_comible;
            					//cout << "Puntos por rival cercano (" << str(pieza.first) << "," << pieza.second << ") con dado " << dado << "= " << coef_segura << endl;
            				}
            			}
            		}
            	}
                //cout << "Fin fichas por delante..." << endl;
                
                //Valoro negativamente que la ficha esté en casa.
                if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_color[i] += coef_casa;
                    //cout << "Puntos por casilla en casa: " << coef_casa << endl;
                }
                //Valoro negativamente que hayan barreras de otros colores entre la pos. actual y meta.
                vector<color> barreras = estado.anyWall(casilla_actual, meta);
                for(int i = 0; i < barreras.size(); i++){
                	if(barreras[i] != c){
                		puntuacion_color[i] += coef_barrera;
                		//cout << "Puntos por barrera de color " << str(barreras[i]) << ": " << coef_barrera << endl;
                	}
                }
            }
            //cout << endl << "*************************************************************************************"<< endl;
        }
        
        //cout << "Puntuacion color 0: " << puntuacion_color[0] << endl;
        //cout << "Puntuacion color 1: " << puntuacion_color[1] << endl;
        //cout << "Coeficiente de ponderacion de colores = " << coef_balance_colores << endl;
        if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[0] *= coef_balance_colores;
        	puntuacion_color[1] *= (1-coef_balance_colores);
        }
        else{
        	puntuacion_color[0] *= (1-coef_balance_colores);
        	puntuacion_color[1] *= coef_balance_colores;
        }
        
        puntuacion_oponente = puntuacion_color[0] + puntuacion_color[1];
        //cout << "Puntuacion oponente = " << puntuacion_oponente << endl;
        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        //cout << "Valoracion final = " << puntuacion_jugador - puntuacion_oponente << endl;
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::Valoracion3_verbose(const Parchis &estado, int jugador){
	int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;
    
    if(estado.gameOver()){
        // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
        if (ganador == jugador)
        {
        	if(verbose)
        		cout << "Victoria jugador" << endl;
            return gana;
        }
        else
        {
        	if(verbose)
        		cout << "Victoria oponente" << endl;
            return pierde;
        }
    }
    else
    {
        double coef_casa = -100,
        	coef_pasillo_final = 1,
        	coef_meta = 5,
        	coef_barrera = -3,
        	coef_segura = 3,
        	coef_ficha_comible = 3,
        	coef_distancia_meta = 0.4,
        	coef_balance_colores = 0.5;
        Box meta;
        meta.type = goal;
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        double puntuacion_jugador = 0;
        double puntuacion_color[2];
        puntuacion_color[0] = puntuacion_color[1] = 0;
        // Recorro colores de mi jugador.
        if(verbose)
        	cout << "******************************Puntuacion jugador******************************" << endl;
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            meta.col = c;
            if(verbose)
            	cout << "/*/*/*/*/Color " << str(c) << "\\*\\*\\*\\*\\"<< endl;
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
            	if(verbose)
            		cout << "////Pieza " << j << ":" << endl;
                
                Box casilla_actual = estado.getBoard().getPiece(c,j);
            	
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, casilla_actual);
                puntuacion_color[i] += (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta;
                if(verbose){
		            cout << "Distancia a meta = " << distancia_meta;
		            cout << "\tPuntuacion: " << (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta << endl;
                }
                if(distancia_meta == 0){
                	puntuacion_color[i] += coef_meta;	//Bonus por ficha en meta
                	if(verbose)
                		cout << "Puntos por estar en meta = " << coef_meta << endl;
                }
                else if(distancia_meta < 8){	//Bonus por ficha en camino final
                	puntuacion_color[i] += coef_pasillo_final;
                	if(verbose)
                		cout << "Puntos por estar en pasillo final = " << coef_pasillo_final << endl;
                }
                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += coef_segura;
                    if(verbose)
                    	cout << "Puntos por estar segura = " << coef_segura << endl;
                }
                //Valoro positivamente que hayan cerca y por delante fichas del otro jugador
                //(puede que en el futuro coma)
                if(verbose)
                	cout << "Comprobando fichas por delante..." << endl;
            	for (int dado = 1; dado <= 6; dado++){
            		Box casilla_delante = estado.computeMove(c, casilla_actual, dado);
            		if(verbose)
            			//cout << "Pass compute con dado = " << dado << endl;
            		if(casilla_delante.type == normal){
            			vector<pair <color, int>> fichas_casilla = estado.boxState(casilla_delante);
            			if(verbose)
            				//cout << "Pass boxState con dado = " << dado << endl;
            			for(int id_ficha = 0; id_ficha < fichas_casilla.size(); id_ficha++){
            				if(verbose)
            					cout << "Comprobando ficha " << id_ficha << endl;
            				pair <color, int> pieza = fichas_casilla[id_ficha];
            				if(pieza.first != my_colors[0] && pieza.first != my_colors[1]){
            					puntuacion_color[i] += coef_ficha_comible;
            					if(verbose)
            						cout << "Puntos por rival cercano (" << str(pieza.first) << "," << pieza.second << ") con dado " << dado << "= " << coef_segura << endl;
            				}
            			}
            		}
            	}
                if(verbose)
                	cout << "Fin fichas por delante..." << endl;
                //Valoro negativamente que la ficha esté en casa.
                if (casilla_actual.type == home)
                {
                    puntuacion_color[i] += coef_casa;
                    if(verbose)
                    	cout << "Puntos por casilla en casa: " << coef_casa << endl;
                }
                //Valoro negativamente que hayan barreras de otros colores entre la pos. actual y meta.
                vector<color> barreras = estado.anyWall(casilla_actual, meta);
                for(int i = 0; i < barreras.size(); i++){
                	if(barreras[i] != my_colors[0] && barreras[i] != my_colors[1]){
                		puntuacion_color[i] += coef_barrera;
                		if(verbose)
                			cout << "Puntos por barrera de color " << str(barreras[i]) << ": " << coef_barrera << endl;
                	}
                }
            }
            if(verbose)
            	cout << endl << "*************************************************************************************"<< endl;
        }
        
        if(verbose){
		    cout << "Puntuacion color 0: " << puntuacion_color[0] << endl;
		    cout << "Puntuacion color 1: " << puntuacion_color[1] << endl;
		    cout << "Coeficiente de ponderacion de colores = " << coef_balance_colores << endl;
		}
        if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[0] *= coef_balance_colores;
        	puntuacion_color[1] *= (1-coef_balance_colores);
        }
        else{
        	puntuacion_color[0] *= (1-coef_balance_colores);
        	puntuacion_color[1] *= coef_balance_colores;
        }
        
        
        puntuacion_jugador = puntuacion_color[0] + puntuacion_color[1];
       	if(verbose){
		    cout << "Puntuacion jugador = " << puntuacion_jugador << endl;
		    cout << endl << "*************************************************************************************"<< endl;
			cout << "******************************Puntuacion oponente******************************" << endl;
		}
        // Recorro todas las fichas del oponente
        double puntuacion_oponente = 0;
        puntuacion_color[0] = puntuacion_color[1] = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            meta.col = c;
            if(verbose)
            	cout << "/*/*/*/*/Color " << str(c) << "\\*\\*\\*\\*\\"<< endl;
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
            	if(verbose)
            		cout << "////Pieza " << j << ":" << endl;
                
                Box casilla_actual = estado.getBoard().getPiece(c,j);
            	
                //Valoro positivamente que la ficha esté cerca de la meta
                int distancia_meta = estado.distanceToGoal(c, casilla_actual);
                puntuacion_color[i] += (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta;
                if(verbose){
		            cout << "Distancia a meta = " << distancia_meta;
		            cout << "\tPuntuacion: " << (CASILLASCASAMETA - distancia_meta) * coef_distancia_meta << endl;
                }
                if(distancia_meta == 0){
                	puntuacion_color[i] += coef_meta;	//Bonus por ficha en meta
                	if(verbose)
                		cout << "Puntos por estar en meta = " << coef_meta << endl;
                }
                else if(distancia_meta < 8){	//Bonus por ficha en camino final
                	puntuacion_color[i] += coef_pasillo_final;
                	if(verbose)
                		cout << "Puntos por estar en pasillo final = " << coef_pasillo_final << endl;
                }
                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_color[i] += coef_segura;
                    if(verbose)
                    	cout << "Puntos por estar segura = " << coef_segura << endl;
                }
                //Valoro positivamente que hayan cerca y por delante fichas del otro jugador
                //(puede que en el futuro coma)
                if(verbose)
            		cout << "Comprobando fichas por delante..." << endl;
            	for (int dado = 1; dado <= 6; dado++){
            		Box casilla_delante = estado.computeMove(c, casilla_actual, dado);
            		if(casilla_delante.type == normal){
            			vector<pair <color, int>> fichas_casilla = estado.boxState(casilla_delante);
            			for(int id_ficha = 0; id_ficha < fichas_casilla.size(); id_ficha++){
            				pair <color, int> pieza = fichas_casilla[id_ficha];
            				if(pieza.first != op_colors[0] && pieza.first != op_colors[1]){
            					puntuacion_color[i] += coef_ficha_comible;
            					if(verbose)
            						cout << "Puntos por rival cercano (" << str(pieza.first) << "," << pieza.second << ") con dado " << dado << "= " << coef_segura << endl;
            				}
            			}
            		}
            	}
                if(verbose)
                	cout << "Fin fichas por delante..." << endl;
                
                //Valoro negativamente que la ficha esté en casa.
                if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_color[i] += coef_casa;
                    if(verbose)
                    	cout << "Puntos por casilla en casa: " << coef_casa << endl;
                }
                //Valoro negativamente que hayan barreras de otros colores entre la pos. actual y meta.
                vector<color> barreras = estado.anyWall(casilla_actual, meta);
                for(int i = 0; i < barreras.size(); i++){
                	if(barreras[i] != op_colors[0] && barreras[i] != op_colors[1]){
                		puntuacion_color[i] += coef_barrera;
                		if(verbose)
                			cout << "Puntos por barrera de color " << str(barreras[i]) << ": " << coef_barrera << endl;
                	}
                }
            }
            if(verbose)
            	cout << endl << "*************************************************************************************"<< endl;
        }
        if(verbose){
		    cout << "Puntuacion color 0: " << puntuacion_color[0] << endl;
		    cout << "Puntuacion color 1: " << puntuacion_color[1] << endl;
		    cout << "Coeficiente de ponderacion de colores = " << coef_balance_colores << endl;
		}
        if(puntuacion_color[0] > puntuacion_color[1]){
        	puntuacion_color[0] *= coef_balance_colores;
        	puntuacion_color[1] *= (1-coef_balance_colores);
        }
        else{
        	puntuacion_color[0] *= (1-coef_balance_colores);
        	puntuacion_color[1] *= coef_balance_colores;
        }
        
        puntuacion_oponente = puntuacion_color[0] + puntuacion_color[1];
        if(verbose){
		    cout << "Puntuacion oponente = " << puntuacion_oponente << endl;
		    cout << "Valoracion final = " << puntuacion_jugador - puntuacion_oponente << endl;
		}
        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        
        return puntuacion_jugador - puntuacion_oponente;
    }
}


/*
Implementar funcion que devuelva cout de la partida
*/
