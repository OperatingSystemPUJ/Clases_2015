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


4.
 El codigo a probar crea varios hilos que deben realizar una serie de operaciones y finalmente retornan un resultado en pantalla, el problema surge del hecho que el proceso padre solo crea los hilos y termina sin antes esperar por el resultado de los hijos, por lo que el programa termina y nunca se sabe el resultado de los hilos. Para solucionarlo es necesario, despues de crear los hilos, esperar a que los hilos terminen agreagando una sentencia join. (e4.c)
