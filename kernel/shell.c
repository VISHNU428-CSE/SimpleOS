#include "lib/string.h"
#include <stdint.h>
#include "drivers/screen.h"
#include "shell.h"

#include "fs.h"
#define PROMPT "SimpleOS> "

extern volatile uint32_t ticks;

void shell_init(void) {
    print_string(PROMPT);
}

void process_command(char* input) {

    char* p = input;

    while (*p == ' ') p++;

    if (*p == '\0') {
        print_string("\n");
        print_string(PROMPT);
        return;
    }

    char* cmd = p;

    while (*p && *p != ' ') p++;

    char* args = "";

    if (*p) {
        *p = '\0';
        p++;
        while (*p == ' ') p++;
        args = p;
    }

    // ---------------- COMMANDS ----------------

    if (strcmp(cmd, "help") == 0) {

        print_string("\nAvailable commands:\n");
        print_string("  help     - Show command list\n");
        print_string("  clear    - Clear screen\n");
        print_string("  echo     - Print text\n");
        print_string("  uptime   - Show system ticks\n");
        print_string("  version  - Show kernel version\n");
        print_string("  ls       - List files\n");
        print_string("  cat      - Display file contents\n");

    }

    else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    }

    else if (strcmp(cmd, "echo") == 0) {

        print_string("\n");

        if (*args)
            print_string(args);
        else
            print_string("(no text provided)");
    }

    else if (strcmp(cmd, "uptime") == 0) {

        print_string("\nTicks: ");
        print_int(ticks);
    }

    else if (strcmp(cmd, "version") == 0) {

        print_string("\nSimpleOS Kernel Version 1.0\n");
        print_string("Architecture: 32-bit x86\n");
    }

    else if (strcmp(cmd, "ls") == 0) {

        print_string("\n");
        fs_list();
    }

    else if (strcmp(cmd, "cat") == 0) {

        if (*args == '\0') {
            print_string("\nUsage: cat <filename>");
        } else {
            print_string("\n");
            fs_cat(args);
        }
    }

    else {
        print_string("\nCommand not found");
    }

    print_string("\n");
    print_string(PROMPT);
}
