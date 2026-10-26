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
    int prefijo = (sexo == 'M') ? PREFIJO_MUJER : PREFIJO_HOMBRE;
    long dni_abs = (dni < 0) ? -dni : dni;
    int digitos[10];
    int i;

    /* formar los 10 digitos: prefijo (2) + dni (8) */
    digitos[0] = prefijo / 10;
    digitos[1] = prefijo % 10;

    for(i = 7; i >= 0; i--)
    {
        digitos[2 + i] = dni_abs % 10;
        dni_abs /= 10;
    }

    /* pesos de izquierda a derecha: 5,4,3,2,7,6,5,4,3,2 */
    int pesos[10] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    int suma = 0;
    for(i = 0; i < 10; i++)
        suma += digitos[i] * pesos[i];

    int resto = suma % 11;
    int z;

    if(resto == 0)
    {
        z = 0;
    }
    else if(resto == 1)
    {
        prefijo = PREFIJO_RESTO1; /* 23 */
        z = (sexo == 'M') ? 4 : 9;
    }
    else
    {
        z = 11 - resto;
    }

    sprintf(dest, "%02d-%08ld-%d", prefijo, (long)dni, z);
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
    int len, i;
    int arrobas = 0;
    const char *arroba = NULL;
    const char *local, *dominio;
    int punto_local = 0, punto_dom = 0;
    int seg_puntos = 0;
    int ultima_letra = 0;

    if(!email)
        return 0;

    len = (int)strlen(email);
    if(len == 0 || len > EMAIL_MAX_LEN)
        return 0;

    if(tiene_espacio(email))
        return 0;

    /* contar arrobas y encontrar la unica */
    for(i = 0; i < len; i++)
    {
        if(email[i] == '@')
        {
            arrobas++;
            arroba = &email[i];
        }
    }

    if(arrobas != 1)
        return 0;

    local = email;
    dominio = arroba + 1;
    int local_len = (int)(arroba - email);
    int dom_len = len - local_len - 1;

    if(local_len == 0 || dom_len == 0)
        return 0;

    /* parte local: letras/digitos/puntos, no empieza ni termina con punto */
    if(local[0] == '.' || local[local_len - 1] == '.')
        return 0;

    for(i = 0; i < local_len; i++)
    {
        char c = local[i];
        if(!(isalnum((unsigned char)c) || c == '.'))
            return 0;
        if(c == '.')
            punto_local = 1;
    }

    /* no dos puntos seguidos en local (opcional pero razonable) */
    if(punto_local)
    {
        for(i = 0; i + 1 < local_len; i++)
        {
            if(local[i] == '.' && local[i+1] == '.')
                return 0;
        }
    }

    /* dominio: letras/digitos/guiones/puntos, al menos un punto */
    if(dominio[0] == '.' || dominio[0] == '-' ||
       dominio[dom_len - 1] == '.' || dominio[dom_len - 1] == '-')
        return 0;

    for(i = 0; i < dom_len; i++)
    {
        char c = dominio[i];
        if(!(isalnum((unsigned char)c) || c == '.' || c == '-'))
            return 0;
        if(c == '.')
            punto_dom++;
    }

    if(punto_dom == 0)
        return 0;

    /* no puntos o guiones seguidos, ni punto-guion, etc. */
    for(i = 0; i + 1 < dom_len; i++)
    {
        if((dominio[i] == '.' && dominio[i+1] == '.') ||
           (dominio[i] == '-' && dominio[i+1] == '-') ||
           (dominio[i] == '.' && dominio[i+1] == '-') ||
           (dominio[i] == '-' && dominio[i+1] == '.'))
            return 0;
    }

    /* ultimo segmento al menos 2 letras */
    /* encontrar el ultimo punto */
    const char *ultimo_punto = caracter_unico(dominio, '.'); /* no sirve, busca unica */
    /* buscar el ultimo punto manualmente */
    {
        const char *p = dominio + dom_len - 1;
        const char *ultimo = NULL;
        while(p >= dominio)
        {
            if(*p == '.')
            {
                ultimo = p;
                break;
            }
            p--;
        }
        if(!ultimo)
            return 0;

        const char *segmento = ultimo + 1;
        int seg_len = (int)(dominio + dom_len - segmento);
        if(seg_len < 2)
            return 0;

        /* al menos 2 letras (no solo numeros) */
        int letras = 0;
        for(i = 0; i < seg_len; i++)
        {
            if(isalpha((unsigned char)segmento[i]))
                letras++;
        }
        if(letras < 2)
            return 0;
    }

    return 1;
}

/******************************************************************************
Descripcion:    devuelve 1 si la cadena contiene al menos un espacio.
******************************************************************************/
int tiene_espacio(const char *src)
{
    if(!src)
        return 0;
    while(*src)
    {
        if(*src == ' ')
            return 1;
        src++;
    }
    return 0;
}

/******************************************************************************
Descripcion:    devuelve puntero a la primera aparicion de c en src, o NULL.
******************************************************************************/
char *caracter_unico(const char *src, char c)
{
    if(!src)
        return NULL;
    while(*src)
    {
        if(*src == c)
            return (char *)src;
        src++;
    }
    return NULL;
}
