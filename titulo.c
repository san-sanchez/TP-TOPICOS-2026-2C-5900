#include <stdlib.h>
#include <string.h>
#include "titulo.h"
#include "cadena.h"
#include "util.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    convierte una linea CSV de 4 campos en t_titulo. Los campos
                numericos invalidos quedan en valores que fallan la validacion
                (id = 0, stock = -1). El estado se fija en 'A'.
Parametros:     linea: linea CSV.
                reg: puntero a t_titulo destino.
Retorno:        OK si la cantidad de campos es correcta, ERROR en caso
                contrario.
Observaciones:  Se pasa por puntero como parsear a leer_archivo_texto.
******************************************************************************/
int texto_a_titulo_ALU(const char *linea, void *reg)
{
    t_titulo *aux = (t_titulo*)reg;
    if(sscanf(linea,"%d;%[^;];%[^;];%d",&aux->id,aux->titulo,aux->genero,&aux->stock) != 4) return 0;
    return 0;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return texto_a_titulo(linea, reg);
}

/******************************************************************************
Descripcion:    aplica las validaciones del punto 6.2 en orden: ID > 0 y no
                existente en el indice, titulo normalizado no vacio, genero
                dentro de GENEROS[] y stock >= 0. Se detiene en el primer
                error.
Parametros:     reg: puntero a t_titulo (se modifica al normalizar el titulo).
                ctx: contexto con indice e id_modificando.
                tipo_error: salida; nombre de la validacion que fallo.
Retorno:        OK si el titulo es valido, ERROR en caso contrario.
Observaciones:  Se pasa por puntero como validar a leer_archivo_texto. Se reutiliza en las altas del
                lote de novedades.
******************************************************************************/
int valida_titulo_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
{
    t_titulo *titulo =(t_titulo*)reg;

    if(titulo->id <0)
    {
        strcpy(tipo_error,"ID");
        return 0;
    }
    t_reg_indice reg_busqueda;
    reg_busqueda.id = titulo->id;

    if(indice_buscar_ALU(ctx->indice,&reg_busqueda,sizeof(reg_busqueda),compara_reg_indice_ALU)==1)
    {
        strcpy(tipo_error, "ID_DUPLICADO");
        return 0;
    }

    arreglar_titulo(titulo->titulo);

    if(genero_titulo(titulo->genero)==0)
    {
        strcpy(tipo_error,"GENERO");
        return 0;
    }
    if(titulo->stock <0)
    {
        strcpy(tipo_error,"STOCK");
        return 0;
    }
    return 1;

    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return valida_titulo(reg, ctx, tipo_error);
}
int genero_titulo(const char *src)
{
    char *cad[CANT_GENEROS]={"Accion","Drama","Comedia","Terror","Ciencia Ficcion"};
    int i;

    for(i=0 ; i < CANT_GENEROS ; i++)
    {
        if(strcmp(src,cad[i]) == 0) return 1;

    }
    return 0;
}

/******************************************************************************
Descripcion:    recorre el indice (ordenado por ID y solo con activos) y lista
                los titulos leyendo cada registro del archivo.
Parametros:     indice: indice de titulos.
                path: ruta de titulos.dat.
Retorno:        OK si se pudo abrir el archivo, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int titulos_listar_ALU(const t_indice *indice, const char *path)
{
    FILE *arch = fopen(path,"rb");

    if(!arch) return 0;
    size_t i;
    unsigned nro_reg;
    t_titulo titulo;

    titulos_encabezado();

    for(i=0;i<indice->cantidad_elementos_actual;i++)
    {
        nro_reg = ((t_reg_indice *)indice->vindice)[i].nro_reg;
        if(titulo_leer(arch,nro_reg,&titulo)) return 0;
        titulo_mostrar(&titulo);
    }
    return 1;

    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return titulos_listar(indice, path);
}
