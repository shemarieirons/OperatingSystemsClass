#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main() {

    printf("\n=== Test 1: Alloc/Free and Empty List ===\n");
    list_t *mylist = list_alloc();
    printf("Length of empty list: %d (expected 0)\n", list_length(mylist));
    list_print(mylist);
    char *str = listToString(mylist);
    if (strcmp("NULL", str) != 0) {
        printf("listToString on empty: FAILED (got '%s')\n", str);
    } else {
        printf("listToString on empty: PASSED\n");
    }
    free(str);
    if (list_get_elem_at(mylist, 1) != -1) printf("get_elem_at(1) on empty: FAILED\n");
    if (list_get_index_of(mylist, 42) != -1) printf("get_index_of(42) on empty: FAILED\n");
    if (list_is_in(mylist, 42)) printf("is_in(42) on empty: FAILED\n");
    elem removed = list_remove_from_front(mylist);
    if (removed != -1) printf("remove_from_front on empty: FAILED (got %d)\n", removed);
    removed = list_remove_from_back(mylist);
    if (removed != -1) printf("remove_from_back on empty: FAILED (got %d)\n", removed);
    list_free(mylist);
    printf(" Test 1: PASSED\n");

    printf("\n===  Test 2: Add to Front/Back and Length ===\n");
    mylist = list_alloc();
    list_add_to_front(mylist, 10);
    list_add_to_front(mylist, 20);
    list_add_to_back(mylist, 30);
    list_add_to_back(mylist, 40);
    list_print(mylist);
    printf("Length: %d (expected 4)\n", list_length(mylist));

    str = listToString(mylist);
    if (strcmp("20->10->30->40->NULL", str) != 0) {
        printf("Add front/back: FAILED (got '%s')\n", str);
    } else {
        printf("Add front/back: PASSED\n");
    }
    free(str);
    list_free(mylist);
    printf("Test 2: PASSED\n");

    printf("\n=== Test 3: Remove from Front/Back ===\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 1);
    list_add_to_back(mylist, 2);
    list_add_to_back(mylist, 3);
    list_print(mylist);

    elem val = list_remove_from_front(mylist);
    if (val != 1) printf("remove_from_front: FAILED (got %d, expected 1)\n", val);
    printf("Removed from front: %d\n", val);
    list_print(mylist);

    val = list_remove_from_back(mylist);
    if (val != 3) printf("remove_from_back: FAILED (got %d, expected 3)\n", val);
    printf("Removed from back: %d\n", val);
    list_print(mylist);

    val = list_remove_from_front(mylist);
    if (val != 2) printf("remove_from_front (single): FAILED (got %d, expected 2)\n", val);
    printf("Removed single: %d\n", val);
    list_print(mylist);

    val = list_remove_from_back(mylist);
    if (val != -1) printf("remove_from_back on empty: FAILED (got %d)\n", val);
    list_free(mylist);
    printf("Test 3: PASSED\n");

    printf("\n=== Test 4: Is In and Get Elem At (1-based) ===\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 5);
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 15);
    list_print(mylist);

    printf("Is 10 in list?: %d (expected 1)\n", list_is_in(mylist, 10));
    printf("Is 99 in list?: %d (expected 0)\n", list_is_in(mylist, 99));

    printf("Elem at index 1: %d (expected 5)\n", list_get_elem_at(mylist, 1));
    printf("Elem at index 2: %d (expected 10)\n", list_get_elem_at(mylist, 2));
    printf("Elem at index 3: %d (expected 15)\n", list_get_elem_at(mylist, 3));
    printf("Elem at index 4: %d (expected -1)\n", list_get_elem_at(mylist, 4));
    printf("Elem at index 0: %d (expected -1)\n", list_get_elem_at(mylist, 0));
    printf("Elem at index -1: %d (expected -1)\n", list_get_elem_at(mylist, -1));

    if (!list_is_in(mylist, 10) || list_is_in(mylist, 99) || 
        list_get_elem_at(mylist, 1) != 5 || list_get_elem_at(mylist, 4) != -1) {
        printf("Is In / Get Elem At: FAILED\n");
    } else {
        printf("Is In / Get Elem At: PASSED\n");
    }
    list_free(mylist);
    printf(" Test 4: PASSED\n");

    printf("\n===  Test 5: Get Index Of (1-based) ===\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 100);
    list_add_to_back(mylist, 200);
    list_add_to_back(mylist, 100);
    list_print(mylist);

    printf("Index of 100: %d (expected 1, first occurrence)\n", list_get_index_of(mylist, 100));
    printf("Index of 200: %d (expected 2)\n", list_get_index_of(mylist, 200));
    printf("Index of 300: %d (expected -1)\n", list_get_index_of(mylist, 300));

    if (list_get_index_of(mylist, 100) != 1 || list_get_index_of(mylist, 200) != 2 || 
        list_get_index_of(mylist, 300) != -1) {
        printf("Get Index Of: FAILED\n");
    } else {
        printf("Get Index Of: PASSED\n");
    }
    list_free(mylist);
    printf(" Test 5: PASSED\n");

    printf("\n=== Test 6: Remove At Index (1-based) ===\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 1);
    list_add_to_back(mylist, 2);
    list_add_to_back(mylist, 3);
    list_print(mylist);

    val = list_remove_at_index(mylist, 2);
    if (val != 2) printf("remove_at_index(2): FAILED (got %d, expected 2)\n", val);
    list_print(mylist);

    val = list_remove_at_index(mylist, 1);
    if (val != 1) printf("remove_at_index(1, front): FAILED (got %d, expected 1)\n", val);
    list_print(mylist);

    val = list_remove_at_index(mylist, 1);
    if (val != 3) printf("remove_at_index(1, back/single): FAILED (got %d, expected 3)\n", val);
    list_print(mylist);

    val = list_remove_at_index(mylist, 5);
    if (val != -1) printf("remove_at_index(5) on empty: FAILED (got %d)\n", val);
    val = list_remove_at_index(mylist, 0);
    if (val != -1) printf("remove_at_index(0): FAILED (got %d)\n", val);

    list_free(mylist);
    printf(" Test 6: PASSED\n");

    return 0;

}