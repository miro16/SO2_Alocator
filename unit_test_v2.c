/*
 * Unit Test Bootstrap
 * Autor: Tomasz Jaworski, 2018-2020
 *
 * Test dla zadania Projekt: Ocena 3,0
 * Autor testowanej odpowiedzi: Maciek Mirowski
 * Test wygenerowano automatycznie o 2023-11-30 19:45:33.564300
 *
 * Debug: 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include <assert.h>

#if !defined(__clang__) && !defined(__GNUC__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na swoim kompilatorze C.
#error System testow jednostkowych jest przeznaczony dla kompilatorów GCC/Clang.
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na platformie Windows.
#error System testow jednostkowych NIE jest przeznaczony dla testów uruchamianych na platformach Windows.
#endif

#define _RLDEBUG_API_
#include "unit_helper_v2.h"
#include "rdebug.h"

#include "tested_declarations.h"
#include "rdebug.h"

//
// Elementy globalne dla całego testu
//




//
//  Test 1: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST1(void)
{
    // informacje o teście
    test_start(1, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
                
                void *ptr = heap_malloc(0);
                
                test_error(ptr == NULL, "Funkcja heap_malloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 2: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST2(void)
{
    // informacje o teście
    test_start(2, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(67108834);

                test_error(ptr == NULL, "Funkcja heap_malloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 3: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST3(void)
{
    // informacje o teście
    test_start(3, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(463);

                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 4: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST4(void)
{
    // informacje o teście
    test_start(4, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(52);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(167);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 5: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST5(void)
{
    // informacje o teście
    test_start(5, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(34);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(12998841);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 6: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST6(void)
{
    // informacje o teście
    test_start(6, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(52);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(67354884);
                test_error(ptr2 == NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 7: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST7(void)
{
    // informacje o teście
    test_start(7, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(39);
                void *ptr2 = heap_malloc(158);
                void *ptr3 = heap_malloc(264);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(152);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 8: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST8(void)
{
    // informacje o teście
    test_start(8, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(63);
                void *ptr2 = heap_malloc(157);
                void *ptr3 = heap_malloc(286);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(157);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 9: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST9(void)
{
    // informacje o teście
    test_start(9, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {762, 838, 715, 195, 304, 164, 627, 679, 102, 272, 151, 114, 761, 424, 465, 551, 931, 784, 483, 149, 967, 776, 357, 441, 318, 756, 386, 941, 770};
                void *ptrs[29];

                for (int i = 0; i < 29; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[16]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[16]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[16]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(893);
                test_error(ptrs[16] == ptr, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptrs[16] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 10: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
//
void UTEST10(void)
{
    // informacje o teście
    test_start(10, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {261, 718, 199, 626, 285, 408, 389, 468, 172, 523, 778, 193, 743, 890, 154, 464, 563, 246, 986, 932, 288, 347, 271, 759, 564, 871};
                void *ptrs[26];

                for (int i = 0; i < 26; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[19]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[19]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[19]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(932);
                test_error(ptrs[19] == ptr, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptrs[19] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 11: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST11(void)
{
    // informacje o teście
    test_start(11, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(78);
                void *ptr2 = heap_malloc(128);
                void *ptr3 = heap_malloc(250);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(137);
                
                test_error(ptr2 != ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 12: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST12(void)
{
    // informacje o teście
    test_start(12, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(94);
                void *ptr2 = heap_malloc(6);
                void *ptr3 = heap_malloc(234);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(15);

                test_error(ptr2 != ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 13: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST13(void)
{
    // informacje o teście
    test_start(13, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(0, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 14: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST14(void)
{
    // informacje o teście
    test_start(14, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(75, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 15: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST15(void)
{
    // informacje o teście
    test_start(15, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(0, 1);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 16: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST16(void)
{
    // informacje o teście
    test_start(16, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(67108843, 1);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 17: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST17(void)
{
    // informacje o teście
    test_start(17, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(16777214, 4);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 18: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST18(void)
{
    // informacje o teście
    test_start(18, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(939, 1);

                test_error(ptr != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 0; i < 939; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 19: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST19(void)
{
    // informacje o teście
    test_start(19, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(203, 32);

                test_error(ptr != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 0; i < 6496; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 20: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST20(void)
{
    // informacje o teście
    test_start(20, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(98, 79);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 7742; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(199, 79);
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 15721; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 21: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST21(void)
{
    // informacje o teście
    test_start(21, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(56, 1);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 56; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(14747968, 1);
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 14747968; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 22: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST22(void)
{
    // informacje o teście
    test_start(22, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(15, 3);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 45; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(67718819, 3);
                test_error(ptr2 == NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 23: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST23(void)
{
    // informacje o teście
    test_start(23, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(81, 2);
                char *ptr2 = heap_calloc(147, 2);
                char *ptr3 = heap_calloc(214, 2);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                for (int i = 0; i < 162; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 294; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 428; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(130, 2);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 260; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 24: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST24(void)
{
    // informacje o teście
    test_start(24, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(90, 4);
                char *ptr2 = heap_calloc(116, 4);
                char *ptr3 = heap_calloc(290, 4);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                for (int i = 0; i < 360; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 464; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 1160; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(116, 4);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 464; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 25: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST25(void)
{
    // informacje o teście
    test_start(25, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {771, 961, 502, 142, 939, 906, 978, 489, 317, 369, 228, 762, 241, 563, 443, 792, 662, 677, 539, 271, 365, 410, 595, 708, 139, 303, 653, 627};
                size_t mem_units[] = {4, 5, 2, 3, 1, 3, 4, 5, 4, 4, 3, 4, 2, 2, 1, 2, 1, 5, 4, 4, 4, 3, 3, 2, 4, 3, 2, 5};
                char *ptrs[28];

                for (int i = 0; i < 28; ++i)
                {
                    ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                        test_error(ptrs[i][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }

                heap_free(ptrs[9]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[9]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[9]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(320, 4);
                test_error(ptrs[9] == ptr, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptrs[9] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (size_t j = 0; j < 320 * 4; ++j)
                    test_error(ptrs[9][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 26: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST26(void)
{
    // informacje o teście
    test_start(26, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {498, 765, 961, 817, 671, 502, 247, 544, 239, 612, 816, 685, 126, 661, 415, 703, 260, 490, 399, 643};
                 size_t mem_units[] = {3, 3, 5, 1, 3, 3, 3, 5, 2, 1, 3, 2, 1, 2, 1, 3, 2, 5, 5, 5};
                 char *ptrs[20];

                 for (int i = 0; i < 20; ++i)
                 {
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                     test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                     for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                         test_error(ptrs[i][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 heap_free(ptrs[7]);

                 test_error(pointer_unallocated == get_pointer_type(ptrs[7]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[7]));

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 void *ptr = heap_calloc(544, 5);
                 test_error(ptrs[7] == ptr, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptrs[7] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                 for (size_t j = 0; j < 544 * 5; ++j)
                     test_error(ptrs[7][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 27: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST27(void)
{
    // informacje o teście
    test_start(27, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(73, 1);
                char *ptr2 = heap_calloc(199, 1);
                char *ptr3 = heap_calloc(259, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                for (int i = 0; i < 73; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 199; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 259; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(208, 1);

                test_error(ptr2 != ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                for (int i = 0; i < 208; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 28: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST28(void)
{
    // informacje o teście
    test_start(28, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 29: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST29(void)
{
    // informacje o teście
    test_start(29, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 30: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST30(void)
{
    // informacje o teście
    test_start(30, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {945, 191, 397, 662, 785, 411, 620, 206, 113, 680, 420, 777, 819, 896, 853, 762, 419, 588, 584, 551, 719, 385, 933, 558, 145, 576, 460, 601, 407, 380, 196, 329};
                void *ptrs[32];

                for (int i = 0; i < 32; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           


                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 945, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 945, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 31: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST31(void)
{
    // informacje o teście
    test_start(31, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {588, 722, 351, 437, 279, 974, 291, 699, 724, 990, 285, 463, 118, 462, 702, 358, 124, 996, 446, 348, 109, 121, 221, 513, 362, 205, 182, 627, 957, 884, 279, 533, 988, 240, 752, 307};
                 size_t mem_units[] = {5, 3, 5, 2, 5, 4, 2, 2, 1, 2, 3, 1, 1, 2, 2, 5, 1, 4, 1, 5, 5, 1, 5, 4, 3, 1, 5, 2, 1, 5, 2, 5, 4, 2, 4, 1};
                 char *ptrs[36];

                 for (int i = 0; i < 36; ++i)
                 {
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 size_t block_size = heap_get_largest_used_block_size();
                 test_error(block_size == 4420, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 4420, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 32: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST32(void)
{
    // informacje o teście
    test_start(32, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {722, 717, 422, 116, 417, 376, 186, 426, 205, 371, 859, 141, 696, 155, 365, 157, 312, 914, 706, 356, 206, 907, 649, 628, 184, 875, 790, 141, 305, 980, 765, 358};
                char *ptrs[32];

                for (int i = 0; i < 32; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                ptrs[29][-1] += 5 ;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 33: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty
//
void UTEST33(void)
{
    // informacje o teście
    test_start(33, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                char *ptr = (char *)(intptr_t)rand();
                
                heap_free(ptr);
                
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
            
                heap_free(NULL);
                
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 34: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST34(void)
{
    // informacje o teście
    test_start(34, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(155);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr + 1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 155, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 155, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr - 1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 155, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 155, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr + 96);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 155, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 155, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr - 22);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 155, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 155, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 35: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST35(void)
{
    // informacje o teście
    test_start(35, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(353);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(85);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 353, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 353, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 85, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 85, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 36: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST36(void)
{
    // informacje o teście
    test_start(36, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(359);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 359, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 359, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 37: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST37(void)
{
    // informacje o teście
    test_start(37, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(262);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(85);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 262, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 262, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 85, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 85, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 38: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
//
void UTEST38(void)
{
    // informacje o teście
    test_start(38, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(392);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(1000);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(76);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 1000, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1000, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 392, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 392, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 76, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 76, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 39: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST39(void)
{
    // informacje o teście
    test_start(39, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(344);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(955);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(59);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 955, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 955, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 344, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 344, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 59, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 59, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr4 = heap_malloc(1308);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error(ptr1 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 1308, "Funkcja heap_validate() powinna zwrócić wartość 1308, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 40: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
//
void UTEST40(void)
{
    // informacje o teście
    test_start(40, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(389);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(6447);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(866);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(51);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6447, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 6447, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 866, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 866, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 389, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 389, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 41: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST41(void)
{
    // informacje o teście
    test_start(41, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(330);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(7635);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(560);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(89);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 7635, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 7635, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 560, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 560, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 330, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 330, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(8205);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr2 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 8205, "Funkcja heap_validate() powinna zwrócić wartość 8205, a zwróciła na %llu", heap_get_largest_used_block_size());


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 42: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą
//
void UTEST42(void)
{
    // informacje o teście
    test_start(42, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(6956);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(369);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(685);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(58);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6956, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 6956, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 685, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 685, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 369, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 369, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 58, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 58, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 43: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST43(void)
{
    // informacje o teście
    test_start(43, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(7751);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(325);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(682);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(97);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 7751, "Funkcja heap_validate() powinna zwrócić wartość 7751, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 682, "Funkcja heap_validate() powinna zwrócić wartość 682, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 325, "Funkcja heap_validate() powinna zwrócić wartość 325, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 97, "Funkcja heap_validate() powinna zwrócić wartość 97, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(8774);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr1 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 8774, "Funkcja heap_validate() powinna zwrócić wartość 8774, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 44: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku
//
void UTEST44(void)
{
    // informacje o teście
    test_start(44, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(274);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(95);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 274, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 274, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 95, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 95, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr3 = heap_malloc(224);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 == ptr1, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(heap_get_largest_used_block_size() == 224, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 224, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                void *ptr4 = heap_malloc(274);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr4 == ptr1, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                test_error(heap_get_largest_used_block_size() == 274, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 274, a zwróciła na %llu", heap_get_largest_used_block_size());


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 45: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST45(void)
{
    // informacje o teście
    test_start(45, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(767);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(281);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(9721);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(68);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9721, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 9721, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 767, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 767, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr5 = heap_malloc(5955);
                test_error(ptr5 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr5 == ptr3, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));

                test_error(heap_get_largest_used_block_size() == 5955, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 5955, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr5);
                heap_free(ptr2);
                
                void *ptr6 = heap_malloc(1062);
                test_error(ptr6 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr6 == ptr2, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr6), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr6));

                test_error(heap_get_largest_used_block_size() == 1062, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1062, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 46: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST46(void)
{
    // informacje o teście
    test_start(46, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(731);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(260);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(9734);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(100);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9734, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 9734, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 731, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 731, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr5 = heap_malloc(7336);
                test_error(ptr5 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr5 == ptr3, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));

                test_error(heap_get_largest_used_block_size() == 7336, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 7336, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);
                heap_free(ptr5);

                void *ptr6 = heap_malloc(1001);
                test_error(ptr6 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr6 == ptr2, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr6), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr6));

                test_error(heap_get_largest_used_block_size() == 1001, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1001, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 47: Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST47(void)
{
    // informacje o teście
    test_start(47, "Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(5964);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(397);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_calloc(832, 1);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(92);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 5964, "Funkcja heap_validate() powinna zwrócić wartość 5964, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 832, "Funkcja heap_validate() powinna zwrócić wartość 832, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 397, "Funkcja heap_validate() powinna zwrócić wartość 397, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 92, "Funkcja heap_validate() powinna zwrócić wartość 92, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(7201);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr1 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 7201, "Funkcja heap_validate() powinna zwrócić wartość 7201, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 48: Sprawdzanie poprawności działania funkcji heap_malloc i heap_free
//
void UTEST48(void)
{
    // informacje o teście
    test_start(48, "Sprawdzanie poprawności działania funkcji heap_malloc i heap_free", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  srand (time(NULL));

                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  char *ptr[3160];
                  int ptr_state[3160] = {0};

                  int is_allocated = 0;

                  for (int i = 0; i < 3160; ++i)
                  {
                    if (rand() % 100 < 66)
                    {
                      for (int j = 0; j < 3160; ++j)
                        if (ptr_state[j] == 0)
                        {
                          ptr_state[j] = 1;
                          ptr[j] = heap_malloc(rand() % 100 + 50);
                          is_allocated++;
                          break;
                        }
                    }
                    else
                    {
                      if (is_allocated)
                      {
                        int to_free = rand() % is_allocated;
                         for (int j = 0; j < 3160; ++j)
                          {
                              if (ptr_state[j] == 1 && !to_free)
                              {
                                ptr_state[j] = 0;
                                is_allocated--;
                                heap_free(ptr[j]);
                                break;
                              }        
                              to_free--;
                          }
                      }
                    }
                  }

                  for (int j = 0; j < 3160; ++j)
                    if (ptr_state[j] == 1)
                      heap_free(ptr[j]);

                  test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 49: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST49(void)
{
    // informacje o teście
    test_start(49, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                test_error(get_pointer_type(NULL) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(NULL));

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 50: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST50(void)
{
    // informacje o teście
    test_start(50, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                test_error(get_pointer_type(NULL) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(NULL));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 51: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST51(void)
{
    // informacje o teście
    test_start(51, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(120);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 52: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST52(void)
{
    // informacje o teście
    test_start(52, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(136);
                char *ptr1 = heap_malloc(1432);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr);

                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr + 40) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 40));
                test_error(get_pointer_type(ptr + 136) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 136));
                test_error(get_pointer_type(ptr - 1) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 53: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST53(void)
{
    // informacje o teście
    test_start(53, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1096);
                char *ptr1 = heap_malloc(1536);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error(get_pointer_type(ptr + 1096) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 1096));
                test_error(get_pointer_type(ptr - 1) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 54: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST54(void)
{
    // informacje o teście
    test_start(54, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1424);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 1; i < 1424; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_inside_data_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_data_block, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 55: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST55(void)
{
    // informacje o teście
    test_start(55, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1368);
                char *ptr1 = heap_malloc(1072);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr1 + i) == pointer_control_block) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 56: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST56(void)
{
    // informacje o teście
    test_start(56, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(840);
                char *ptr1 = heap_malloc(1536);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = 840 + 1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr + i) == pointer_control_block) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 57: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST57(void)
{
    // informacje o teście
    test_start(57, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(840);
                char *ptr1 = heap_malloc(1112);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1, fences_size_before = 0, fences_size_after = 0;

                while (get_pointer_type(ptr + i) == pointer_inside_fences) 
                {
                    fences_size_before++;
                    --i;
                }

                i = 840 + 1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)             
                {
                    fences_size_after++;
                    ++i;
                }

                heap_free(ptr);

                for (int i = -fences_size_before; i < 840 + fences_size_after; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 58: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST58(void)
{
    // informacje o teście
    test_start(58, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(137, 139);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 59: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST59(void)
{
    // informacje o teście
    test_start(59, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1416, 18);
                char *ptr1 = heap_calloc(1240, 18);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr);

                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr + 49) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 49));
                test_error(get_pointer_type(ptr + 25488) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 1416));
                test_error(get_pointer_type(ptr - 1) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 60: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST60(void)
{
    // informacje o teście
    test_start(60, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1464, 10);
                char *ptr1 = heap_calloc(864, 10);


                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error(get_pointer_type(ptr + 14640) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 1464));
                test_error(get_pointer_type(ptr - 1) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 61: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST61(void)
{
    // informacje o teście
    test_start(61, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1416, 16);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 1; i < 22656; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_inside_data_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_data_block, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 62: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST62(void)
{
    // informacje o teście
    test_start(62, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1112, 19);
                char *ptr1 = heap_calloc(1152, 19);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr1 + i) == pointer_control_block) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 63: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST63(void)
{
    // informacje o teście
    test_start(63, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_validate();
                test_error(status == 2, "Funkcja heap_validate() powinna zwrócić wartość 2, a zwróciła na %d", status);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 64: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST64(void)
{
    // informacje o teście
    test_start(64, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(171);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
     
                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 65: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST65(void)
{
    // informacje o teście
    test_start(65, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(133);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                int i = -1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)
                {
                    char temp = ptr[i];
                    ptr[i] += 17;

                    status = heap_validate();
                    test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                    ptr[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    --i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 66: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST66(void)
{
    // informacje o teście
    test_start(66, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(144);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                int i = 144;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)
                {
                    char temp = ptr[i];
                    ptr[i] += 18;

                    status = heap_validate();
                    test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                    ptr[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    ++i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 67: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST67(void)
{
    // informacje o teście
    test_start(67, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(120);
                char *ptr1 = heap_malloc(120);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                while (get_pointer_type(ptr1 + i) == pointer_control_block)
                {
                    char temp = ptr1[i];
                    ptr1[i] += 19;

                    status = heap_validate();
                    test_error(status == 3, "Funkcja heap_validate() powinna zwrócić wartość 3, a zwróciła na %d", status);

                    ptr1[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    --i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 68: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST68(void)
{
    // informacje o teście
    test_start(68, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {483, 225, 888, 215, 490, 855, 326, 138, 325, 779, 767, 189, 566, 477, 636, 378, 916, 396, 428, 739, 867, 853, 404, 734, 213, 868, 202, 241, 682};
                 size_t mem_units[] = {4, 5, 1, 2, 5, 1, 3, 5, 4, 4, 1, 2, 5, 4, 4, 4, 4, 2, 4, 3, 1, 3, 2, 3, 5, 4, 5, 5, 2};
                 char *ptrs[29];

                 for (int i = 0; i < 29; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 ptrs[26][-1] += 10;
                 
                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 69: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST69(void)
{
    // informacje o teście
    test_start(69, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {422, 148, 959, 654, 342, 929, 202, 893, 258, 121, 972, 132, 280, 847, 936, 348, 983, 639, 839, 143, 193, 244, 436, 634, 930, 288, 795, 235, 106, 300, 419, 991, 651, 228, 490, 219, 428, 788};
                 size_t mem_units[] = {5, 3, 3, 2, 3, 2, 5, 2, 5, 2, 3, 1, 3, 1, 4, 5, 4, 5, 1, 4, 3, 4, 2, 3, 2, 1, 3, 1, 2, 1, 5, 1, 1, 5, 1, 4, 2, 2};
                 char *ptrs[38];

                 for (int i = 0; i < 38; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 ptrs[17][mem_sizes[17] * mem_units[17]] += 7;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 70: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST70(void)
{
    // informacje o teście
    test_start(70, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {698, 836, 573, 748, 139, 851, 482, 147, 804, 294, 511, 955, 493, 597, 425, 808, 322, 430, 405, 691, 712, 311};
                 size_t mem_units[] = {1, 5, 2, 2, 2, 3, 2, 2, 5, 3, 5, 4, 5, 5, 5, 2, 4, 4, 4, 1, 5, 2};
                 char *ptrs[22];

                 for (int i = 0; i < 22; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 int i = -1;

                 while (get_pointer_type(ptrs[15] + i) == pointer_inside_fences) --i;

                 ptrs[15][i - 7] += 9;

                 status = heap_validate();
                 test_error(status == 3, "Funkcja heap_validate() powinna zwrócić wartość 3, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 71: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST71(void)
{
    // informacje o teście
    test_start(71, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 0);

                test_error(ptr == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 72: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
//
void UTEST72(void)
{
    // informacje o teście
    test_start(72, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                void *ptr = heap_realloc(NULL, 173);

                test_error(ptr == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 73: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
//
void UTEST73(void)
{
    // informacje o teście
    test_start(73, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 116);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 74: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST74(void)
{
    // informacje o teście
    test_start(74, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 194);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                void *ptr1 = heap_realloc(ptr, 0);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 75: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
//
void UTEST75(void)
{
    // informacje o teście
    test_start(75, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_realloc(NULL, 154);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                void *ptr1 = heap_realloc(ptr + 67, 154);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
        
                ptr1 = heap_realloc(ptr - 30, 154);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 76: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
//
void UTEST76(void)
{
    // informacje o teście
    test_start(76, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                unsigned char values[136];
                
                for (int i = 0; i < 136; ++i)
                    values[i] = rand() % 255;

                char *ptr = heap_realloc(NULL, 136);
                char *ptr2 = heap_realloc(NULL, 136);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                memcpy(ptr, values, 136);

                void *ptr1 = heap_realloc(ptr, 66);

                test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                test_error(memcmp(ptr, values, 66) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                test_error(get_pointer_type(ptr + 66) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 66));

                 int i = 66;

                char temp = ptr[i];
                ptr[i] += 19;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                ptr[i] = temp;

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 77: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST77(void)
{
    // informacje o teście
    test_start(77, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[104];

                 for (int i = 0; i < 104; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 104);
                 char *ptr2 = heap_realloc(NULL, 104);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                 memcpy(ptr, values, 104);

                 void *ptr1 = heap_realloc(ptr, 104);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 104) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 104) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 104));

                  int i = 104;

                 char temp = ptr[i];
                 ptr[i] += 19;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 78: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST78(void)
{
    // informacje o teście
    test_start(78, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[128];

                 for (int i = 0; i < 128; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 128);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 memcpy(ptr, values, 128);

                 void *ptr1 = heap_realloc(ptr, 967);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 128) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 967) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 104));

                  int i = 967;

                 char temp = ptr[i];
                 ptr[i] += 19;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 79: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST79(void)
{
    // informacje o teście
    test_start(79, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[196];

                 for (int i = 0; i < 196; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 196);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 memcpy(ptr, values, 196);

                 void *ptr1 = heap_realloc(ptr, 11566997);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 196) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 11566997) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 104));

                  int i = 11566997;

                 char temp = ptr[i];
                 ptr[i] += 19;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 80: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST80(void)
{
    // informacje o teście
    test_start(80, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[185];

                  for (int i = 0; i < 185; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 185);
                  char *ptr2 = heap_realloc(NULL, 1978);
                  char *ptr3 = heap_realloc(NULL, 101);
                  char *ptr4 = heap_realloc(NULL, 121);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);
                
                  ptr2 = heap_realloc(NULL, 185);
                
                  heap_free(ptr3);

                  memcpy(ptr2, values, 185);

                  char *ptr5 = heap_realloc(ptr2, 2092);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 185) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 2092) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 2092));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 81: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST81(void)
{
    // informacje o teście
    test_start(81, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[197];

                  for (int i = 0; i < 197; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 197);
                  char *ptr2 = heap_realloc(NULL, 1357);
                  char *ptr3 = heap_realloc(NULL, 194);
                  char *ptr4 = heap_realloc(NULL, 169);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 197);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 197);

                  char *ptr5 = heap_realloc(ptr2, 1377);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 197) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 1377) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 1377));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 82: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST82(void)
{
    // informacje o teście
    test_start(82, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[187];

                  for (int i = 0; i < 187; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 187);
                  char *ptr2 = heap_realloc(NULL, 1170);
                  char *ptr3 = heap_realloc(NULL, 104);
                  char *ptr4 = heap_realloc(NULL, 157);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 187);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 187);

                  char *ptr5 = heap_realloc(ptr2, 3115);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr5, values, 187) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr5 + 3115) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr5 + 3115));
                  test_error(get_pointer_type(ptr2) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 83: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST83(void)
{
    // informacje o teście
    test_start(83, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[124];

                  for (int i = 0; i < 124; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 124);
                  char *ptr2 = heap_realloc(NULL, 1676);
                  char *ptr3 = heap_realloc(NULL, 129);
                  char *ptr4 = heap_realloc(NULL, 114);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 124);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 124);

                  char *ptr5 = heap_realloc(ptr2, 67937307);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 124) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 84: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST84(void)
{
    // informacje o teście
    test_start(84, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[197];

                  for (int i = 0; i < 197; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 147);
                  char *ptr2 = heap_realloc(NULL, 1710);
                  char *ptr3 = heap_realloc(NULL, 112);
                  char *ptr4 = heap_realloc(NULL, 197);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  memcpy(ptr4, values, 197);

                  char *ptr5 = heap_realloc(ptr4, 67158336);

                  test_error(ptr5 == NULL, "Funkcja heap_realloc() powinna zwrócić NULL");
                  test_error(memcmp(ptr4, values, 197) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  test_error(get_pointer_type(ptr4 + 197) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr4 + 197));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 85: Sprawdzanie poprawności działania funkcji heap_realloc
//
void UTEST85(void)
{
    // informacje o teście
    test_start(85, "Sprawdzanie poprawności działania funkcji heap_realloc", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                   srand (time(NULL));

                   int status = heap_setup();
                   test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                   char *ptr[3438];
                   int ptr_state[3438] = {0};

                   int is_allocated = 0;

                   for (int i = 0; i < 3438; ++i)
                   {
                     int rand_value = rand() % 100;
                     if (rand_value < 33)
                     {
                       for (int j = 0; j < 3438; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc(NULL, rand() % 1000 + 500);  
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 66)
                     {
                       if (is_allocated)
                       {
                          int to_reallocate = rand() % is_allocated;
                          for (int j = 0; j < 3438; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_reallocate)
                               {
                                   if (rand() % 100 < 50)
                                       ptr[j] = heap_realloc(ptr[j], rand() % 1000 + 500);
                                   else
                                       ptr[j] = heap_realloc(ptr[j], rand() % 10000 + 5000);    
                                   break;
                               }
                               to_reallocate--;
                           }
                       }
                     }
                     else
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 3438; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc(ptr[j], 0);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);           
                   }

                   for (int j = 0; j < 3438; ++j)
                     if (ptr_state[j] == 1)
                         heap_realloc(ptr[j], 0);

                   test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 86: Sprawdzanie poprawności działania funkcji heap_realloc, heap_malloc, heap_calloc i heap_free
//
void UTEST86(void)
{
    // informacje o teście
    test_start(86, "Sprawdzanie poprawności działania funkcji heap_realloc, heap_malloc, heap_calloc i heap_free", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                   srand (time(NULL));

                   int status = heap_setup();
                   test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                   char *ptr[698];
                   int ptr_state[698] = {0};

                   int is_allocated = 0;

                   for (int i = 0; i < 698; ++i)
                   {
                     int rand_value = rand() % 100;
                     if (rand_value < 16)
                     {
                       for (int j = 0; j < 698; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc(NULL, rand() % 100 + 50);  
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 33)
                     {
                       if (is_allocated)
                       {
                          int to_reallocate = rand() % is_allocated;
                          for (int j = 0; j < 698; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_reallocate)
                               {
                                   if (rand() % 100 < 50)
                                       ptr[j] = heap_realloc(ptr[j], rand() % 100 + 50);
                                   else
                                       ptr[j] = heap_realloc(ptr[j], rand() % 1000 + 500);    
                                   break;
                               }
                               to_reallocate--;
                           }
                       }
                     }
                     else if (rand_value < 50)
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 698; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc(ptr[j], 0);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     else if (rand_value < 66)
                     {
                       for (int j = 0; j < 698; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_malloc(rand() % 100 + 50);  
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 83)
                     {
                       for (int j = 0; j < 698; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_calloc(rand() % 100 + 50, rand() % 10 + 1);  
                           is_allocated++;
                           break;
                         }
                     }     
                     else
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 698; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_free(ptr[j]);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła %d", status);           
                   }

                   for (int j = 0; j < 698; ++j)
                     if (ptr_state[j] == 1)
                         heap_realloc(ptr[j], 0);

                   test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}




enum run_mode_t { rm_normal_with_rld = 0, rm_unit_test = 1, rm_main_test = 2 };

int __wrap_main(volatile int _argc, char** _argv, char** _envp)
{
    int volatile vargc = _argc;
    char ** volatile vargv = _argv, ** volatile venvp = _envp;
	volatile enum run_mode_t run_mode = rm_unit_test; // -1
	volatile int selected_test = -1;

    if (vargc > 1)
	{
	    char* smode = strtok(vargv[1], ",");
	    char* stest = strtok(NULL, "");
		char *errptr = NULL;
		run_mode = (enum run_mode_t)strtol(smode, &errptr, 10);
		if (*errptr == '\x0')
		{
			memmove(vargv + 1, vargv + 2, sizeof(char*) * (vargc - 1));
			vargc--;

			if (stest != NULL)
			{
			    int val = (int)strtol(stest, &errptr, 10);
			    if (*errptr == '\x0')
			        selected_test = val;
			}
		}
	}

    // printf("runmode=%d; selected_test=%d\n", run_mode, selected_test);

    // inicjuj testy jednostkowe
    unit_test_init(run_mode, "unit_test_v2.c");
    test_limit_init();
    rldebug_set_reported_severity_level(MSL_FAILURE);

    if (run_mode == rm_normal_with_rld)
    {
        // konfiguracja ograniczników
        rldebug_reset_limits();
        

        // uruchom funkcję main Studenta a potem wyświetl podsumowanie sterty i zasobów
        volatile int ret_code = rdebug_call_main(tested_main, vargc, vargv, venvp);

        rldebug_reset_limits();
        

        int leaks_detected = rldebug_show_leaked_resources(0);
        if (leaks_detected)
            raise(SIGHEAP);

        return ret_code;
    }

    
    if (run_mode == rm_unit_test)
    {
        test_title("Testy jednostkowe");

        void (*pfcn[])(void) =
        { 
            UTEST1, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST2, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST3, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST4, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST5, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST6, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST7, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST8, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST9, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST10, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
            UTEST11, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST12, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST13, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST14, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST15, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST16, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST17, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST18, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST19, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST20, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST21, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST22, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST23, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST24, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST25, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST26, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST27, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST28, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST29, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST30, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST31, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST32, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST33, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty
            UTEST34, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST35, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST36, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST37, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST38, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
            UTEST39, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST40, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
            UTEST41, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST42, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą
            UTEST43, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST44, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku
            UTEST45, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST46, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST47, // Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST48, // Sprawdzanie poprawności działania funkcji heap_malloc i heap_free
            UTEST49, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST50, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST51, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST52, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST53, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST54, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST55, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST56, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST57, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST58, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST59, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST60, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST61, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST62, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST63, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST64, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST65, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST66, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST67, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST68, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST69, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST70, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST71, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST72, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
            UTEST73, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
            UTEST74, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST75, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
            UTEST76, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
            UTEST77, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
            UTEST78, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST79, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST80, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST81, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST82, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST83, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST84, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST85, // Sprawdzanie poprawności działania funkcji heap_realloc
            UTEST86, // Sprawdzanie poprawności działania funkcji heap_realloc, heap_malloc, heap_calloc i heap_free
            NULL
        };

        for (int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx]();

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(86); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem
        return EXIT_SUCCESS;
    }
    

    if (run_mode == rm_main_test)
    {
        test_title("Testy funkcji main()");

        void (*pfcn[])(int, char**, char**) =
        { 
            NULL
        };

        for (volatile int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx](vargc, vargv, venvp);

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(0); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem

        return EXIT_SUCCESS;
    }

    printf("*** Nieznana wartość RunMode: %d", (int)run_mode);
    abort();
}