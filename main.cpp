#include <iostream>
#include <vector>

#include "Lexer.h"

using namespace std;


int main() {

    string codigo = R"(

        // VALIDOS
        REGISTRAR FERIA = "Ichilashun Oroyata";
        VALIDAR RUC = "20123456789";
        VALIDAR DNI = "70123456";
        VALIDAR AFORO = 80;
        VALIDAR ZONA = "La Oroya Antigua";
        VALIDAR RUBRO = "Gastronomia";
        VALIDAR HORARIO = "09:00-18:00";
        CALCULAR TASA = 145.50;
        EMITIR CONSTANCIA("FS-001");
        ANULAR CONSTANCIA("FS-002");
        CONSULTAR REPORTE("SEMANA_SANTA_2026");


        // INVALIDOS
        REGISTRAR 2026feria = "Ichilashun Oroyata";
        VALIDAR RUC = "2012345678";
        VALIDAR DNI = "7012345";
        VALIDAR AFORO = 8@;
        VALIDAR ZON@ = "La Oroya Antigua";
        VALIDAR RUBR@ = "Gastronomia";
        VALIDAR HORARIO = "9:00-18:75";
        CALCULAR TASA = 145.50.25;
        EMITIR CONSTANCIA("FS-001);
        ANULAR CONSTANCIA = FS#002;
        CONSULTAR REPORTE("SEMANA_SANTA_2026);

    )";


    // Crear analizador lexico
    AnalizadorLexico analizador(codigo);


    // Analizar codigo fuente
    vector<Token> tokens =
        analizador.analizar();


    // =====================================
    // TOKENS
    // =====================================

    cout
        << "=== TOKENS RECONOCIDOS ==="
        << "\n\n";


    for (const Token& token : tokens) {

        if (
            token.tipo ==
            TipoToken::FIN_ARCHIVO
        ) {

            continue;
        }


        cout
            << "<"
            << tipoTokenAString(
                   token.tipo
               )
            << ", "
            << token.lexema
            << "> [linea "
            << token.linea
            << ", columna "
            << token.columna
            << "]"
            << '\n';
    }


    // =====================================
    // ERRORES LEXICOS
    // =====================================

    vector<ErrorLexico> errores =
        analizador.obtenerErrores();


    cout
        << "\n=== ERRORES LEXICOS ===\n";


    if (errores.empty()) {

        cout
            << "\nNo se encontraron "
            << "errores lexicos.\n";

    } else {

        for (
            const ErrorLexico& error :
            errores
        ) {

            cout
                << "\nLexema:  "
                << error.lexema
                << '\n';

            cout
                << "Linea:   "
                << error.linea
                << '\n';

            cout
                << "Columna: "
                << error.columna
                << '\n';

            cout
                << "Mensaje: "
                << error.mensaje
                << '\n';
        }
    }


    return 0;
}