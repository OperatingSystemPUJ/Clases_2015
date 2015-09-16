## Respuestas de exercises2.txt

### Santiago Quintero

1.
 El codigo lanza cuatro hilos al tiempo para recorrer una lista de ARRAYSIZE posiciones, cada hilo inicializa una porcion del array, calcula una suma interna y luego agrega el resultado de la suma interna a la variable global sum.
 Adicionar lo calculado en la suma interna a la variable global sum seria la zona critica del programa ya que la inicializacion de las subsecciones del array no se sobrelapan entre los hilos.

 Para asegurar que la zona critica sea protegida se utiliza un mutex, el cual es bloqueado en cada hilo antes de entrar a la zona critica y al terminar la adicion se desbloquea el mutex para que otro hilo lo utilice.

 El programa principal esperar a que todos los hilos terminar (join) y al hacerlo imprime en pantalla el resultado de la variable global sum. No hay errores en la eleccion de la seccion critica y se libera la informacion del mutex al final.

