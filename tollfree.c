#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

int* random800(void);
static int get_line(char *prmpt, char *buff, size_t sz);

int main(void)
{
    int *ph_nums = 0;
    size_t ph_nums_size = 0;
    int line_status;
    char comments[160];
    int *called_num = 0;

    srand(time(0)); /* current time as seed of random number generator */
    called_num = random800();

    /* make the digits of a called_num into a single int */
    int i, k = 0;
    for (i = 0; i < 7; i++) {
        k = 10 * k + *(called_num + i);
    }

    FILE *readlogfd = fopen("/home/cth/.tollfree/800.called", "r");

    if (readlogfd == NULL) {
        printf("Cannot open number log file.\n");
        return EXIT_FAILURE;
    }
    else {
        int ch;
        while ((ch = fgetc(readlogfd)) != EOF) {
            if (ch == '\n') {
                ph_nums_size++;
                // printf("%zu\n", ph_nums_size);
            }
        } 
        rewind(readlogfd);

        ph_nums = (int *) malloc(sizeof(int[ph_nums_size]));

        /* scan all lines of text file */
        for (i = 0; i < ph_nums_size; i++) {
            fscanf(readlogfd, "%d\n", &ph_nums[i]);
            // printf("read from file: %d\n", ph_nums[i]);
            if (ph_nums[i] == k) {
                perror("You hit a bingo! 800 number already called!");
                return EXIT_FAILURE;
            }
            else
                continue;
        }
        fclose(readlogfd);
    }

    line_status = get_line("Comments: ", comments, sizeof(comments));
    if (line_status == NO_INPUT) {
        /* in event of ctrl-d/EOF */
        printf("\nNo input\n");
        return EXIT_FAILURE;
    }

    if (line_status == TOO_LONG) {
        printf("Input too long [%s]\n", comments);
        return EXIT_FAILURE;
    }

    FILE *writelogfd = fopen("/home/cth/.tollfree/800.scan", "a");
    if (writelogfd == NULL) {
        printf("Cannot open number log file.\n");
        return EXIT_FAILURE;
    }
    else {
        fprintf(writelogfd, "800-%d-", k / 10000);
        fprintf(writelogfd, "%d ", k % 10000);
        fputs(comments, writelogfd);
        fprintf(writelogfd, "\n");
        fclose(writelogfd);
    }

    FILE *scanlogfd = fopen("/home/cth/.tollfree/800.called", "a");
    if (scanlogfd == NULL) {
        printf("Cannot open comments log file.\n");
        return EXIT_FAILURE;
    }
    else {
        fprintf(scanlogfd, "%d\n", k); 
        fclose(scanlogfd);
    }

    if (ph_nums) {
        free(ph_nums);
    }

    return EXIT_SUCCESS;
}

int* random800(void)
{ 
    int i;
    int pass_count = 7;
    int printed = 0;
    static int num[7];
    int ret;

    printf("800-");
    for (i = 0; i < pass_count; i++) {
        int rand_num = rand() % 10;
        if (printed == 0) {
            if (rand_num != 0 && rand_num != 1) {
                printf("%d", rand_num);
                num[printed] = rand_num;
                printed++;
              }
            else {
                pass_count++;
            }
        }
        else {
            printf("%d", rand_num);
            num[printed] = rand_num;
            printed++;
        }
        if (printed == 3)
           printf("-");
    }
     printf("\n");
    return num;
}

static int get_line(char *prmpt, char *buff, size_t sz)
{
    int ch, extra;

    /* Get line with buffer overrun protection */
    if (prmpt != NULL) {
        printf("%s", prmpt);
        fflush(stdout);
    }
    if (fgets(buff, sz, stdin) == NULL)
        return NO_INPUT;

    /* if it was too long, there'll be no newline. In that case, we flush
    to end of line so that excess doesn't affect the next call */
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    /* otherwise remove newline and give string back to caller */
    buff[strlen(buff)-1] = '\0';
    return OK;
}