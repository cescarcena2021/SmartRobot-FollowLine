# SmartRobot-FollowLine

## Objetivo 🎯
El objetivo de esta práctica es la programación de un kit de arduino proporcionado por la universidad. Este kit consta de un coche conectado a una placa de Arduino uno y enlazado a otra placa ESP32 con conexión wifi. El objetivo es el seguimiento de una linea mediante los sensores de infrarojos del coche. Además de esto también se pide mandar mensajes a un servidor mqtt con información de lo que va ocurriendo en el coche 

![image](https://github.com/cescarcena2021/SmartRobot-FollowLine/assets/102520602/4a3fd3b4-965b-4c7a-ac50-899e252d3155)


## Implementación

Para la primera implementación lo que intentamos hacer fue una pequeña máquina de estados donde el robot transitaba en función de las lecturas del sensor de infrarrojos. 

Una vez tuvimos eso hecho nos pusimos manos a la obra con todo lo que se refería a la comunicación. Comenzamos enviado mensajes simples y ma tarde comenzamosa a ir añadiendo los JSONs

Tambien desarollamos una función que nos permitia depurar el codigo atraves de los codigos de colores que transmitia un led 

Una vez teniamos una buena base de la práctica comenzamos a añadir implementaciones pero no de forma muy exitosa.

- Intentamos crear una version que estuviera dotada de freeRTOS para que el sistama fuera mas reactivo y pudieranos nosotros asignar los prioridades que quisiéramos a las diferentes tareas. Esto no salio del todo bien ya que en vez de sernos util nos perjudicó y hacia que el bucle principal no fuinonara de la manera adecuada

- También intentamos añadir un controlador PID para mejorar la corrección de errores y manejos de la velocidad del coche. Una vez lo tuvimos implementado, nos pusimos a ajustar los valores y nos dimos cuenta de que este o era muy inseguro e inestable y perida rápidamente la linea o era mas seguro pero también mas lento que la version anterior.

- Para calcular la distancia a la que se tenía que parar el coche, usamos el ultrasonido que tiene el coche ubicado en al parte frontal. Este lanza una onda y en funcion de cuanto tarde en regresar calcula la distancia. Esto lo tenemos que realizar lo más rápido posible ya que el coche cambia constantemente de posición y una lectura tardia podria significar una colision. Aunque la distancia a la que se deveria detener el coche es a 8 cm este realmente lo hace a una distancia mas cercana de entre uno o dos centímetros de erro ya que este ya tenia una velocidad y una inercia.

- Para la comunicación entre el Arduino y la placa ESP hemos usado el puero serie que ambos proporcionan. En este kit en concreto es un poco engorroso ya que hay que estar constantemente cambiando un switch que tiene la placa para subir el código a la palca o comunicarse entre ellas.

## 

## Dependencias

Para que el codigo funcione correctamente es necesario instalar estas dos librerias: 
### MQTT
Esta libreria nos permite mantener una comunicacion con el servidor e intercambiar mensajes.
Para lenzar el servidor usamos *mosquitto_sub* con el id de el grupo correspondiente que en nustro caso es el 9
```
mosquitto_sub -v -h 193.147.53.2 -p 21883 -t /SETR/2023/$ID_EQUIPO/

```
![image](https://github.com/cescarcena2021/SmartRobot-FollowLine/assets/102520602/96f8d6bf-3dd7-4b0e-83cc-2ec1fa121023)

### FastLED
Con esta libreria podemos controlar los colores del led RGD situado en la placa de Arduino.
![image](https://github.com/cescarcena2021/SmartRobot-FollowLine/assets/102520602/4acad45c-5f5c-4893-836b-93a053eab074)



## Conclusión 

Tras muchas pruebas y errores. Nos centramos en hacer una versión quiza mas lenta pero mas segura, sin sistema freeRTOS ni sistema PID. Simplemente una velocidad constante pero muy bien calibrada para que el coche cumpliera el objetivo, **seguir la linea** ...

## Video demostrativo 

https://github.com/cescarcena2021/SmartRobot-FollowLine/assets/102520602/6cdc0828-822a-4bf0-b96e-db5e6bc6e3ae

