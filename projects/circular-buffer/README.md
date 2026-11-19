# Embedded Circular Buffer Library

A lightweight static circular buffer library implemented in C for embedded systems.

## Overview

This project implements a fixed-size circular buffer (ring buffer) using static memory allocation.

The library provides basic FIFO (First-In, First-Out) operations, including initialization, data insertion, data retrieval, and element counting.

The implementation is designed for resource-constrained embedded systems and does not use dynamic memory allocation.

## Features

- Static memory allocation
- FIFO data management
- Fixed-size buffer
- O(1) push and pop operations
- Full and empty state detection
- Wraparound support
- Null pointer validation
- Unit testing

## Architecture & Design

### Buffer Configuration

- Physical buffer size: 8 elements
- Usable capacity: 7 elements
- Memory allocation: Static allocation
- Data type: `uint8_t`
- Buffer organization: Circular array

### Buffer Management

This implementation uses the **Reserve One Slot** design.

One slot is intentionally left unused to distinguish between the full and empty states.

The buffer maintains two indices:

- `head`: Points to the next element to be read.
- `tail`: Points to the next available position for writing.

### Buffer States

**Empty condition:**

```c
head == tail
```

**Full condition:**

```c
(tail + 1) % BUFFER_SIZE == head
```

**Element count:**

```c
(tail - head + BUFFER_SIZE) % BUFFER_SIZE
```

### Complexity

| Operation | Time Complexity |
|---|---|
| Initialization | O(1) |
| Push | O(1) |
| Pop | O(1) |
| Count | O(1) |

## API Reference

The library provides four public functions.

| Function | Description |
|---|---|
| `ring_buffer_init()` | Initializes the ring buffer. |
| `ring_buffer_push()` | Inserts an element into the buffer. |
| `ring_buffer_pop()` | Retrieves and removes the oldest element. |
| `ring_buffer_count()` | Returns the number of stored elements. |

### Function Behavior

#### `ring_buffer_init()`

- Initializes `head` and `tail` to zero.
- Safely handles a NULL buffer pointer.

#### `ring_buffer_push()`

- Inserts an element if the buffer is not full.
- Returns `1` on success.
- Returns `0` if the buffer is full or the buffer pointer is NULL.

#### `ring_buffer_pop()`

- Retrieves the oldest element if the buffer is not empty.
- Returns `1` on success.
- Returns `0` if the buffer is empty, the buffer pointer is NULL, or the output pointer is NULL.
- Does not modify the output value when the operation fails.

#### `ring_buffer_count()`

- Returns the current number of stored elements.
- Returns `0` if the buffer pointer is NULL.

## Build Instructions

### Requirements

- GCC compiler
- Linux or Ubuntu environment
- Terminal

### Compile and Link

Build the project using:

```bash
gcc -Wall -Wextra -Werror -std=c11 ring_buffer.c test_ring_buffer.c -o ring_buffer
```

### Run Tests

Execute the test program:

```bash
./ring_buffer
```

## Test Results

The library was validated using a custom unit test suite.

### Test Coverage

- Pop from an empty buffer
- NULL pointer handling
- Push multiple elements
- Push when the buffer is full
- FIFO ordering
- Wraparound behavior

### Test Summary

| Metric | Result |
|---|---:|
| Total assertions | 82 |
| Passed assertions | 82 |
| Failed assertions | 0 |

**Result:** All 82 assertions passed successfully.

## Limitations

- Fixed buffer size; capacity cannot be changed at runtime.
- No dynamic memory allocation.
- Designed for single-threaded operation.
- No thread-safety or interrupt-safety guarantees.
- No RTOS synchronization mechanisms are implemented.