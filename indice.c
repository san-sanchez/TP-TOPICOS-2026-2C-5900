#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indice.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    toma memoria para nmemb elementos e inicializa la estructura
                vacia.
Parametros:     indice: TDA indice.
                nmemb: cantidad inicial de elementos (CANTIDAD_ELEMENTOS).
                tamanyo: espacio en bytes ocupado por cada elemento.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int indice_crear_ALU(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    size_t capacidad = (nmemb == 0) ? CANTIDAD_ELEMENTOS : nmemb;
    indice->vindice = malloc(capacidad * tamanyo);

    if(!indice->vindice)
        return ERROR;

    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = (unsigned)capacidad;
    return OK;
}

/******************************************************************************
Descripcion:    redimensiona el arreglo interno del indice con realloc. La
                nueva capacidad es la actual multiplicada por INCREMENTO (30%
                mas); la calcula la propia funcion.
Parametros:     indice: TDA indice.
                tamanyo: espacio en bytes ocupado por cada elemento.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario.
Observaciones:  Si el producto no aumenta la capacidad (arreglos muy chicos)
                se garantiza al menos un elemento mas.
******************************************************************************/
int indice_redimensionar_ALU(t_indice *indice, size_t tamanyo)
{
    void *aux;
    size_t nuevaCapacidad = (size_t)indice->cantidad_elementos_actual * (size_t)INCREMENTO;

    if((double)nuevaCapacidad <= (double)indice->cantidad_elementos_maxima)
    {
        nuevaCapacidad = (size_t)indice->cantidad_elementos_maxima + 1;
    }

    aux = realloc(indice->vindice, nuevaCapacidad * tamanyo);
    if(!aux)
        return ERROR;

    indice->vindice = aux;
    indice->cantidad_elementos_maxima = (unsigned)nuevaCapacidad;
    return OK;
}

/******************************************************************************
Descripcion:    inserta un elemento manteniendo el orden segun la clave. Si el
                arreglo esta lleno invoca indice_redimensionar.
Parametros:     indice: TDA indice.
                registro: elemento a insertar.
                tamanyo: espacio en bytes ocupado por el elemento.
                cmp: funcion de comparacion provista.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario
                (clave duplicada o falta de memoria).
Observaciones:  -
******************************************************************************/
int indice_insertar_ALU(t_indice *indice, const void *registro, size_t tamanyo,
                    int (*cmp)(const void *, const void *))
{
    const t_reg_indice *nuevo = (const t_reg_indice *)registro;
    unsigned i, pos;

    /* verificar duplicado: buscar la clave */
    for(i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        t_reg_indice *elem = (t_reg_indice *)((char *)indice->vindice + i * tamanyo);
        if(elem->id == nuevo->id)
            return ERROR; /* clave duplicada */
    }

    /* si esta lleno, redimensionar */
    if(indice_lleno_ALU(indice) == OK)
    {
        if(indice_redimensionar_ALU(indice, tamanyo) != OK)
            return ERROR;
    }

    /* encontrar posicion de insercion para mantener orden ascendente por id */
    pos = indice->cantidad_elementos_actual;
    for(i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        t_reg_indice *elem = (t_reg_indice *)((char *)indice->vindice + i * tamanyo);
        if(elem->id > nuevo->id)
        {
            pos = i;
            break;
        }
    }

    /* desplazar elementos desde pos hacia la derecha */
    for(i = indice->cantidad_elementos_actual; i > pos; i--)
    {
        t_reg_indice *dest = (t_reg_indice *)((char *)indice->vindice + i * tamanyo);
        t_reg_indice *src  = (t_reg_indice *)((char *)indice->vindice + (i - 1) * tamanyo);
        *dest = *src;
    }

    /* insertar el nuevo elemento */
    t_reg_indice *dest = (t_reg_indice *)((char *)indice->vindice + pos * tamanyo);
    *dest = *nuevo;

    indice->cantidad_elementos_actual++;
    return OK;
}

/******************************************************************************
Descripcion:    elimina el elemento cuya clave coincide con la de registro,
                desplazando los siguientes.
Parametros:     indice: TDA indice.
                registro: elemento a eliminar (solo importa su clave).
                tamanyo: espacio en bytes ocupado por el elemento.
                cmp: funcion de comparacion provista.
Retorno:        OK si la operacion fue exitosa y ERROR si la clave no existe.
Observaciones:  -
******************************************************************************/
int indice_eliminar_ALU(t_indice *indice, const void *registro, size_t tamanyo,
                    int (*cmp)(const void *, const void *))
{
    const t_reg_indice *clave = (const t_reg_indice *)registro;
    unsigned i, pos;
    int encontrado = 0;

    for(i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        t_reg_indice *elem = (t_reg_indice *)((char *)indice->vindice + i * tamanyo);
        if(elem->id == clave->id)
        {
            encontrado = 1;
            pos = i;
            break;
        }
    }

    if(!encontrado)
        return ERROR;

    /* desplazar elementos siguientes a la izquierda */
    for(i = pos + 1; i < indice->cantidad_elementos_actual; i++)
    {
        t_reg_indice *dest = (t_reg_indice *)((char *)indice->vindice + (i - 1) * tamanyo);
        t_reg_indice *src  = (t_reg_indice *)((char *)indice->vindice + i * tamanyo);
        *dest = *src;
    }

    indice->cantidad_elementos_actual--;
    return OK;
}

