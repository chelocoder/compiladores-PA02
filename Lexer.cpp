#include "Lexer.h"

#include <cctype>


// =====================================
// CONSTRUCTOR
// =====================================

AnalizadorLexico::AnalizadorLexico(
    const string& codigoFuente
)
    : codigo(codigoFuente),
      posicion(0),
      linea(1),
      columna(1) {

    palabrasReservadas = {

        // Comandos
        "REGISTRAR",
        "VALIDAR",
        "CALCULAR",
        "EMITIR",
        "ANULAR",
        "CONSULTAR",

        // Datos del dominio municipal
        "FERIA",
        "RUC",
        "DNI",
        "AFORO",
        "ZONA",
        "RUBRO",
        "TASA",
        "CONSTANCIA",
        "REPORTE",
        "HORARIO"
    };
}


// =====================================
// CONTROL DE LECTURA
// =====================================

bool AnalizadorLexico::esFinal() const {

    return posicion >= codigo.size();
}


char AnalizadorLexico::actual() const {

    if (esFinal()) {
        return '\0';
    }

    return codigo[posicion];
}


char AnalizadorLexico::siguiente(
    size_t desplazamiento
) const {

    size_t destino =
        posicion + desplazamiento;

    if (destino >= codigo.size()) {
        return '\0';
    }

    return codigo[destino];
}


char AnalizadorLexico::avanzar() {

    if (esFinal()) {
        return '\0';
    }

    char c = codigo[posicion++];

    if (c == '\n') {

        linea++;
        columna = 1;

    } else {

        columna++;
    }

    return c;
}


// =====================================
// IDENTIFICADORES
// =====================================

bool AnalizadorLexico::esInicioIdentificador(
    char c
) const {

    return
        isalpha(
            static_cast<unsigned char>(c)
        ) || c == '_';
}


bool AnalizadorLexico::esParteIdentificador(
    char c
) const {

    return
        isalnum(
            static_cast<unsigned char>(c)
        ) || c == '_';
}


// =====================================
// ESPACIOS Y COMENTARIOS
// =====================================

void AnalizadorLexico::ignorarEspaciosYComentarios() {

    bool repetir = true;

    while (repetir && !esFinal()) {

        repetir = false;

        // Ignorar espacios, tabulaciones
        // y saltos de linea
        while (
            !esFinal() &&
            isspace(
                static_cast<unsigned char>(
                    actual()
                )
            )
        ) {

            avanzar();
        }


        // Comentarios de una linea //
        if (
            !esFinal() &&
            actual() == '/' &&
            siguiente() == '/'
        ) {

            while (
                !esFinal() &&
                actual() != '\n'
            ) {

                avanzar();
            }

            repetir = true;
        }
    }
}


// =====================================
// IDENTIFICADORES Y PALABRAS RESERVADAS
// =====================================

Token
AnalizadorLexico::
reconocerIdentificadorOPalabraReservada() {

    int lineaInicio = linea;
    int columnaInicio = columna;

    size_t inicio = posicion;

    while (
        !esFinal() &&
        esParteIdentificador(actual())
    ) {

        avanzar();
    }


    string lexema =
        codigo.substr(
            inicio,
            posicion - inicio
        );


    TipoToken tipo;

    if (
        palabrasReservadas.count(lexema)
    ) {

        tipo =
            TipoToken::PALABRA_RESERVADA;

    } else {

        tipo =
            TipoToken::IDENTIFICADOR;
    }


    return {
        tipo,
        lexema,
        lineaInicio,
        columnaInicio
    };
}


// =====================================
// NUMEROS
// =====================================

