#include <string.h>
#include "lectura.h"
#include "util.h"
#include "cadena.h"
#include "socio.h"
#include "titulo.h"
#include "config.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    funcion generica de lectura de archivos de texto. Por cada
                linea no vacia: parsear -> validar -> escribir al final del
                archivo binario e insertar en el indice. Ante el primer error
                (de formato o de validacion) se rechaza la linea invocando
                rechazar y se continua con la siguiente.
Parametros:     path_txt: ruta del archivo de texto (CSV).
                arch_bin: archivo binario destino abierto en "w+b" o "r+b".
                indice: indice de la entidad (se usa para validar duplicados y
                se va completando).
                reg: buffer de un registro de la entidad.
                tamanyo: tamanyo en bytes del registro.
                fecha_proceso: fecha de proceso.
                parsear: convierte una linea en registro.
                validar: valida el registro y deja el tipo de error.
                rechazar: accion ante un rechazo (auditar, ignorar, etc.).
                obtener_clave: devuelve la clave del registro para el indice.
                aceptados, rechazados: salida; contadores de registros.
Retorno:        OK si se pudo abrir el archivo de texto, ERROR en caso
                contrario.
Observaciones:  Se reutiliza para Socios.csv y Titulos.csv; lo especifico de
                cada entidad lo resuelven los punteros a funcion.
******************************************************************************/
int leer_archivo_texto_ALU(const char *path_txt, FILE *arch_bin, t_indice *indice,
                       void *reg, size_t tamanyo, const t_fecha *fecha_proceso,
                       int (*parsear)(const char *, void *),
                       int (*validar)(void *, const t_contexto *, char *),
                       void (*rechazar)(const void *, const char *, const char *, const t_fecha *),
                       long (*obtener_clave)(const void *),
                       unsigned *aceptados, unsigned *rechazados)
{
    //texto_a_socio | texto_a_titulo

    //validar
    //int valida_titulo_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
    //int valida_socio_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
    // rechazar
    //int auditoria_registrar_ALU(const char *path, const char *dni_texto,
    //                    const char *tipo_error, const t_fecha *fecha_proceso)

    FILE *archTxt = fopen(path_txt,"r");
    if(!archTxt) return 0;

    char linea[512];

    t_contexto ctx;
    ctx.indice = indice;
    ctx.arch_bin = arch_bin;
    ctx.fecha_proceso = fecha_proceso;
    ctx.id_modificando = 0;

    while(fgets(linea,sizeof(linea),archTxt))
    {
        if(!parsear(linea,reg))
        {
            rechazar(reg,linea,"error",fecha_proceso);
        }
        else
        {

        }

    }

    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return leer_archivo_texto(path_txt, arch_bin, indice, reg, tamanyo, fecha_proceso, parsear, validar, rechazar, obtener_clave, aceptados, rechazados);
}

/******************************************************************************
Descripcion:    compara dos t_reg_indice por su clave id.
Parametros:     a, b: punteros a t_reg_indice.
Retorno:        Negativo si a < b, 0 si son iguales, positivo si a > b.
Observaciones:  Se usa como cmp en todas las operaciones del TDA indice.
******************************************************************************/
int compara_reg_indice_ALU(const void *a, const void *b)
{
    t_reg_indice *auxA = (t_reg_indice*)a;
    t_reg_indice *auxB = (t_reg_indice*)b;

    return auxA->id - auxB->id;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return compara_reg_indice(a, b);
}

/******************************************************************************
Descripcion:    construye los indices de socios y de titulos a partir de los
                archivos binarios existentes (solo registros activos).
Parametros:     ind_socios: indice de socios.
                ind_titulos: indice de titulos.
Retorno:        1 si ambos binarios existen y se cargaron, 0 en caso
                contrario.
Observaciones:  -
******************************************************************************/
int cargar_indices_ALU(t_indice *ind_socios, t_indice *ind_titulos)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return cargar_indices(ind_socios, ind_titulos);
}

/******************************************************************************
Descripcion:    opcion 1 del menu: lee Socios.csv y Titulos.csv con la funcion
                generica de lectura, genera socios.dat y titulos.dat
                (sobrescribiendolos) y reconstruye los indices desde los
                binarios generados.
Parametros:     ind_socios: indice de socios.
                ind_titulos: indice de titulos.
                fecha_proceso: fecha de proceso.
Retorno:        1 si los indices quedaron cargados, 0 en caso contrario.
Observaciones:  Los rechazos de socios se auditan en errores_socios.txt; los
                de titulos solo se cuentan.
******************************************************************************/
int generar_binarios_ALU(t_indice *ind_socios, t_indice *ind_titulos, const t_fecha *fecha_proceso)
{
    indice_vaciar_ALU(ind_socios);
    indice_vaciar_ALU(ind_titulos);

    FILE *archSocio = fopen(ARCH_SOCIOS_CSV,"wb+");
    if(!archSocio) return 0;

    t_socio socio;
    unsigned aceptados_socio = 0, rechazados_socio = 0;

    leer_archivo_texto_ALU(
        ARCH_SOCIOS_CSV,
        archSocio,
        ind_socios,
        &socio,
        sizeof(t_socio),
        fecha_proceso,
        texto_a_socio_ALU,
        valida_socio_ALU,
        rechaza_socio,
        clave_socio,
        &aceptados_socio,
        &rechazados_socio
    );

    fclose(archSocio);

    FILE *archTitulo = fopen(ARCH_TITULOS_CSV,"wb+");
    if(!archTitulo) return 0;

    t_titulo titulo_buf;
    unsigned aceptados_titulo = 0, rechazados_titulo = 0;

    leer_archivo_texto_ALU(
        ARCH_TITULOS_CSV,
        archTitulo,
        ind_titulos,
        &titulo_buf,
        sizeof(t_titulo),
        fecha_proceso,
        texto_a_titulo_ALU,
        valida_titulo_ALU,
        rechaza_titulo,
        clave_titulo,
        &aceptados_titulo,
        &rechazados_titulo
    );

    fclose(archTitulo);

    int cargados_socios = indice_cargar_ALU(ARCH_SOCIOS_BIN, ind_socios, sizeof(t_socio),clave_socio,activo_socio, compara_reg_indice_ALU);
    int cargados_titulos = indice_cargar_ALU(ARCH_TITULOS_BIN, ind_titulos, sizeof(t_titulo),clave_titulo,activo_titulo, compara_reg_indice_ALU);

    printf("\n--- REGENERACION DE ARCHIVOS BINARIOS EXITOSA ---\n");
    printf("Socios   -> Aceptados: %u | Rechazados: %u \n", aceptados_socio, rechazados_socio);
    printf("Titulos  -> Aceptados: %u | Rechazados: %u\n", aceptados_titulo, rechazados_titulo);

    return (cargados_socios && cargados_titulos);

    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return generar_binarios(ind_socios, ind_titulos, fecha_proceso);
}
