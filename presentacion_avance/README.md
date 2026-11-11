# CB-CTT: avance con solución inicial aleatoria

Código en C++ para el problema de asignación de horarios basada en currículos (CB-CTT, formulación UD2 de ITC-2007). Esta entrega cubre la base del algoritmo asignado (Hill Climbing con Best Improvement):

- Lectura de las instancias `.ctt` y almacenamiento interno de los datos.
- Solución inicial aleatoria.
- Función de evaluación: cuenta las 4 restricciones duras y las 4 blandas.
- Escritura de la solución en el formato de salida pedido.

Hill Climbing y su movimiento todavía **no están implementados**: el programa construye una solución aleatoria, la evalúa y la guarda.

## Qué necesitas

- Linux, o Windows con WSL (Ubuntu).
- `g++` con soporte de C++17 y `make`.

Si falta alguno en Ubuntu/WSL:

```bash
sudo apt update && sudo apt install -y build-essential
```

## Contenido de la carpeta

```
presentacion_avance/
├── Makefile
├── src/            código fuente
├── instancias/     las 23 instancias .ctt
└── soluciones/     salidas generadas (una por instancia)
```

| Archivo | Responsabilidad |
|---|---|
| `parser.h/.cpp` | Lee el `.ctt`, guarda cursos, salas, currículos, indisponibilidades y la matriz de conflictos |
| `solucion.h/.cpp` | Representación de la solución (matriz sala × periodo) y generación aleatoria |
| `evaluacion.h/.cpp` | Cuenta restricciones duras y blandas y calcula la penalización |
| `salida.h/.cpp` | Escribe el archivo de resultados |
| `main.cpp` | Une los pasos anteriores |

## Cómo usarlo

Desde la carpeta `presentacion_avance`:

```bash
make                 # compila y deja el ejecutable ./cbctt
make test            # corre solo la primera instancia y muestra el inicio de su salida
make test-all        # corre las 23 instancias y muestra una tabla resumen
make clean           # borra lo compilado (no toca soluciones/)
```

Cada ejecución escribe su resultado en `soluciones/<instancia>.txt`.

### Elegir la semilla

La solución inicial depende de un número aleatorio. Por defecto se usa la semilla 1, así que dos ejecuciones iguales dan el mismo resultado. Para probar otra:

```bash
make test-all SEMILLA=7
```

### Ejecutar una instancia a mano

```bash
./cbctt instancias/comp05.ctt soluciones/comp05.txt 3
```

Los argumentos son: instancia, archivo de salida y semilla. El archivo de salida y la semilla son opcionales. Sin archivo de salida, el resultado se imprime por consola.

## Qué contiene cada archivo de salida

```
0.001                       tiempo de cómputo, en segundos
64 2444                     infactibilidad (suma de las duras)  y  penalización (blandas ponderadas)

# Conteo restricciones duras
5 50 0 9                    Lectures, Conflicts, RoomOccupancy, Availabilities

# Conteo restricciones blandas
2127 12 92 73               RoomCapacity, MinWorkingDays, CurrCompactness, RoomStability

# Lista de asignaciones (una por leccion)
c0070 B 0 0                 curso, sala, día, periodo del día
...
```

La penalización pondera las blandas con los pesos de UD2: 1·RoomCapacity + 5·MinWorkingDays + 2·CurrCompactness + 1·RoomStability.

## Qué esperar de `make test-all`

La solución es aleatoria y no se mejora, por lo que casi todas las instancias salen **infactibles** (infactibilidad mayor que 0). Solo `toytoy`, por ser muy pequeña, puede resultar factible. Mejorar estos valores es el trabajo de las siguientes entregas.

## Nota sobre reproducibilidad

Con la misma semilla se obtiene la misma solución en un mismo entorno. El generador de números aleatorios es estándar, pero el orden en que `std::shuffle` mezcla puede variar entre implementaciones de la biblioteca de C++. Se comprobó que dan idéntico resultado `g++` 11.4 (Ubuntu 22.04) y `g++` 13.2 (Windows, MinGW), ambos con libstdc++.
