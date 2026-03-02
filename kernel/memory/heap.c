#include "heap.h"
#include <stdint.h>

#define HEAP_START 0x01000000   // 16MB
#define HEAP_SIZE  0x00100000   // 1MB

typedef struct block_header {
    uint32_t size;
    uint8_t free;
    struct block_header* next;
} block_header_t;

static block_header_t* head = 0;

void heap_init() {
    head = (block_header_t*)HEAP_START;

    head->size = HEAP_SIZE - sizeof(block_header_t);
    head->free = 1;
    head->next = 0;
}

void* kmalloc(uint32_t size) {

    if (size == 0)
        return 0;

    block_header_t* current = head;

    while (current) {

        if (current->free && current->size >= size) {

            // Split only if enough space for another block
            if (current->size > size + sizeof(block_header_t)) {

                block_header_t* new_block =
                    (block_header_t*)((uint32_t)current
                    + sizeof(block_header_t) + size);

                new_block->size =
                    current->size - size - sizeof(block_header_t);

                new_block->free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->free = 0;

            return (void*)((uint32_t)current
                    + sizeof(block_header_t));
        }

        current = current->next;
    }

    return 0;
}
void kfree(void* ptr) {

    if (!ptr)
        return;

    block_header_t* block =
        (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));

    block->free = 1;

    block_header_t* current = head;

    while (current && current->next) {

        uint32_t current_end =
            (uint32_t)current + sizeof(block_header_t) + current->size;

        if (current->free &&
            current->next->free &&
            current_end == (uint32_t)current->next) {

            current->size += sizeof(block_header_t)
                             + current->next->size;

            current->next = current->next->next;
        }
        else {
            current = current->next;
        }
    }
}
