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

## Estado actual (última sesión: 15/09/2026)

- **Bug del coredump SOLUCIONADO** (sesión anterior). `coders[i].shared` se
  inicializa en `fill_coder` con `g_info->coders[i].shared = g_info;` y el
  programa ejecuta sin crashear.
- Compila limpio con `cc -Wextra -Werror -Wall -g` (`make` OK).
- **`printer()` IMPLEMENTADA** (sesión de hoy):
  - `printer(int phrase, int coder, t_shared *g_info)` con `print_mutex`
    dentro de la propia función (lock + unlock).
  - Mensajes prefijados por código: 1 burned, 2 take a dongle, 3 leave a
    dongle, 4 compiling, 5 debugging, 6 refactoring.
  - `the_timeline()` corregida: instante = `gettime() - start_ime` y con su
    `pthread_mutex_unlock` correspondiente (faltaba y habría bloqueado).
  - Se resolvió el conflicto de tipos entre `.h` y `.c`: `printer` usa
    `t_shared *` (y se llama con `*coder->shared`).
- **Bug usleep SOLUCIONADO** (hoy): `usleep` espera microsegundos pero los
  tiempos vienen en ms. Había que multiplicar por 1000 (200 ms → 200000 us).
  Hasta entonces toda la simulación corría casi instantánea y el timeline solo
  llegaba a 1.

## Pendiente / siguiente

- **Por resolver (bug "0 Coder 0 is burned" al inicio)**:
  1. En `fill_struc.c` `fill_coder`: `last_compile = 0` inicializa a 0, pero
     `on_fire` compara `gettime() - last_compile >= burntime`; como `gettime()`
     devuelve ms desde 1970, la resta es gigantesca y quema al instante.
     Candidato: inicializar `last_compile` a `gettime()` (o `start_ime`).
  2. En `rutines.c` `on_fire`: `printer(1, coders->id, ...)` usa el primer
     coder, pero se itera con `coders[i]`. Imprime siempre "Coder 0".
     Candidato: usar `coders[i].id` (y `coders[i].shared`).
- Verificar el output tras esos dos arreglos (probado con
  `./codexion 5 800 200 200 200 5 50 fifo`).
- Revisar pasos del subject (está en `es.subject.pdf`) contra lo implementado.

## Notas de aprendizaje (curso)

- El usuario usa git manualmente como práctica.
- Con el tiempo quiere aprender sobre IA; trabajar con estos `.md` (contexto,
  progreso) es parte de ese aprendizaje.