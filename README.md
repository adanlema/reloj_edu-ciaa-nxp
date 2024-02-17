# Implementacion de un Reloj Despertador usando FreeRTOS. 
### Lema, Adan Juan Angel - **adanlema@hotmail.com**

Se deberá crear un repositorio git con acceso público que contenga el código fuente de un proyecto desarrollado en C para gestionar el funcionamiento de un reloj despertador, empleando la placa EDU-CIAA-NXP y su poncho, y empleando el sistema operativo de tiempo real FreeRTOS.

El reloj deberá cumplir los siguientes requisitos:
* Debe mostrar la hora y minutos y se debe poder ajustar en hora.
* El reloj funcionará únicamente en modo de 24 horas. El equipo normalmente muestra la hora y minutos actuales.
* Cuando el reloj está funcionado debe parpadear el punto del segundo dígito una vez por segundo.
* Se debe poder definir una alarma para una hora y minutos definidos.
* Se debe poder habilitar e inhabilitar la alarma. Cuando la alarma esté activa el punto del último dígito estará encendido.
* Cuando suena la alarma se puede posponer por cinco minutos.
* Cuando el equipo arranca la hora parpadea junto con el punto para informar que no está configurado.
* Cuando se presiona la tecla F1 por más de 3 segundos se ingresa al modo ajuste de hora, y entonces comienzan a parpadear los dígitos de los minutos.
* Con la tecla F4 se aumenta el valor de los minutos y con la tecla F3 se disminuye el mismo.
* El incremento y decremento vuelven a comenzar cuando alcanzan el valor máximo o mínimo.
* Si se presiona la tecla Aceptar quedan fijos los minutos y comienza a parpadear los sitios de la hora.
* Si no se presiona una tecla por más de 30 segundos, o si se presiona la tecla Cancelar, descartan todos los cambios.
* Cuando se presiona la tecla F2 por más de 3 segundos se ingresa al modo ajuste de la alarma.
* Al ingresar a este modo se muestra en los dígitos la hora a la que se encuentra ajustada actualmente la alarma, para distinguir este modo se prenden todos los puntos de los dígitos.
* El ajuste de la hora de la alarma sigue el mismo comportamiento que el ajuste de la hora del reloj.
* Cuando el reloj está en modo normal la tecla Aceptar activa la alarma y la tecla Cancelar la
desactiva.
* El punto del primer dígito indica si la alarma esta activada o no.
* Cuando la alarma esta sonando la tecla Aceptar la silencia por 5 minutos y la tecla Cancelar la silencia hasta el día siguiente a la hora programada.

**IMPORTANTE**

* **El valor de la constante *TICKS_POR_SEG* debe ser igual a la de *INT_POR_SEG* (mil interrupciones por segundo) para que el reloj aumenta la hora como corresponde, utilizamos en este caso que la primera constante tenga un valor de 10 solo para poder verificar con mas rapidez el funcionamiento de nuestro reloj.**
* **El valor de la constante *COUNT_MAX_TIME* debe ser igual a (30 * *INT_POR_SEG*) para que espero los 30 segundos indicados en el enunciado en caso de que no se presione ninguna tecla, utilizamos en este caso un valor de *10* solo para  poder verificar con mas rapidez el funcionamiento de nuestro reloj.**

Para el funcionamiento de nuestro programa se debe emplear los siguientes comandos en la terminal:

| Comando | Descripcion |
| --- | --- |
| make | Compila el codigo |
| make doc | Documento el codigo usando doxygen |
| make download | Sube el codigo a la placa |
| make clean | Borra la carpeta *build* |