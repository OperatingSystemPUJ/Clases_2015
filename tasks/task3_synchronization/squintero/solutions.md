## Respuestas de exercise1.txt

### Santiago Quintero

1.
 El primer programa efectivamente imprime 42.
 El segundo programa no imprime 42 ya que lo devuelve como codigo de status mas no como un parametro de retorno.
 El tercer programa contiene una condicion de carrera por lo que el valor a imprimir puede ser 42 u otro valor como 0, dependiendo del hilo que modifique el dato de ultimo.
 El cuarto programa al probarlo en esta maquina especifica retornaba 42, sin embargo segun la especificacion de pthread_join() al llamarse con un hilo que ha sido desenganchado (en este caso con pthread_detach()) los resultados son indefinidos.
 El quinto programa tambien presenta una condicion de carrera donde solo imprime 42 si el hilo se ejecuta hasta el printf antes que el segundo hilo modifique el valor a 31.

2.
 Implementado como matrix.c

## Respuestas de exercises2.txt

### Santiago Quintero

1.
 El codigo lanza cuatro hilos al tiempo para recorrer una lista de ARRAYSIZE posiciones, cada hilo inicializa una porcion del array, calcula una suma interna y luego agrega el resultado de la suma interna a la variable global sum.
 Adicionar lo calculado en la suma interna a la variable global sum seria la zona critica del programa ya que la inicializacion de las subsecciones del array no se sobrelapan entre los hilos.

 Para asegurar que la zona critica sea protegida se utiliza un mutex, el cual es bloqueado en cada hilo antes de entrar a la zona critica y al terminar la adicion se desbloquea el mutex para que otro hilo lo utilice.

 El programa principal esperar a que todos los hilos terminar (join) y al hacerlo imprime en pantalla el resultado de la variable global sum. No hay errores en la eleccion de la seccion critica y se libera la informacion del mutex al final.

2.
 Implementado como threads_buffers.c

3.
 El programa principal lanza varios hilos para operar sobre secciones de dos arrays de datos, las secciones de cada hilo no se sobrelapan, sin embargo, cada hilo altera los datos de la variable global sum, al ejecutarse en concurrencia se presentan una condicion de carrera y la zona critica (las operaciones en sum) no esta siendo protegida.

 Para arreglar este problema solo necesitamos agregar un mutex (por ejemplo) en la zona critica y prevenir asi la condicion de carrera. (e3.c) Despues de intentos con el bloqueo del mutex dentro del ciclo y fuera, decidi dejarlo por fuera del ciclo ya que en este caso en especifico el mutex en el ciclo generaba suficiente overeheat para tardar mas que con el bloqueo por fuera del mismo (usando time).

4.
 El codigo a probar crea varios hilos que deben realizar una serie de operaciones y finalmente retornan un resultado en pantalla, el problema surge del hecho que el proceso padre solo crea los hilos y termina sin antes esperar por el resultado de los hijos, por lo que el programa termina y nunca se sabe el resultado de los hilos. Para solucionarlo es necesario, despues de crear los hilos, esperar a que los hilos terminen agreagando una sentencia join. (e4.c)
