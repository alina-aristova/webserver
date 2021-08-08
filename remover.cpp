#include <stdio.h>

int main() {
    if (remove("./root/root1/dice_6.png"))
        printf("something went wrong\n");
    return (1);
}