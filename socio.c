#include <stdlib.h>
#include <string.h>
#include "socio.h"
#include "cadena.h"
#include "util.h"
#include "auditoria.h"
#include "config.h"
#include "menu.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    convierte una linea CSV de 10 campos en t_socio. Los campos
                con formato invalido quedan con valores que haran fallar la
                validacion correspondiente (dni = 0, fecha 0/0/0, sexo '?',
                etc.). El estado se fija en 'A'.
Parametros:     linea: linea CSV.
                reg: puntero a t_socio destino.
Retorno:        OK si la cantidad de campos es correcta, ERROR en caso
                contrario.
Observaciones:  Se pasa por puntero como parsear a leer_archivo_texto.
******************************************************************************/
int texto_a_socio_ALU(const char *linea, void *reg)
{
    t_socio *s = (t_socio *)reg;
    inicializar_socio_default(s);

    char aux_linea[TAM_LINEA];
    strncpy(aux_linea, linea, TAM_LINEA - 1);
    aux_linea[TAM_LINEA - 1] = '\0';
    char *p = strrchr(linea, '\n');
    if (p) *p = '\0';

    if (aux_linea[0] == '\0') return 0; // ERROR


    char *campos[10];
    int cant_campos = separar_campos_ALU(aux_linea, campos, 10);

    if (cant_campos < 9) return 0;

    asignar_campos_socio(s, campos, cant_campos);
    s->estado = 'A';
    return 1;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return texto_a_socio(linea, reg);
}
void inicializar_socio_default(t_socio *s)
{
    s->dni = 0;
    strcpy(s->cuil, "");
    strcpy(s->apellidos_nombres, "");
    s->fecha_nacimiento.dia = 0; s->fecha_nacimiento.mes = 0; s->fecha_nacimiento.anio = 0;
    s->sexo = '?';
    s->fecha_afiliacion.dia = 0; s->fecha_afiliacion.mes = 0; s->fecha_afiliacion.anio = 0;
    strcpy(s->categoria, "");
    s->fecha_ultima_cuota.dia = 0; s->fecha_ultima_cuota.mes = 0; s->fecha_ultima_cuota.anio = 0;
    s->estado = 'A';
    strcpy(s->plan, "");
    s->email_tutor[0] = '\0';
}
void asignar_campos_socio(t_socio *s, char *campos[], int cant_campos)
{
    // 1. DNI
    if (campos[0] && campos[0][0] != '\0') {
        s->dni = atol(campos[0]);
    }

    // 2. CUIL
    if (campos[1] && campos[1][0] != '\0') {
        strncpy(s->cuil, campos[1], TAM_CUIL - 1);
        s->cuil[TAM_CUIL - 1] = '\0';
    }

    // 3. Apellidos y Nombres -> Aplicamos la normalización oficial (Anexo A.2)
    if (campos[2] && campos[2][0] != '\0') {
        arreglar_apyn_ALU(campos[2]);
        strncpy(s->apellidos_nombres, campos[2], TAM_APYN - 1);
        s->apellidos_nombres[TAM_APYN - 1] = '\0';
    }

    // 4. Fecha de Nacimiento
    if (campos[3] && campos[3][0] != '\0') {
        sscanf(campos[3], "%d/%d/%d", &s->fecha_nacimiento.dia, &s->fecha_nacimiento.mes, &s->fecha_nacimiento.anio);
    }

    // 5. Sexo
    if (campos[4] && campos[4][0] != '\0') {
        s->sexo = campos[4][0];
    }

    // 6. Fecha de Afiliación
    if (campos[5] && campos[5][0] != '\0') {
        sscanf(campos[5], "%d/%d/%d", &s->fecha_afiliacion.dia, &s->fecha_afiliacion.mes, &s->fecha_afiliacion.anio);
    }

    // 7. Categoría
    if (campos[6] && campos[6][0] != '\0') {
        strncpy(s->categoria, campos[6], TAM_CATEGORIA - 1);
        s->categoria[TAM_CATEGORIA - 1] = '\0';
    }

    // 8. Fecha de Última Cuota
    if (campos[7] && campos[7][0] != '\0') {
        sscanf(campos[7], "%d/%d/%d", &s->fecha_ultima_cuota.dia, &s->fecha_ultima_cuota.mes, &s->fecha_ultima_cuota.anio);
    }

    // 9. Plan
    if (campos[8] && campos[8][0] != '\0') {
        strncpy(s->plan, campos[8], TAM_PLAN - 1);
        s->plan[TAM_PLAN - 1] = '\0';
    }

    // 10. Email del tutor (opcional para menores)
    if (cant_campos >= 10 && campos[9] && campos[9][0] != '\0') {
        strncpy(s->email_tutor, campos[9], TAM_EMAIL - 1);
        s->email_tutor[TAM_EMAIL - 1] = '\0';
    }
}
/******************************************************************************
Descripcion:    aplica las validaciones del punto 6.1 en orden (DNI, Sexo,
                CUIL, Apellidos y Nombres, Fecha de nacimiento, Fecha de
                afiliacion, Categoria, Fecha de ultima cuota, Plan, Email
                tutor). Normaliza apellidos y nombres. Se detiene en el primer
                error.
Parametros:     reg: puntero a t_socio a validar (se modifica al normalizar).
                ctx: contexto con indice, archivo binario, fecha de proceso e
                id_modificando.
                tipo_error: salida; nombre de la validacion que fallo.
Retorno:        OK si el socio es valido, ERROR en caso contrario.
Observaciones:  Si ctx->id_modificando == dni no se considera duplicado y se
                excluye ese DNI al verificar unicidad del email. Cumple con el
                puntero validar de leer_archivo_texto.
******************************************************************************/
int valida_socio_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return valida_socio(reg, ctx, tipo_error);
}