Token AnalizadorLexico::reconocerNumero() {

    int lineaInicio = linea;
    int columnaInicio = columna;

    size_t inicio = posicion;

    int cantidadPuntos = 0;


    while (!esFinal()) {

        char c = actual();


        if (
            isdigit(
                static_cast<unsigned char>(c)
            )
        ) {

            avanzar();
            continue;
        }


        if (c == '.') {

            cantidadPuntos++;

            avanzar();

            continue;
        }


        // Detectar identificador que empieza
        // con numero, por ejemplo: 2026feria
        if (esParteIdentificador(c)) {

            while (
                !esFinal() &&
                esParteIdentificador(actual())
            ) {

                avanzar();
            }


            string lexema =
                codigo.substr(
                    inicio,
                    posicion - inicio
                );


            registrarError(
                lexema,
                lineaInicio,
                columnaInicio,
                "Identificador invalido: "
                "no puede comenzar con un digito."
            );


            return {
                TipoToken::DESCONOCIDO,
                lexema,
                lineaInicio,
                columnaInicio
            };
        }


        break;
    }


    string lexema =
        codigo.substr(
            inicio,
            posicion - inicio
        );


    // Numero con mas de un punto
    if (cantidadPuntos > 1) {

        registrarError(
            lexema,
            lineaInicio,
            columnaInicio,
            "Numero invalido: contiene "
            "mas de un punto decimal."
        );


        return {
            TipoToken::DESCONOCIDO,
            lexema,
            lineaInicio,
            columnaInicio
        };
    }


    // Numero decimal
    if (cantidadPuntos == 1) {

        if (
            !lexema.empty() &&
            lexema.back() == '.'
        ) {

            registrarError(
                lexema,
                lineaInicio,
                columnaInicio,
                "Numero decimal invalido: "
                "faltan digitos despues del punto."
            );


            return {
                TipoToken::DESCONOCIDO,
                lexema,
                lineaInicio,
                columnaInicio
            };
        }


        return {
            TipoToken::DECIMAL,
            lexema,
            lineaInicio,
            columnaInicio
        };
    }


    // Numero entero
    return {
        TipoToken::ENTERO,
        lexema,
        lineaInicio,
        columnaInicio
    };
}


// =====================================
// CADENAS
// =====================================

Token AnalizadorLexico::reconocerCadena() {

    int lineaInicio = linea;
    int columnaInicio = columna;

    size_t inicio = posicion;


    // Consumir comilla inicial
    avanzar();


    while (
        !esFinal() &&
        actual() != '"' &&
        actual() != '\n'
    ) {

        avanzar();
    }


    // Cadena sin cerrar
    if (
        esFinal() ||
        actual() == '\n'
    ) {

        string lexema =
            codigo.substr(
                inicio,
                posicion - inicio
            );


        registrarError(
            lexema,
            lineaInicio,
            columnaInicio,
            "Cadena sin cerrar: "
            "falta la comilla final."
        );


        return {
            TipoToken::DESCONOCIDO,
            lexema,
            lineaInicio,
            columnaInicio
        };
    }


    // Consumir comilla final
    avanzar();


    string lexema =
        codigo.substr(
            inicio,
            posicion - inicio
        );


    return {
        TipoToken::CADENA,
        lexema,
        lineaInicio,
        columnaInicio
    };
}


// =====================================
// OPERADORES Y DELIMITADORES
// =====================================

Token
AnalizadorLexico::
reconocerOperadorODelimitador() {

    int lineaInicio = linea;
    int columnaInicio = columna;

    size_t inicio = posicion;


    // Operadores de dos caracteres
    string dosCaracteres;

    dosCaracteres += actual();
    dosCaracteres += siguiente();


    if (
        dosCaracteres == "==" ||
        dosCaracteres == "<=" ||
        dosCaracteres == ">=" ||
        dosCaracteres == "!="
    ) {

        avanzar();
        avanzar();


        return {
            TipoToken::OPERADOR,
            dosCaracteres,
            lineaInicio,
            columnaInicio
        };
    }


    char c = actual();


    // Asignacion
    if (c == '=') {

        avanzar();


        return {
            TipoToken::ASIGNACION,
            "=",
            lineaInicio,
            columnaInicio
        };
    }


    // Operadores simples
    if (
        c == '<' ||
        c == '>' ||
        c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/'
    ) {

        avanzar();


        return {
            TipoToken::OPERADOR,
            string(1, c),
            lineaInicio,
            columnaInicio
        };
    }


    // Delimitadores
    if (
        c == ';' ||
        c == ',' ||
        c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}'
    ) {

        avanzar();


        return {
            TipoToken::DELIMITADOR,
            string(1, c),
            lineaInicio,
            columnaInicio
        };
    }


    // Simbolo no reconocido
    avanzar();


    string lexema =
        codigo.substr(
            inicio,
            posicion - inicio
        );


    registrarError(
        lexema,
        lineaInicio,
        columnaInicio,
        "Caracter no reconocido "
        "por la especificacion lexica."
    );


    return {
        TipoToken::DESCONOCIDO,
        lexema,
        lineaInicio,
        columnaInicio
    };
}


// =====================================
// REGISTRO DE ERRORES
// =====================================

void AnalizadorLexico::registrarError(
    const string& lexema,
    int lineaError,
    int columnaError,
    const string& mensaje
) {

    errores.push_back({
        lexema,
        lineaError,
        columnaError,
        mensaje
    });
}


// =====================================
// VALIDACION DE RUC
// =====================================

bool AnalizadorLexico::esRUCValido(
    const string& valor
) const {

    if (valor.length() != 11) {
        return false;
    }


    for (char c : valor) {

        if (
            !isdigit(
                static_cast<unsigned char>(c)
            )
        ) {

            return false;
        }
    }


    return true;
}


