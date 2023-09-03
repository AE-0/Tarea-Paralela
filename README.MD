## Tarea Computación Paralela y Distribuida

Solución de un laberinto utilizando algoritmo `flood fill` en C++20

## Como Usar

**Automaticamente** *con make*  `recomendado`

Ejecutar utilizando el laberinto de ejemplo y exportar solución a archivo `output.txt`

    $ make run

Solo compilar, recomendado para uso manual

    $ make

***

**Manualmente** *sin make*

Compilar sin make

    $ g++ std=c++20 -O2 main.cpp utils.cpp -o maze-solver

Opciones

```bash
$ ./maze-solver -i <input-file> -o <output-file>
Usage: maze-solver [OPTION...] FILE
Pass a file as an argument using -i and -o flag

  -i, --input=FILE           Input file
  -o, --output=FILE          Output file
  -?, --help                 Give this help list
  -h, --help                 Display this message
      --usage                Give a short usage message

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

Ejemplo

    $ ./maze-solver -i example.txt -o output.txt