/******************************************************************************
Descripcion:    lee el registro nro_reg del archivo binario de socios.
Parametros:     arch: archivo abierto en modo binario.
                nro_reg: numero de registro.
                s: socio destino.
Retorno:        OK si se pudo leer, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_leer_ALU(FILE *arch, unsigned nro_reg, t_socio *s)
{
    if(!arch || !s) return 0;

    if(!fseek(arch,(long)(nro_reg*sizeof(t_socio)),SEEK_SET)) return 0;

    if(!fread(s,sizeof(t_socio),1,arch)) return 0;

    return 1;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return socio_leer(arch, nro_reg, s);
}

/******************************************************************************
Descripcion:    escribe el socio en la posicion nro_reg del archivo binario y
                hace flush.
Parametros:     arch: archivo abierto en modo de escritura binaria.
                nro_reg: numero de registro.
                s: socio a grabar.
Retorno:        OK si se pudo grabar, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_escribir_ALU(FILE *arch, unsigned nro_reg, const t_socio *s)
{
    if(!arch || !s) return 0;

    if(!fseek(arch,(long)(nro_reg*sizeof(t_socio)),SEEK_SET)) return 0;

    if(!fwrite(s,sizeof(t_socio),1,arch)) return 0;

    fflush(arch);
    return 1;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return socio_escribir(arch, nro_reg, s);
}

/******************************************************************************
Descripcion:    aplica la regla del Anexo A.4: el socio esta al dia si la
                ultima cuota es del mes de la fecha de proceso, o del mes
                anterior cuando el dia de la fecha de proceso es <= 10.
Parametros:     ultima_cuota: fecha de ultima cuota paga.
                fecha_proceso: fecha de proceso.
Retorno:        1 si esta al dia, 0 en caso contrario.
Observaciones:  -
******************************************************************************/
int cuota_al_dia_ALU(const t_fecha *ultima_cuota, const t_fecha *fecha_proceso)
{
    if (!ultima_cuota || !fecha_proceso) return 0;

    // Calcular diferencia de meses y años absoluta
    int aniosDif = fecha_proceso->anio - ultima_cuota->anio;
    int mesesDif = fecha_proceso->mes - ultima_cuota->mes;
    int diferenciaMeses = aniosDif * 12 + mesesDif;

    if (diferenciaMeses == 0) {
        return 1;
    }
    if (diferenciaMeses == 1 && fecha_proceso->dia <= 10) {
        return 1;
    }
    return 0;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return cuota_al_dia(ultima_cuota, fecha_proceso);
}

