# Nave espacial 3D

Este proyecto dibuja una nave sencilla en una ventana. La nave esta hecha con piezas basicas y gira mientras el programa esta abierto. Puedes cerrar la ventana con Escape.

## Archivos principales

- `main.cpp`: tiene las instrucciones del programa, la forma y los colores de la nave.
- `glad.c`: ayuda a conectar el programa con las funciones graficas disponibles en la computadora.
- `include/`: guarda los archivos que describen GLAD y GLFW para que el compilador sepa como usar esas herramientas.

## Librerias

- `glad/glad.h`: declara las funciones de OpenGL que usa el programa.
- `GLFW/glfw3.h`: permite abrir la ventana, leer el teclado y mantener el programa funcionando mientras la ventana esta abierta.
- `iostream`: sirve para mostrar mensajes en la terminal cuando ocurre un error.
- `vector`: permite guardar una lista de numeros que describen las piezas y sus colores.
- `cmath`: contiene operaciones matematicas usadas para el giro y la perspectiva.
- **OpenGL**: dibuja las piezas en la ventana.
- **GLAD**: busca las funciones de OpenGL que ofrece el sistema al iniciar el programa.
- **GLFW**: administra la ventana y las teclas.

## Como esta formada la nave

El programa guarda cada punto de la nave con seis numeros: tres indican donde esta el punto (X, Y y Z) y tres indican su color (rojo, verde y azul). Luego junta muchos puntos para dibujar triangulos.

Las piezas se construyen asi:

1. El cuerpo es una caja.
2. La punta y el fuego del motor son piramides.
3. Las alas y aletas son prismas triangulares.
4. Las ventanas son cajas pequenas.

Las caras usan distintos niveles de brillo para que se distingan mejor. Los colores se escriben con tres valores entre `0.0` y `1.0`: rojo, verde y azul. Por ejemplo, `Color verde {0.1f, 0.8f, 0.2f};` crea un verde.

Para cambiar el color de una pieza, busca los colores al inicio de `construirNave()` y cambia el valor que se usa en la pieza. Por ejemplo, el cuerpo usa `gris` y las alas usan `azul`.

## Funciones del codigo

- `brillo(Color c, float k)`: hace un color mas claro u oscuro. `k` es el nivel de brillo.
- `tri(Vec3 a, Vec3 b, Vec3 c, Color col)`: guarda los tres puntos de un triangulo y su color.
- `quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color col)`: divide una pieza de cuatro lados en dos triangulos.
- `caja(...)`: crea una caja usando seis lados y un color.
- `prisma(...)`: crea un prisma triangular extendiendo un triangulo en una direccion.
- `piramide(...)`: crea una piramide con base rectangular y una punta.
- `construirNave()`: define los colores y coloca cada pieza de la nave.
- `compilar(GLenum tipo, const char* fuente)`: prepara un shader para que OpenGL lo pueda usar. Si hay un error, muestra el mensaje en la terminal.
- `ajustarVista(...)`: ajusta el area de dibujo cuando cambia el tamano de la ventana.
- `main()`: inicia GLFW y GLAD, prepara la ventana y los datos, dibuja la nave repetidamente y al final libera lo que uso.

## Palabras clave y nombres utiles

- `#include`: agrega al archivo las herramientas declaradas en otra libreria.
- `struct`: crea un tipo propio para agrupar datos. `Vec3` guarda X, Y y Z; `Color` guarda rojo, verde y azul.
- `void`: indica que una funcion hace algo pero no devuelve un resultado.
- `float`: representa numeros con decimales.
- `return`: termina una funcion y, si corresponde, devuelve un resultado.
- `if`: revisa una condicion y ejecuta instrucciones si se cumple.
- `while`: repite instrucciones mientras una condicion siga siendo cierta.
- `for`: repite instrucciones una cantidad de veces.
- `const`: indica que un valor se mantiene igual.
- `unsigned int`: guarda un numero entero sin signo; aqui identifica recursos de OpenGL.
- `nullptr`: indica que no se esta pasando un objeto o dato.
- `GLFWwindow*`: referencia a la ventana que administra GLFW.

Algunos nombres que empiezan con `gl` son ordenes de OpenGL. Por ejemplo, `glClearColor` elige el color de fondo, `glClear` limpia la imagen anterior, `glDrawArrays` dibuja las piezas y `glViewport` define el area de dibujo.

## Shaders y dibujo

Los shaders son instrucciones pequenas que ejecuta la tarjeta grafica. El `vertexShaderSource` acomoda los puntos, aplica el giro y la perspectiva. El `fragmentShaderSource` asigna el color a cada parte dibujada.

En `main()`, el programa guarda los puntos en la memoria grafica. Despues, el ciclo `while` limpia la imagen anterior, actualiza el giro, dibuja la nave y revisa si se presiono Escape. Al cerrar la ventana, libera los recursos graficos.

## Compilar y ejecutar en Ubuntu

Instala las herramientas una vez:

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev
```

Desde la carpeta del proyecto, compila y abre el programa:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp glad.c -o nave -lglfw -lGL -ldl -pthread
./nave
```

En VS Code tambien puedes compilar con `Ctrl+Shift+B`.

## Compilar y ejecutar en Windows

Instala MSYS2 y abre la terminal llamada **MSYS2 UCRT64**. Instala las herramientas con:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-glfw
```

Agrega `C:\msys64\ucrt64\bin` al PATH de Windows. Abre PowerShell en la carpeta del proyecto y ejecuta:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp glad.c -o nave.exe -LC:/msys64/ucrt64/lib -lglfw3 -lopengl32 -lgdi32
.\nave.exe
```

Tambien puedes ejecutar `compilar_windows.bat` desde una terminal donde MSYS2 `g++` este disponible.
