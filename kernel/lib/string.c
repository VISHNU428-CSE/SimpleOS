int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}
int strncmp(const char* a, const char* b, int n) {
    while (n-- > 0 && *a && (*a == *b)) {
        a++;
        b++;
    }

    if (n < 0) return 0;

    return *(const unsigned char*)a - *(const unsigned char*)b;
}
