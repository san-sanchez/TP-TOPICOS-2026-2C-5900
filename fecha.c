#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "fecha.h"
#include "util.h"
#include "indice.h"
      /* OK / ERROR */

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    solicita la fecha de proceso por teclado hasta obtener una
                valida. Si el usuario solo presiona Enter toma la fecha actual
                del sistema.
Parametros:     fecha_proceso: fecha destino.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/

void pedir_fecha_proceso_ALU(t_fecha *fecha_proceso)
{
    char cad[TAM_FECHA_TXT];
    int flag = 0;
    time_t tActual;
    struct tm *fecha;
    do{


        printf("***--------      <<< NOTA >>>      --------***\n");
        printf("***EL FORMATO DE FECHA DEBE SER DD/MM/AAAA ***\n");
        printf("*** PARA FECHA ACTUAL SOLO PRECIONE ENTER  ***\n\n");
        printf("***--------INGRESE FECHA DE PROCESO--------***\n");
        fgets(cad,TAM_FECHA_TXT,stdin);
        if(*cad == '\n' || *cad == '\0')
        {
            tActual = time(NULL);
            fecha = localtime(&tActual);
            fecha_proceso->dia = fecha->tm_mday;
            fecha_proceso->mes = fecha->tm_mon+1;
            fecha_proceso->anio= fecha->tm_year +1900;
            printf("INGRESO DE FECHA: %02d/%02d/%04d\n",fecha_proceso->dia,fecha_proceso->mes,fecha_proceso->anio);
            return;
        }
        if(sscanf(cad,"%d/%d/%d\n",&fecha_proceso->dia,&fecha_proceso->mes,&fecha_proceso->anio)==3)
        {
            if(fechaCorrecta(fecha_proceso)) flag=1;
        }else{
            system("cls");
            printf("***--------INGRESO FECHA DE ERRONEO--------***\n");
            Sleep(1500);
            system("cls");
        }
    }while(!flag);
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
       printf("INGRESO DE FECHA: %02d/%02d/%04d\n",fecha_proceso->dia,fecha_proceso->mes,fecha_proceso->anio);
}
int fechaCorrecta(const t_fecha *fecha)
{
    if(fecha->anio < 1)
        return 0;

    if(fecha->mes < 1 || fecha->mes > 12)
        return 0;

    if(fecha->dia < 1)
        return 0;

    if(fecha->dia > cantDiasMes(fecha->mes, fecha->anio))
        return 0;

    return 1;
}
int cantDiasMes(int mes, int anio)
{
    switch(mes)
    {
        case 1:  return 31;
        case 2:  return esBisiesto(anio) ? 29 : 28;
        case 3:  return 31;
        case 4:  return 30;
        case 5:  return 31;
        case 6:  return 30;
        case 7:  return 31;
        case 8:  return 31;
        case 9:  return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;
    }
}
int esBisiesto(int anio)
{
    return anio % 400 == 0 ||
          (anio % 4 == 0 && anio % 100 != 0);
}
