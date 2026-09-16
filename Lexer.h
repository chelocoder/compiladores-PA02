#ifndef LEXER_H
#define LEXER_H

#include "Token.h"

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct ErrorLexico {
    string lexema;
    int linea;
    int columna;
    string mensaje;
};

class AnalizadorLexico {

private:

    string codigo;
    size_t posicion;

    int linea;
    int columna;

    vector<Token> tokens;
    vector<ErrorLexico> errores;

    unordered_set<string> palabrasReservadas;


    // Control de lectura
    bool esFinal() const;

    char actual() const;

    char siguiente(size_t desplazamiento = 1) const;

    char avanzar();


    // Identificadores
    bool esInicioIdentificador(char c) const;

    bool esParteIdentificador(char c) const;


    // Reconocimiento de tokens
    void ignorarEspaciosYComentarios();

    Token reconocerIdentificadorOPalabraReservada();

    Token reconocerNumero();

    Token reconocerCadena();

    Token reconocerOperadorODelimitador();


    // Manejo de errores
    void registrarError(
        const string& lexema,
        int lineaError,
        int columnaError,
        const string& mensaje
    );


    // Validaciones propias de MuniLic Yauli
    bool esRUCValido(const string& valor) const;

    bool esDNIValido(const string& valor) const;

    bool esHorarioValido(const string& valor) const;

    void validarCadenaSegunContexto(const Token& token);


public:

    explicit AnalizadorLexico(
        const string& codigoFuente
    );

    vector<Token> analizar();

    vector<ErrorLexico> obtenerErrores() const;
};

#endif