/******************************************************************************
Descripcion:    opcion Alta: carga un socio por teclado, lo valida con
                valida_socio, lo agrega al final de socios.dat con estado 'A'
                y lo inserta en el indice. Si es rechazado lo audita.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
                fecha_proceso: fecha de proceso.
Retorno:        OK si el socio fue agregado, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_alta_manual_ALU(t_indice *indice, const char *path, const t_fecha *fecha_proceso)
{

    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return socio_alta_manual(indice, path, fecha_proceso);
}

/******************************************************************************
Descripcion:    opcion Modificacion: pide el DNI, lo busca en el indice (un
                socio dado de baja no se puede modificar), muestra los datos
                actuales, pide los nuevos (Enter conserva), valida y
                sobrescribe el registro. El DNI y el estado no se modifican.
                Si es rechazado lo audita.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
                fecha_proceso: fecha de proceso.
Retorno:        OK si se modifico, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_modificacion_manual_ALU(t_indice *indice, const char *path, const t_fecha *fecha_proceso)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return socio_modificacion_manual(indice, path, fecha_proceso);
}

/******************************************************************************
Descripcion:    baja logica de socio: busca el DNI en el indice, pone estado
                'B' en el registro del archivo y lo elimina del indice.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
Retorno:        OK si se dio de baja, ERROR si no existe o ya estaba dado de
                baja.
Observaciones:  -
******************************************************************************/
int socio_baja_ALU(t_indice *indice, const char *path)
{
    if(!indice || !path) return 0;

    t_reg_indice reg;
    long dni;
    printf("Ingrese DNI del Socio a dar de baja: ");
    if(!scanf("%ld",&dni))
    {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    reg.id = dni;

    if(indice_buscar_ALU(indice,&reg,sizeof(t_reg_indice),compara_reg_indice_ALU)==0)
    {
        printf("Error: Socio con DNI: %ld no existe o ya esta dado de baja",dni);
        return 0;
    }
    FILE *arch = fopen(path, "r+b");
    if(!arch) return 0;

    t_socio socio;
    if(!socio_leer_ALU(arch,reg.nro_reg,&socio))
    {
        fclose(arch);
        return 0;
    }
    if(socio.estado == 'B')
    {
        fclose(arch);
        return 0;
    }else{
         socio.estado = 'B';
    }


    if (!socio_escribir_ALU(arch,reg.nro_reg, &socio))
    {
        fclose(arch);
        return 0;
    }
    fclose(arch);
    if(!indice_eliminar_ALU(indice,&reg,sizeof(t_reg_indice),compara_reg_indice_ALU)) return 0;
    return 1;
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    //return socio_baja(indice, path);
}

/******************************************************************************
Descripcion:    recorre el indice (ordenado por DNI y solo con activos) y
                lista los socios leyendo cada registro del archivo.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
Retorno:        OK si se pudo abrir el archivo, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socios_listar_ALU(const t_indice *indice, const char *path)
{
    if(!indice || !path) return 0;

    FILE *arch =fopen(path,"rb");
    if(!arch) return 0;

    printf("%-10s   %-15s   %-30s   %-10s   %-1s   %-10s   %-7s   %-10s   %-1s   %-10s   %-s\n",
       "DNI", "CUIL", "APELLIDOS Y NOMBRES", "NACIMIENTO", "S",
       "AFILIACION", "CATEG.", "ULT.CUOTA", "E", "PLAN", "EMAIL TUTOR");



    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return socios_listar(indice, path);
}

/******************************************************************************
Descripcion:    recorre el indice y exporta a un archivo binario los socios
                con plan PREMIUM o VIP y cuota al dia (el estado 'A' lo
                garantiza el indice). Sobrescribe el archivo de exportacion
                anterior.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
                path_export: ruta de beta_testers.dat.
                fecha_proceso: fecha de proceso.
Retorno:        OK si se pudo exportar, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socios_exportar_ALU(const t_indice *indice, const char *path, const char *path_export,
                    const t_fecha *fecha_proceso)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return socios_exportar(indice, path, path_export, fecha_proceso);
}

/******************************************************************************
Descripcion:    opcion 2 del menu: submenu para elegir entre alta y
                modificacion manual de socio.
Parametros:     ind_socios: indice de socios.
                fecha_proceso: fecha de proceso.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void abm_socio_ALU(t_indice *indice, const t_fecha *fecha_proceso)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    abm_socio(indice, fecha_proceso);
}
