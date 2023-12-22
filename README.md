# SmartRobot-FollowLine

## Objetivo 🎯
El objetivo de esta práctica es la programación de un kit de arduino proporcionado por la universidad. Este kit consta de un coche conectado a una placa de Arduino uno y enlazado a otra placa ESP32 con conexión wifi. El objetivo es el seguimiento de una linea mediante los sensores de infrarojos del coche. Además de esto también se pide mandar mensajes a un servidor mqtt con información de lo que va ocurriendo en el coche 

## Implementación

Para la primera implementación lo que intentamos hacer fue una pequeña máquina de estados donde el robot transitaba en función de las lecturas del sensor de infrarrojos. 

Una vez tuvimos eso hecho nos pusimos manos a la obra con todo lo que se refería a la comunicación. Comenzamos enviado mensajes simples y ma tarde comenzamosa a ir añadiendo los JSONs

Tambien desarollamos una función que nos permitia depurar el codigo atraves de los codigos de colores que transmitia un led 

Una vez teniamos una buena base de la práctica comenzamos a añadir implementaciones pero no de forma muy exitosa.

- Intentamos crear una version que estuviera dotada de freeRTOS para que el sistama fuera mas reactivo y pudieranos nosotros asignar los prioridades que quisiéramos a las diferentes tareas. Esto no salio del todo bien ya que en vez de sernos util nos perjudicó y hacia que el bucle principal no fuinonara de la manera adecuada

- También intentamos añadir un controlador PID para mejorar la corrección de errores y manejos de la velocidad del coche. Una vez lo tuvimos implementado, nos pusimos a ajustar los valores y nos dimos cuenta de que este o era muy inseguro e inestable y perida rápidamente la linea o era mas seguro pero también mas lento que la version anterior.

## Conclusión 

Tras muchas pruebas y errores. Nos centramos en hacer una versión quiza mas lenta pero mas segura, sin sistema freeRTOS ni sistema PID. Simplemente una velocidad constante pero muy bien calibrada para que el coche cumpliera el objetivo, **seguir la linea** ...

## Video demostrativo 
