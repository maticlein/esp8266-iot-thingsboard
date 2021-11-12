# esp8266-iot-thingsboard
Actividad de ejemplo de IoT utilizando ESP8266 para obtener los datos de un sensor de temperatura/humedad y de un módulo GPS para monitorearlos a través de la plataforma ThingsBoard.

## Instalación de ESP8266 en el IDE de Arduino
Para poder utilizar placas con el chip ESP8266 en el IDE de Arduino primero debe instalarla. Siga el tutorial descrito [aquí](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/).

## Instalación de librería ESP8266WiFi
Descargue el directorio publicado en [este link](https://github.com/esp8266/Arduino). Luego busque en la carpeta "libraries" la carpeta "ESP8266WiFi" e instálela como librería en Arduino. 

## Instalación librerías complementarias
En Arduino seleccionar Programa -> Incluir Librería -> Administrar Bibliotecas y buscar e instalar las siguientes librerías:
1. PubSubClient (por Nick O'Leary)
2. Adafruit Unified Sensor (por Adafruit)
3. DHT sensor library (porAdafruit)
4. Arduino ThingsBoard (por ThingsBoard)
5. ArduinoJSON (por bblanchon)
6. Arduino Http Client

[Link de referencia](https://thingsboard.io/docs/samples/esp8266/temperature/) 

## Conexión WiFi
Edite las líneas 5 y 6 del código compartido en este repositorio con el nombre de su red WiFi y la contraseña.

## ThingsBoard
Ingresar a [https://demo.thingsboard.io/](https://demo.thingsboard.io/) y crear una cuenta.
### Crear dispositivo
En el menú de la izquierda, seleccionar "Dispositivos" y luego la opción "+" que aparece en la esquina superior derecha. Luego seleccionar "Agregar nuevo dispositivo", darle un nombre y presionar "Agregar".\
Una vez creado, seleccione su dispositivo y vaya a la sección "Gestionar Credenciales", ahí obtendrá el Tóken de acceso, el cual deberá copiar y pegar en la línea 7 del código compartido en este repositorio.
### Crear panel
En el menú de la izquierda, seleccionar "Paneles" y luego la opción "+" que aparece en la esquina superior derecha. Luego seleccionar "Crear nuevo panel", darle un título y presionar "Agregar".
### Agregar Widgets al panel
Seleccionar el panel y presionar "Abrir panel". Luego, en la parte inferior derecha de la pantalla seleccionar el ícono del lápiz y agregar un nuevo widget. Seleccione el tipo de widget que desea agregar (sugerido: Digital gauges -> Mini gauge).\
En la configuración del widget encontrará la sección Set de datos. Presione agregar, escriba un Alias de entidad (sugerido: mismo nombre del dispositivo) y presionar "Crear nuevo alias!. En "Filtro por tipo" marque la opción "Única entidad", en "Tipo" marque "Dispositivo" y luego seleccione su dispositivo en la lista desplegable que aparecerá. Finalmente seleccione la variable del dispositivo que quiere mostrar en el widget.