// =====================================
// VALIDACION DE DNI
// =====================================

bool AnalizadorLexico::esDNIValido(
    const string& valor
) const {

    if (valor.length() != 8) {
        return false;
    }


    for (char c : valor) {

        if (
            !isdigit(
                static_cast<unsigned char>(c)
            )
        ) {

            return false;
        }
    }


    return true;
}


// =====================================
// VALIDACION DE HORARIO
// =====================================

bool AnalizadorLexico::esHorarioValido(
    const string& valor
) const {

    // Formato:
    // HH:MM-HH:MM

    if (valor.length() != 11) {
        return false;
    }


    if (
        valor[2] != ':' ||
        valor[5] != '-' ||
        valor[8] != ':'
    ) {

        return false;
    }


    const int posicionesDigitos[] = {
        0, 1,
        3, 4,
        6, 7,
        9, 10
    };


    for (int i : posicionesDigitos) {

        if (
            !isdigit(
                static_cast<unsigned char>(
                    valor[i]
                )
            )
        ) {

            return false;
        }
    }


    int horaInicio =
        (valor[0] - '0') * 10 +
        (valor[1] - '0');


    int minutoInicio =
        (valor[3] - '0') * 10 +
        (valor[4] - '0');


    int horaFin =
        (valor[6] - '0') * 10 +
        (valor[7] - '0');


    int minutoFin =
        (valor[9] - '0') * 10 +
        (valor[10] - '0');


    return
        horaInicio <= 23 &&
        horaFin <= 23 &&
        minutoInicio <= 59 &&
        minutoFin <= 59;
}


// =====================================
// VALIDACIONES SEGUN EL CONTEXTO
// =====================================

void
AnalizadorLexico::
validarCadenaSegunContexto(
    const Token& token
) {

    if (
        token.tipo != TipoToken::CADENA ||
        token.lexema.size() < 2
    ) {

        return;
    }


    string palabraRelacionada;


    // Buscar hacia atras la ultima
    // palabra reservada reconocida
    for (
        auto it = tokens.rbegin();
        it != tokens.rend();
        ++it
    ) {

        if (
            it->tipo ==
            TipoToken::PALABRA_RESERVADA
        ) {

            palabraRelacionada =
                it->lexema;

            break;
        }
    }


    // Quitar las comillas
    string valor =
        token.lexema.substr(
            1,
            token.lexema.size() - 2
        );


    // Validar RUC
    if (
        palabraRelacionada == "RUC" &&
        !esRUCValido(valor)
    ) {

        registrarError(
            token.lexema,
            token.linea,
            token.columna,
            "RUC invalido: debe contener "
            "exactamente 11 digitos."
        );
    }


    // Validar DNI
    else if (
        palabraRelacionada == "DNI" &&
        !esDNIValido(valor)
    ) {

        registrarError(
            token.lexema,
            token.linea,
            token.columna,
            "DNI invalido: debe contener "
            "exactamente 8 digitos."
        );
    }


    // Validar HORARIO
    else if (
        palabraRelacionada == "HORARIO" &&
        !esHorarioValido(valor)
    ) {

        registrarError(
            token.lexema,
            token.linea,
            token.columna,
            "Horario invalido: use el formato "
            "HH:MM-HH:MM con horas "
            "y minutos validos."
        );
    }
}


// =====================================
// ANALISIS LEXICO
// =====================================

vector<Token> AnalizadorLexico::analizar() {

    tokens.clear();
    errores.clear();


    while (!esFinal()) {

        ignorarEspaciosYComentarios();


        if (esFinal()) {
            break;
        }


        Token token;

        char c = actual();


        // Identificador o palabra reservada
        if (esInicioIdentificador(c)) {

            token =
                reconocerIdentificadorOPalabraReservada();
        }


        // Numero
        else if (
            isdigit(
                static_cast<unsigned char>(c)
            )
        ) {

            token =
                reconocerNumero();
        }


        // Cadena
        else if (c == '"') {

            token =
                reconocerCadena();

            validarCadenaSegunContexto(
                token
            );
        }


        // Operador, delimitador
        // o simbolo desconocido
        else {

            token =
                reconocerOperadorODelimitador();
        }


        tokens.push_back(token);
    }


    tokens.push_back({

        TipoToken::FIN_ARCHIVO,

        "",

        linea,

        columna
    });


    return tokens;
}


// =====================================
// OBTENER ERRORES
// =====================================

vector<ErrorLexico>
AnalizadorLexico::obtenerErrores() const {

    return errores;
}