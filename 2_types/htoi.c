#include <stdio.h>
#include <math.h>
#define MAXLEN 1000

int htoi(const char hex[]);
int string_size(const char str[]);

int main() {
    char input[MAXLEN] = "0xA1b2C3d4\n\0";
    printf("%d\n", htoi(input));
}

// power
int string_size(const char str[]) {
    int length;
    for (length = 0; str[length] != '\n'; length++);
    return length;
}

int htoi(const char hex[]) {
    int result = 0;
    int len = string_size(hex);
    for (int i = len - 1; hex[i] != 'x' && i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            result += (hex[i]-'0')*pow(16, (len - 1) - i);
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            result += (10+hex[i]-'A')*pow(16, (len - 1) - i);
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            result += (10+hex[i]-'a')*pow(16, (len - 1) - i);
        }
        printf("%d\n", result);
    }
    return result;
}
