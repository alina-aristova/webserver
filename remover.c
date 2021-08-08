#include <stdio.h>

int main() {
    if (remove("./root/dice_3.png"))
        printf("something went wrong\n");
    return (1);
}