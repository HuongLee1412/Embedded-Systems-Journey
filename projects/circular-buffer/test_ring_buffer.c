#include "ring_buffer.h"
#include <stdio.h>

static int total_assertions=0;
static int passed_assertions=0;
static int failed_assertions=0;

void test_assert_equal(int expected, int actual, const char *test_name){
    if(expected == actual){
        printf("[PASS] %s\n", test_name);
        passed_assertions++;
    }
    else{
        printf("[FAIL] %s | Expected: %d | Actual: %d\n", test_name, expected, actual);
        failed_assertions++;
    }
    total_assertions++;
}

void test_report(void){
    printf("=======ASSERTIONS======\n");
    printf("TOTAL ASSERTIONS: %d\n", total_assertions);
    printf("PASSED ASSERTIONS: %d\n", passed_assertions);
    printf("FAILED ASSERTIONS: %d\n", failed_assertions);
    printf("=======END======\n");
}

void test_pop_when_empty(void){
    RingBuffer rb;
    uint8_t value = 99;
    ring_buffer_init(&rb);
    test_assert_equal(0, ring_buffer_pop(&rb, &value), "pop_empty_returns_failure");
    test_assert_equal(99, value, "pop_empty_preserves_output");
    test_assert_equal(0, rb.head, "head_0");
    test_assert_equal(0, rb.tail, "tail_0");
    test_assert_equal(0, ring_buffer_count(&rb), "count_0");
}

void test_null_pointer_handling(void){
    RingBuffer rb;
    uint8_t value = 99;
    ring_buffer_init(&rb);
    test_assert_equal(0, ring_buffer_push(NULL, 10), "push_null_buffer_returns_failure");
    test_assert_equal(0, ring_buffer_pop(NULL, &value), "pop_null_buffer_returns_failure");
    test_assert_equal(0, ring_buffer_pop(&rb, NULL), "pop_null_data_returns_failure");
}

void test_push_multiple_elements(void){
    RingBuffer rb;
    ring_buffer_init(&rb);
    test_assert_equal(1, ring_buffer_push(&rb, 10), "push_10_success");
    test_assert_equal(1, ring_buffer_push(&rb, 20), "push_20_success");
    test_assert_equal(1, ring_buffer_push(&rb, 30), "push_30_success");
    test_assert_equal(0, rb.head, "head_0");
    test_assert_equal(3, rb.tail, "tail_3");
    test_assert_equal(3, ring_buffer_count(&rb), "count_3");
}

void test_push_when_full(void){
    RingBuffer rb;
    ring_buffer_init(&rb);
    for(int i = 1; i <= 7; i++){
        test_assert_equal(1, ring_buffer_push(&rb, i * 10), "push_until_full_success");
    }
    test_assert_equal(0, ring_buffer_push(&rb, 80), "push_80_when_full_returns_failure");
    test_assert_equal(7, ring_buffer_count(&rb), "count_7");
    test_assert_equal(0, rb.head, "head_0");
    test_assert_equal(7, rb.tail, "tail_7");
}

void test_fifo_order(void){
    RingBuffer rb;
    ring_buffer_init(&rb);
    uint8_t value = 99;
    test_assert_equal(1, ring_buffer_push(&rb, 10), "push_10_success");
    test_assert_equal(1, ring_buffer_push(&rb, 20), "push_20_success");
    test_assert_equal(1, ring_buffer_push(&rb, 30), "push_30_success");
    test_assert_equal(1, ring_buffer_pop(&rb, &value), "pop_10_success");
    test_assert_equal(10, value, "pop_10_returns_expected_value");
    test_assert_equal(1, ring_buffer_pop(&rb, &value), "pop_20_success");
    test_assert_equal(20, value, "pop_20_returns_expected_value");
    test_assert_equal(1, ring_buffer_push(&rb, 40), "push_40_success");
    test_assert_equal(1, ring_buffer_push(&rb, 50), "push_50_success");
    test_assert_equal(1, ring_buffer_pop(&rb, &value), "pop_30_success");
    test_assert_equal(30, value, "pop_30_returns_expected_value");
    test_assert_equal(1, ring_buffer_pop(&rb, &value), "pop_40_success");
    test_assert_equal(40, value, "pop_40_returns_expected_value");
    test_assert_equal(1, ring_buffer_pop(&rb, &value), "pop_50_success");
    test_assert_equal(50, value, "pop_50_returns_expected_value");
    test_assert_equal(5, rb.head, "head_5");
    test_assert_equal(5, rb.tail, "tail_5");
    test_assert_equal(0, ring_buffer_count(&rb), "count_0");
}

void test_wraparound(void){
    RingBuffer rb;
    ring_buffer_init(&rb);

    uint8_t popped_data = 99;
    uint8_t expected_pre_wrap_data[] = {10, 20, 30, 40};
    uint8_t expected_post_wrap_data[] = {50, 60, 70, 80, 90, 100, 110};

    for(uint8_t i = 1; i <= 7; i++){
        test_assert_equal(1, ring_buffer_push(&rb, i * 10), "push_initial_to_full_success");
    }
    for(uint8_t i = 0; i < 4; i++){
        test_assert_equal(1, ring_buffer_pop(&rb, &popped_data), "pop_pre_wrap_success");
        test_assert_equal(expected_pre_wrap_data[i], popped_data, "pop_pre_wrap_matches_expected");
    }
    for(uint8_t k = 8; k <= 11; k++){
        test_assert_equal(1, ring_buffer_push(&rb, k * 10), "push_wraparound_success");
    }
    test_assert_equal(3, rb.tail, "tail_3_after_wraparound");
    test_assert_equal(4, rb.head, "head_4_after_wraparound");
    test_assert_equal(7, ring_buffer_count(&rb), "count_7_after_wraparound");
    for(uint8_t i = 0; i < 7; i++){
        test_assert_equal(1, ring_buffer_pop(&rb, &popped_data), "pop_remaining_success");
        test_assert_equal(expected_post_wrap_data[i], popped_data, "pop_remaining_matches_expected");
    }

    test_assert_equal(3, rb.tail, "tail_3_when_empty");
    test_assert_equal(3, rb.head, "head_3_when_empty");
    test_assert_equal(0, ring_buffer_count(&rb), "count_0_when_empty");
}

int main(void){
    test_pop_when_empty();
    test_null_pointer_handling();
    test_push_multiple_elements();
    test_push_when_full();
    test_fifo_order();
    test_wraparound();
    test_report();
    return 0;
}