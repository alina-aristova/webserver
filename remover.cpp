#include <stdio.h>

int main() {
    if (remove("./root/root1/dice_2.png"))
        printf("something went wrong\n");
    return (1);
}