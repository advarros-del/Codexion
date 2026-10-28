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

## Estado actual (última sesión: 26/10/2026)

- Se sigue atacando el **deadlock de dongles** con adquisición "todo o nada".
  `compiling()` guarda una sola vez el resultado de `take_dongles()` y repite
  el intento después de volver de `go_to_sleep()`.
- `go_to_sleep()` ya espera con el mutex del dongle, comprueba el fin de
  simulación después de despertar y selecciona FIFO o EDF según `scheduler`.
- El proyecto compila y ya no se queda colgado al finalizar, pero en la prueba
  siguen avanzando principalmente los Coders 0 y 2.
- Diagnóstico actual del scheduler: en `fifo_thing()` y `edf_thing()` se usa
  `aux = 2147483647`, demasiado pequeño para los valores absolutos de
  `gettime()` en milisegundos desde 1970. Por eso las peticiones no se
  seleccionan correctamente. `id_aux` también debe inicializarse para no
  usarse sin valor cuando no hay peticiones activas.
- FIFO y EDF ya están conectados en `requesting()`: `scheduler == 0` guarda
  `time_stamp` y `scheduler == 1` guarda `deadline`. `edf_thing()` recorre las
  peticiones del dongle y busca la menor `deadline`.
- **Ramas del orden de lockeo ACLARADAS y aceptadas**: la condición correcta
  es `id - 1 < 0` (solo el Coder 0). Aclarado por qué:
  - Coder 0: `l_don = donglers[N-1]` (el mayor), `r_don = donglers[0]` → se
    coge la derecha primero.
  - Coder `id >= 1`: `l_don = donglers[i-1] < r_don = donglers[i]` → se
    coge la izquierda primero. Regla general: siempre el de menor índice
    primero; la rama no es caprichosa, es consecuencia de qué punteros apunta
    cada coder.
  - Estuvieron cruzadas (cogía el mayor primero para el Coder 1) — corregido.
- **Decisión del usuario sobre `ndong`**: si al intentar el segundo dongle ve
  que está ocupado, libera el que tenga cogido e iguala `ndong = 0`.
- Pendiente dentro de la pareja: `go_to_sleep` sin terminar (usa `x_don` /
  `x_coder` que no son sus parámetros), y decidir si al despertar `go_to_sleep`
  devuelve SIN coger el dongle (reintentar pareja desde cero) o lo COGE y el
  llamador gestiona la caída (ver más abajo).

### Historial previo (18/09/2026)

- Todos los mutex se inicializan ya (se había olvidado): `coders[i].mutex`,
  `end_mutex` y `print_mutex`. Sin esto, glibc abortaba con
  "pthread_mutex_lock: assertion failed: mutex->__data.__owner == 0".
- **Bug de puntero en `on_fire` SOLUCIONADO**: `coder_routine` le pasaba
  `&coders[id]` (su propia estructura) en vez de la base del array.
  Ahora llama `on_fire(coder->shared->coders)`. Antes recorría posiciones
  fuera del array (para el Coder 2 leía posiciones 2,3,4,5,6) y lockeaba
  memoria basura. Se depuró desde cero con lldb:
  - Puedes dejar que el programa pete sin breakpoints; lldb se detiene solo.
  - Localizar tu frame: `bt`, buscar `on_fire`, `frame select N`.
  - Aprendido: interpretar la pila de abajo arriba y que los frames de libc
    (`___pthread_mutex_lock`, `abort`, `raise`, etc.) son la maquinaria del
    abort; lo interesante son las funciones propias.
- Con esos arreglos el programa ya NO crashea.

## Pendiente / siguiente

1. **`printer` en `on_fire` imprime siempre "Coder 0"**: el mensaje de
   "is burned" usa un `id` que no es `coders[i].id`. Candidato: imprimir
   `coders[i].id` (y no `coders->id`).
2. **Deadlock de dongles**: 5 coders cogen 1 dongle cada uno, ninguno puede
   compilar y alguien quema. Diseño acordado (en curso, 21/09):
   - Regla de orden de lockeo: SIEMPRE subiendo de índice (asegura que nunca
     se forme el círculo). Cada coder lockea el dongle de índice menor
     primero. Con `l_don = i-1`, `r_don = i`: la excepción es el Coder 0
     (su `l_don` es el de índice mayor). Condición: "si `i-1` no es
     negativo → izquierda primero"; si `i-1` es negativo (Coder 0) →
     derecha primero (índice 0). **Ramas ya aclaradas/acordadas.**
   - Estrategia "todo o nada": o se cogen LOS DOS dongles a la vez, o se
     sueltan ambos antes de hacer `cond_wait` y se reintenta en bucle al
     despertar (nunca esperar teniendo un mutex cogido). El usuario ya libera
     el dongle cogido y pone `ndong = 0` antes de esperar por el segundo.
   - Se baraja encapsular la lógica en una función propia de "coger la
     pareja" (la capa que ve `l_don` y `r_don` a la vez), por encima o
     sustituyendo a `take_dong`. Nombre de trabajo: `the_dongles` / `cosa`
     + `go_to_sleep`.
   - **Decisión pendiente** sobre `go_to_sleep`: ¿devuelve SIN coger el
     dongle (al despertar se reintenta la pareja entera desde cero, según el
     diseño acordado) o COGE el dongle al ganar la competición y el llamador
     cae al segundo `take_dong` llevando uno en la mano (esquema distinto,
     también viable, pero cambia el resto)?
   - Discutido pero SIN decidir: si al liberar conviene avisar a la cola
     `requests`/`fifo_thing` de quién sigue o basta con `cond_signal`
     (o `cond_broadcast` para que re-comprueben la prioridad los
     que esperan) .
   - Para terminar la pareja falta resolver: (a) pasar el dongle a
     `go_to_sleep` (hoy usa `x_don`/`x_coder` inexistentes), (b) el `else`
     pendiente de la rama de Coder 0, (c) quién señaliza la `cond` del
     dongle liberado.
3. **Scheduler FIFO/EDF**: corregir el valor inicial de comparación para
   `long long` y la inicialización de `id_aux`; después verificar que todos los
   coders compiten correctamente por sus dongles.
4. **El programa no se detiene al quemarse**: tras el primer "is burned"
   el mensaje se repite cada ciclo y la simulación no termina pronto.
   (Hipótesis en revisión en la próxima sesión.)
- Verificar el output tras esos arreglos (probado con
  `./codexion 5 800 200 200 200 5 50 fifo`).
- Revisar pasos del subject (está en `es.subject.pdf`) contra lo implementado.

## Notas de aprendizaje (curso)

- El usuario usa git manualmente como práctica.
- Con el tiempo quiere aprender sobre IA; trabajar con estos `.md` (contexto,
  progreso) es parte de ese aprendizaje.