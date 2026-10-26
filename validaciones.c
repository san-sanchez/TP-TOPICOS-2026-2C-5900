#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "validaciones.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    calcula el CUIL con el algoritmo modulo 11 del Anexo A.1:
                prefijo 20 (hombre) o 27 (mujer), DNI a 8 digitos y digito
                verificador. Si el resto es 1 el prefijo pasa a 23 y Z es 9
                (hombre) o 4 (mujer).
Parametros:     dni: numero de documento.
                sexo: 'H' o 'M'.
                dest: buffer de al menos TAM_CUIL bytes donde se deja "XY-
                NNNNNNNN-Z".
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void calcular_cuil_ALU(long dni, char sexo, char *dest)
{
    char cuil[TAM_CUIL];
    int peso[TAM_VEC]={5,4,3,2,7,6,5,4,3,2};
    int prefijo;
    int clave;
    int aux=0;
    int i;

    prefijo = sexo == 'H' ? 20 : 27 ;

    sprintf(cuil,"%02d%ld",prefijo,dni);

    for(i=0; i < TAM_VEC ;i++)
    {
        aux += (cuil[i] - '0') * peso[i];
    }

    aux = aux % 11;

    if(aux == 1)
    {
        if(sexo == 'H')
        {
            prefijo = 23;
            clave = 9;
        }else{
            prefijo = 27;
            clave = 4;
        }
    }else if(aux == 0){
        clave = 0;
    }else{
        clave = 11 - aux;
    }
    sprintf(dest,"%02d-%08ld-%d",prefijo,dni,clave);
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    calcular_cuil(dni, sexo, dest);
}

char *caracter_unico(const char *src, char c)
{
    char *caracter = NULL;
    int unico = 0;

    while (*src)
    {
        if (*src == c)
        {
            unico++;
            if (unico == 1)
                caracter = (char *)src;
        }
        src++;
    }

    return unico == 1 ? caracter : NULL;
}
int tiene_espacio(const char *cadena)
{
    while (*cadena)
    {
        if (isspace((unsigned char)*cadena))
            return 1;

        cadena++;
    }

    return 0;
}
/******************************************************************************
Descripcion:    valida un email segun el Anexo A.3: hasta 30 caracteres sin
                espacios, una unica '@', parte local de letras/digitos/puntos
                que no empieza ni termina con punto, y dominio de
                letras/digitos/guiones/puntos con al menos un punto, que no
                empieza ni termina con punto o guion y cuyo ultimo segmento
                tiene al menos 2 letras.
Parametros:     email: cadena a validar.
Retorno:        1 si es valido, 0 en caso contrario.
Observaciones:  No verifica unicidad; eso lo hace socio_email_existe.
******************************************************************************/
int email_valido_ALU(const char *email)
{
    size_t caracteres = strlen(email);
    char *pIni = (char*)email;
    char *aux;

    if(caracteres > EMAIL_MAX_LEN) return 0;/// email mas largo de lo establecido
    if(tiene_espacio(email)) return 0;///existencia ' ' en alguna parte del email

    char *c = caracter_unico(email,'@');
    if(!c) return 0;/// no tiene @ u posee varios

    if(email == c || *(c + 1) == '\0') return 0;/// no pose un caracter anterior

    while(*email)
    {
        if(email == c)
        {
            email++;
            continue;
        }
        if(email < c)
        {
            ///PARTE LOCAL
            if((email == pIni && *email == '.') || (email == (c-1)&& *email == '.')) return 0;
            if (!isalnum((unsigned char)*email) && *email != '.') return 0;
        }
        else
        {
            ///PARTE DOMINIO
            if(*email=='.'&&(*(email-1)=='-'|| *(email+1)=='-')) return 0;
            if(email == c + 1 && (*email == '.' || *email == '-')) return 0;
            if (!isalnum((unsigned char)*email) && *email != '.' && *email != '-') return 0;
        }
        email++;
    }
    if(*(email - 1) == '.' || *(email - 1) == '-') return 0;///EXISTE COMO ULTIMO CARACTER '.' U '-'


    aux = strrchr(pIni,'.'); ///BUSCO EL ULTIMO PUNTO
    if(!aux || aux < c) return 0;/// SI NO EXISTE U EXISTE '.' EL LA PARTE LOCAL SE DA POR INVALIDO

    aux++;
    if((email - aux) < 2) return 0;

    while(aux < email)
    {
        if(!isalpha((unsigned char)*aux)) return 0;
        aux++;
    }

    return 1;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return email_valido(email);
}