/******************************************************************************
Descripcion:    busca la clave de registro en el indice. Si existe copia el
                elemento completo (nro_reg incluido) en registro.
Parametros:     indice: TDA indice.
                registro: elemento a buscar (entrada/salida).
                tamanyo: espacio en bytes ocupado por el elemento.
                cmp: funcion de comparacion provista.
Retorno:        NO_EXISTE si la clave no existe; en caso contrario la posicion
                que ocupa dentro del arreglo.
Observaciones:  La comparacion entre claves se realiza exclusivamente mediante
                cmp.
******************************************************************************/
int indice_buscar_ALU(const t_indice *indice, void *registro, size_t tamanyo,
                  int (*cmp)(const void *, const void *))
{
    const t_reg_indice *clave = (const t_reg_indice *)registro;
    unsigned izq = 0, der = indice->cantidad_elementos_actual;

    while(izq < der)
    {
        unsigned medio = izq + (der - izq) / 2;
        t_reg_indice *elem = (t_reg_indice *)((char *)indice->vindice + medio * tamanyo);

        int c = cmp((const void *)clave, (const void *)elem);
        if(c == 0)
        {
            /* copiar el elemento completo en registro */
            t_reg_indice *dest = (t_reg_indice *)registro;
            *dest = *elem;
            return (int)medio;
        }
        else if(c < 0)
        {
            der = medio;
        }
        else
        {
            izq = medio + 1;
        }
    }

    return NO_EXISTE;
}

/******************************************************************************
Descripcion:    determina si el indice contiene 0 (cero) elementos.
Parametros:     indice: TDA indice.
Retorno:        OK si esta vacio, ERROR si no lo esta.
Observaciones:  -
******************************************************************************/
int indice_vacio_ALU(const t_indice *indice)
{
    return(!indice || indice->cantidad_elementos_actual == 0)? OK:ERROR;
}

/******************************************************************************
Descripcion:    determina si el indice alcanzo su capacidad maxima actual.
Parametros:     indice: TDA indice.
Retorno:        OK si esta lleno, ERROR si no lo esta.
Observaciones:  -
******************************************************************************/
int indice_lleno_ALU(const t_indice *indice)
{
    return (!indice || indice->cantidad_elementos_actual >= indice->cantidad_elementos_maxima)?OK:ERROR;
}

/******************************************************************************
Descripcion:    deja el indice vacio sin liberar la memoria del arreglo.
Parametros:     indice: TDA indice.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void indice_vaciar_ALU(t_indice *indice)
{
    if(!indice) return;
    indice->cantidad_elementos_actual = 0;
}

/******************************************************************************
Descripcion:    libera toda la memoria dinamica del indice.
Parametros:     indice: TDA indice.
Retorno:        No posee.
Observaciones:  Luego de invocarla el indice no puede usarse sin volver a
                llamar a indice_crear.
******************************************************************************/
void indice_destruir_ALU(t_indice *indice)
{
    if(!indice) return;
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = 0;
    free(indice->vindice);
    indice->vindice = NULL;
}

/******************************************************************************
Descripcion:    construye el indice a partir de un archivo binario de
                registros. Recorre el archivo secuencialmente y por cada
                registro activo inserta un t_reg_indice (nro_reg = posicion en
                el archivo, id = clave obtenida con obtener_clave).
Parametros:     path: ruta al archivo binario.
                indice: TDA indice (ya creado; se vacia antes de cargar).
                tamanyo_reg: tamanyo en bytes de cada registro del archivo.
                obtener_clave: funcion que devuelve la clave (long) de un
                registro.
                es_activo: funcion que devuelve distinto de 0 si el registro
                tiene estado 'A'.
                cmp: funcion de comparacion provista.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario.
Observaciones:  Los registros con estado 'B' se omiten. El archivo no necesita
                estar ordenado: el orden lo garantiza indice_insertar.
******************************************************************************/
int indice_cargar_ALU(const char *path, t_indice *indice, size_t tamanyo_reg,
                  long (*obtener_clave)(const void *),
                  int (*es_activo)(const void *),
                  int (*cmp)(const void *, const void *))
{
    FILE *arch = fopen(path, "rb");
    if(!arch)
        return ERROR;

    indice_vaciar_ALU(indice);

    void *registro = malloc(tamanyo_reg);
    if(!registro)
    {
        fclose(arch);
        return ERROR;
    }

    t_reg_indice reg_ind;
    unsigned nro_reg = 0;
    size_t leidos;

    while((leidos = fread(registro, tamanyo_reg, 1, arch)) == 1)
    {
        if(es_activo(registro))
        {
            reg_ind.nro_reg = nro_reg;
            reg_ind.id = obtener_clave(registro);

            if(indice_insertar_ALU(indice, &reg_ind, sizeof(t_reg_indice), cmp) != OK)
            {
                free(registro);
                fclose(arch);
                return ERROR;
            }
        }
        nro_reg++;
    }

    free(registro);
    fclose(arch);
    return OK;
}
