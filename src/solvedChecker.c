#include <stdio.h>

int main() {
    FILE *prob = fopen("\doc\problem.txt", "r");
    FILE *soln = fopen("\doc\solution.txt", "r");

    if (!prob || !soln) {
        printf("Error opening files.\n");
        return 1;
    }

    int ch1, ch2;

    while (1) {
        ch1 = getc(prob);
        ch2 = getc(soln);

        if (ch1 == EOF && ch2 == EOF) {
            printf("Congratulations ! Good Game\n");
            break;
        }

        if (ch1 != ch2) {
            printf("Oops! Wrong Solution\n");
            break;
        }
    }

    fclose(prob);
    fclose(soln);
    return 0;
}
