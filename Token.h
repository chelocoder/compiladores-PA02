#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum class TipoToken {
    PALABRA_RESERVADA,
    IDENTIFICADOR,
    ENTERO,
    DECIMAL,
    CADENA,
    OPERADOR,
    ASIGNACION,
    DELIMITADOR,
    DESCONOCIDO,
    FIN_ARCHIVO
};

struct Token {
    TipoToken tipo;
    string lexema;
    int linea;
    int columna;
};

inline string tipoTokenAString(TipoToken tipo) {

    switch (tipo) {

        case TipoToken::PALABRA_RESERVADA:
            return "PALABRA_RESERVADA";

        case TipoToken::IDENTIFICADOR:
            return "IDENTIFICADOR";

        case TipoToken::ENTERO:
            return "ENTERO";

        case TipoToken::DECIMAL:
            return "DECIMAL";

        case TipoToken::CADENA:
            return "CADENA";

        case TipoToken::OPERADOR:
            return "OPERADOR";

        case TipoToken::ASIGNACION:
            return "ASIGNACION";

        case TipoToken::DELIMITADOR:
            return "DELIMITADOR";

        case TipoToken::DESCONOCIDO:
            return "DESCONOCIDO";

        case TipoToken::FIN_ARCHIVO:
            return "FIN_ARCHIVO";
    }

    return "DESCONOCIDO";
}

#endif