# Motor de Scoring Basado en Reglas en C

Este proyecto implementa un motor de evaluación basado en reglas desarrollado en lenguaje C.
Las reglas y categorías se cargan desde archivos CSV y se procesan utilizando estructuras de
datos personalizadas, como listas enlazadas y mapas hash.

El sistema evalúa respuestas del usuario, calcula puntajes por categoría y entrega una
recomendación final de forma determinística, además de exportar los resultados a un archivo
de salida.

---

## Características

- Parsing de archivos CSV sin uso de librerías externas
- Implementación propia de mapas hash y listas enlazadas
- Scoring y clasificación determinística
- Manejo manual de memoria
- Arquitectura orientada a línea de comandos (CLI)
- Separación entre datos, lógica y presentación

---

## Casos de uso

- Sistemas de clasificación automática
- Motores de scoring
- Lógica de decisión basada en reglas
- Prototipos de sistemas expertos simples

---

## Estructura del proyecto

main.c
Map.c
Map.h
list.c
list.h
preguntas_test_vocacional.csv
carreras.csv


---

## Formato de los archivos CSV

### preguntas_test_vocacional.csv

Cada línea representa una pregunta y el área asociada:


Ejemplo:

Te interesa aprender a programar?,ingenieria
Te gustaria ayudar a las personas?,salud



Áreas válidas:
- ingenieria
- salud
- artes
- cienciaSociales

---

### carreras.csv

Cada línea representa una carrera y el área a la que pertenece:


Áreas válidas:
- ingenieria
- salud
- artes
- cienciaSociales

---

### carreras.csv

Cada línea representa una carrera y el área a la que pertenece:

Carrera,area

Ejemplo:

Ingenieria en Informatica,ingenieria
Medicina,salud
Arquitectura,artes
Psicologia,cienciaSociales


---

## Compilación

Desde la raíz del proyecto:

bash
gcc main.c Map.c list.c -o motor_scoring


Ejecución

./motor_scoring


El programa realizará las preguntas por consola y solicitará respuestas del tipo si o no.


Output

Puntaje obtenido por cada área

Área recomendada según el puntaje máximo

Listado de carreras asociadas a dicha área

Exportación del resultado a un archivo resultado.txt

Ejemplo de salida:


Resultados:
Ingeniería: 8
Salud: 3
Artes: 2
Ciencias Sociales: 4

Área recomendada: ingenieria


Archivo de salida

El programa genera un archivo resultado.txt con el resumen del test, incluyendo:

Puntajes por área

Área recomendada

Carreras sugeridas


Notas

Este proyecto forma parte de un portafolio técnico y tiene como objetivo demostrar:

Dominio del lenguaje C

Uso de estructuras de datos

Parsing de archivos sin librerías externas

Diseño de motores de decisión basados en reglas

Manejo explícito de memoria

No depende de frameworks externos y está pensado para ejecutarse en entornos estándar
compatibles con GCC.