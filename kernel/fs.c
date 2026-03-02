#include "fs.h"
#include "drivers/screen.h"
#include "lib/string.h"

#define FILE_COUNT 3

typedef struct {
    const char* name;
    const char* content;
} file_t;

static file_t files[FILE_COUNT] = {
    { "readme.txt", "Welcome to SimpleOS!\nThis is a demo file.\n" },
    { "notes.txt", "Kernel project notes:\n- IRQ working\n- FS working\n" },
    { "kernel.log", "System boot successful.\nNo errors detected.\n" }
};

void fs_list() {
    for (int i = 0; i < FILE_COUNT; i++) {
        print_string(files[i].name);
        print_char('\n');
    }
}

void fs_cat(const char* filename) {
    for (int i = 0; i < FILE_COUNT; i++) {
        if (strcmp(filename, files[i].name) == 0) {
            print_string(files[i].content);
            return;
        }
    }

    print_string("File not found.\n");
}
