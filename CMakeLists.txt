#
# DANTE :: System Automatycznych Testów Jednostkowych oraz Akceptacyjnych
# Tomasz Jaworski, 2018-2020
#
# Plik wygenerowany automatycznie
# Znacznik czasowy: 2023-11-30 19:45:40.756408
#

cmake_minimum_required(VERSION 3.17)
project(project1 C)

# Przyjmij standard C11
set(CMAKE_C_STANDARD 11)

# Ustaw opcje kompilatora (z raportu Dante)
add_compile_options(
      "-Wall"
      "-pedantic"
      "-ggdb3"
      "-Wextra"
      "-Wno-error=implicit-fallthrough"
      "-xc"
      "-Werror"
      "-Wno-error=parentheses"
      "-Wno-error=unused-parameter"
      "-Wno-parentheses"
      "-D_GNU_SOURCE"
      "-fdiagnostics-color"
      "-std=c11"
      "-Werror=vla"
      "-DINSIDE_DANTE"
      "-D_ANSI_OUTPUT"
      "-fmax-errors=5"

)

# Ustaw opcje konsolidatora (z raportu Dante)
add_link_options(
        "-Wl,-wrap,main"
        "-ggdb3"
        "-Wl,-Map=main.map"
        "-Wl,-cref"
)

# Utwórz target 'project1' i dodaj do niego pliki źródlowe:
add_executable(project1
        "main.c"
        "heap.c"
        "unit_helper_v2.c"
        "unit_test_v2.c"
        "rdebug.c"
        "memmanager.c"
)

# Dołącz niezbędne biblioteki
target_link_libraries(project1
        "pthread"
        "m"
)