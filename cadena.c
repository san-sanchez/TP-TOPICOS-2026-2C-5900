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
    char aux[256];
    char *apyn = s;
    char *coma = strchr(s, ',');

    if(coma)
    {
        /* formato: "APELLIDOS, NOMBRES" */
        *coma = '\0';
        char *apellido = s;
        char *nombre = coma + 1;
        /* quitar espacios sobrantes alrededor */
        while(*apellido == ' ') apellido++;
        while(*nombre == ' ') nombre++;
        /* construir: "Apellido Nombre" */
        int i = 0, j = 0;
        /* copiar apellido */
        while(apellido[i] && apellido[i] != ' ')
        {
            aux[j++] = (i == 0) ? toupper((unsigned char)apellido[i]) : tolower((unsigned char)apellido[i]);
            i++;
        }
        if(apellido[i] == ' ')
        {
            aux[j++] = ' ';
            i++;
            while(apellido[i] == ' ') i++;
            while(apellido[i])
            {
                aux[j++] = tolower((unsigned char)apellido[i]);
                i++;
            }
        }
        aux[j++] = ' ';
        /* copiar nombre */
        i = 0;
        while(nombre[i] == ' ') i++;
        int primera = 1;
        while(nombre[i])
        {
            if(nombre[i] == ' ')
            {
                aux[j++] = ' ';
                i++;
                while(nombre[i] == ' ') i++;
                primera = 1;
                continue;
            }
            aux[j++] = (primera) ? toupper((unsigned char)nombre[i]) : tolower((unsigned char)nombre[i]);
            primera = 0;
            i++;
        }
        aux[j] = '\0';
    }
    else
    {
        /* formato sin coma: normalizar palabra por palabra */
        int i = 0, j = 0, primera = 1;
        while(s[i])
        {
            if(s[i] == ' ')
            {
                aux[j++] = ' ';
                i++;
                while(s[i] == ' ') i++;
                primera = 1;
                continue;
            }
            aux[j++] = (primera) ? toupper((unsigned char)s[i]) : tolower((unsigned char)s[i]);
            primera = 0;
            i++;
        }
        aux[j] = '\0';
    }

    /* quitar espacio final si lo hay */
    int len = (int)strlen(aux);
    if(len > 0 && aux[len-1] == ' ')
        aux[len-1] = '\0';

    strcpy(s, aux);
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
    int i = 0;
    campos[0] = linea;

    for(char *p = linea; *p && i < max - 1; p++)
    {
        if(*p == SEPARADOR_CSV)
        {
            *p = '\0';
            i++;
            campos[i] = p + 1;
        }
    }

    return i + 1;
}
