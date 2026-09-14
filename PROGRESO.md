# PROGRESO.md — Codexion

Bitácora del proyecto. Se actualiza solo cuando el usuario lo pide, normalmente
al final de cada sesión.

## Qué es el proyecto

Codexion es una simulación tipo "filósofos" hecha en C con `pthreads`:
un grupo de "coders" comparte "dongles" (recursos) y debe cumplir un número
de compilaciones sin "quemarse". Soporta dos planificadores: `fifo` y `edf`
(por fechas límite).

### Estructura

- `coders/main.c` — entrada: valida argumentos (es esperado `argc == 9`) y
  llama a `codexion()`.
- `coders/codexion.c` — luz verde: carga datos, monta estructuras, crea
  el hilo monitor y los hilos "coder".
- `coders/fill_struc.c` — inicialización de datos, dongles y mutex/cond.
- `coders/rutines.c` — rutinas de los coders (compilar/debuggear/refactorizar),
  monitor y helpers (tiempo, fin de simulación, "burned").
- `coders/scheduler.c` — lógica de planificación (FIFO vs EDF).
- `coders/ft_atol.c` — conversión ASCII a long (estilo libft).

### Uso

`./codexion <coders> <burntime> <compile_time> <debug_time> <refac_time> <n_compile> <dongle_cd> <fifo|edf>`

## Estado actual (última sesión: 14/09/2026)

- **Bug del coredump SOLUCIONADO.** El problema era que `coders[i].shared`
  nunca se inicializaba: `the_coders` viene de `malloc` (basura) y cada
  `t_coder.shared` (un `struct share *`) apuntaba a memoria inválida. En
  `fill_coder` se ha añadido `g_info->coders[i].shared = g_info;` dentro del
  bucle. El programa ya ejecuta sin crashear (probado con `lldb`).
- Compila limpio con `cc -Wextra -Werror -Wall -g` (`make` OK).
- En curso: función `printer()` para que cada `printf` lleve la timeline
  (instante relativo = `gettime() - g_info->start_ime`) y use mensajes
  prefijados por código numérico.

## Pendiente / siguiente

- **Característica timeline** (diseño acordado):
  - Función `printer(phrase, coder, g_info)` que reciba un código numérico de
    evento, el id del coder y el `t_shared` (para la resta del tiempo).
  - El `printf` debe tener `print_mutex` dentro de la propia función.
  - Frases del subject (línea 271-275 del PDF): _X has taken a dongle_, _X is
    compiling_, _X is debugging_, _X is refactoring_, _X burned out_.
  - **Restricciones de la norma:** máximo 5 variables por función (los
    parámetros no cuentan) y NO se puede declarar + inicializar en la misma
    línea.
  - **`strcpy` NO está autorizado** (lista de la línea 342) y no hay libft
    enlazada. Pendiente de resolver: mecanismo que devuelva el `char *` de la
    frase sin copiar (candidata: selección por `switch`/condiciones que
    devuelvan el literal directamente).
- Revisar pasos del subject (está en `es.subject.pdf`) contra lo implementado.

## Notas de aprendizaje (curso)

- El usuario usa git manualmente como práctica.
- Con el tiempo quiere aprender sobre IA; trabajar con estos `.md` (contexto,
  progreso) es parte de ese aprendizaje.