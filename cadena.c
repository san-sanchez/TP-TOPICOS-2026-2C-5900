#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "cadena.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    normaliza un campo Apellidos y Nombres segun el Anexo A.2 (con
                regla de coma).
Parametros:     s: cadena a normalizar (se modifica in place).
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void arreglar_apyn_ALU(char *s)
{
    if (!s || *s == '\0') return;

    char *lect = s;
    char *esc = s;
    int posicion_palabra = 0;
    int coma_insertada = 0;

    // Omitir espacios iniciales
    while (*lect && isspace((unsigned char)*lect))
    {
        lect++;
    }

    while (*lect)
    {
        // Ignorar espacios múltiples o comas sobrantes mientras buscamos una palabra válida
        while (*lect && (isspace((unsigned char)*lect) || *lect == ','))
        {
            if (*lect == ',')
            {
                coma_insertada = 1; // Si ya venía con coma, registramos que el apellido ya está separado
            }
            lect++;
        }

        if (!*lect) break; // Fin de cadena

        posicion_palabra++;

        // Manejo de espacios y comas entre palabras según Anexo A.2
        if (posicion_palabra > 1)
        {
            if (posicion_palabra == 2 && !coma_insertada)
            {
                // Si es la segunda palabra y la entrada original no tenía coma, se la ponemos al apellido
                *esc++ = ',';
                *esc++ = ' ';
                coma_insertada = 1;
            }
            else
            {
                // Entre nombres subsiguientes va un único espacio
                *esc++ = ' ';
            }
        }

        // Capitalizar la palabra: Primera letra Mayúscula, el resto Minúsculas
        int primera_letra = 1;
        while (*lect && !isspace((unsigned char)*lect) && *lect != ',')
        {
            if (primera_letra)
            {
                *esc = (char)toupper((unsigned char)*lect);
                primera_letra = 0;
            }
            else
            {
                *esc = (char)tolower((unsigned char)*lect);
            }
            esc++;
            lect++;
        }
    }

    *esc = '\0'; // Fin de cadena normalizada
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //arreglar_apyn(s);
}

/******************************************************************************
Descripcion:    separa una linea CSV in place por SEPARADOR_CSV, reemplazando
                cada separador por '\0' y guardando en campos[] el puntero al
                inicio de cada campo.
Parametros:     linea: linea a separar (se modifica).
                campos: vector donde se guardan los punteros a cada campo.
                max: cantidad maxima de campos a separar.
Retorno:        Cantidad de campos encontrados (los vacios cuentan). Nunca mas
                de max.
Observaciones:  Si la linea tiene mas separadores que max, el ultimo campo
                conserva el resto de la linea.
******************************************************************************/
int separar_campos_ALU(char *linea, char *campos[], int max)
{
    int cant_campos = 0;
    if (!linea || max <= 0) return 0;

    // El primer campo arranca al inicio de la línea
    campos[cant_campos++] = linea;

    char *p = linea;
    while (*p && cant_campos < max) {
        if (*p == ';') {
            *p = '\0'; // Reemplazamos el separador por el centinela
            campos[cant_campos++] = p + 1; // El sig campo arranca después del ';'ahora'\0'
        }
        p++;
    }

    return cant_campos;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return separar_campos(linea, campos, max);
}
