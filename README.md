# PA2 - Analizador léxico para ferias de Yauli - La Oroya

Caso automatización de validación para licencias de la Municipalidad Provincial de Yauli - La Oroya.

## Comandos
REGISTRAR, VALIDAR, CALCULAR, EMITIR, ANULAR, CONSULTAR.

## Dominio
Ferias temporales con sector gastronómico:
- Ichilashun Oroyata
- Feria Provincial Altoandina Yauli - La Oroya
- Feria Gastronómica por Fiestas Patrias

## Compilación
### g++
g++ -std=c++17 main.cpp Lexer.cpp -o pa2_lexer

### MSVC
cl /EHsc /std:c++17 main.cpp Lexer.cpp
