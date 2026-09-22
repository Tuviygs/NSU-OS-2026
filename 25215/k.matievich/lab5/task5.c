#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct stroke {
    off_t position;
    size_t len;
} stroke_t;

typedef struct stroke_table {
     stroke_t* table;
     size_t cap;
     size_t size;
} str_tab;


int main(int argc, char** argv) {


    if (argc != 2) {
        write(2, "Unknown command. Correct use is: \"./task5 <file_name>\n",
            sizeof("Unknown command. Correct use is: \"./task5 <file_name>\n") - 1);
        return 1;
    }

    str_tab* table = (str_tab*)malloc(sizeof(str_tab));
    table->cap = 16;
    table->table = (stroke_t*)malloc(table->cap * sizeof(stroke_t));
    table->size = 0;

    char* fileName = argv[1];
    int fildes = open(fileName, O_RDONLY);

    if (fildes == -1) {
        write(2, "There's no such file in this directory\n", sizeof("There's no such file in this directory\n") - 1);
        free(table->table);
        free(table);
        return 2;
    }

    
    char buf[BUFSIZ];
    ssize_t bytesRead;
    off_t offset = 0;
    size_t curSize = 0;
    off_t curOfffset = 0;


    bytesRead = read(fildes, buf, BUFSIZ);



    while (bytesRead > 0) {
        for (int i = 0; i < bytesRead; i++) {
            curSize++;
            if (buf[i] == '\n') {

                if (table->size == table->cap-1) {
                    table->cap *= 2;
                    table->table = (stroke_t*)realloc(table->table, table->cap*sizeof(stroke_t));
                }

                table->table[table->size].len = curSize;
                curSize = 0;
                table->table[table->size].position = curOfffset;
                curOfffset = offset + 1;
                table->size++;
            }
            offset++;
        }

        bytesRead = read(fildes, buf, BUFSIZ);
    }

    if (curSize != 0) {
        if (table->size == table->cap-1) {
            table->cap += 2;
            table->table = (stroke_t*)realloc(table->table, table->cap*sizeof(stroke_t));
        }

        table->table[table->size].len = curSize;
        table->table[table->size].position = curOfffset;
        table->size++;
    }


    write(1, "Enter the line number for which you want to get information or \"0\" to exit.\n",
         sizeof("Enter the line number for which you want to get information or \"0\" to exit.\n")) - 1;

    char inputBuf[BUFSIZ];
    int lineNum;
    
    while ((lineNum = read(0, inputBuf ,BUFSIZ)) > 0) {

        char* endPtr;

        long long lineNum = strtoll(inputBuf, &endPtr, 10);

        if (endPtr == inputBuf || (*endPtr) != '\n') {
            write(2, "Incorrect input. Try again\n", sizeof("Incorrect input. Try again\n"));
            write(1, "Enter the line number for which you want to get information or \"0\" to exit.\n",
                sizeof("Enter the line number for which you want to get information or \"0\" to exit.\n")) - 1;
            continue;
        }


        if (lineNum == 0) {
            write(1, "Exiting the programm\n", sizeof("Exiting the programm\n"));
            break;
        }

        if (lineNum < 0 || lineNum > (long long)table->size) {
            write(2, "There's no such line in this file. Try again\n", sizeof("There's no such line in this file. Try again\n") - 1);
            write(1, "Enter the line number for which you want to get information or \"0\" to exit.\n",
                sizeof("Enter the line number for which you want to get information or \"0\" to exit.\n") - 1);
            continue;
        }


        off_t newOffset = table->table[lineNum-1].position;
        size_t lineSize = table->table[lineNum-1].len;
        lseek(fildes, newOffset, SEEK_SET);
        char newBuf[lineSize + 3];

        size_t newBytesRead = read(fildes, newBuf, lineSize);

        newBuf[newBytesRead] = '\n';
        newBytesRead++;
        lineSize++;
        
        newBuf[newBytesRead] = '\0';
        write(1, newBuf, lineSize);

        write(1, "Enter the line number for which you want to get information or \"0\" to exit.\n",
         sizeof("Enter the line number for which you want to get information or \"0\" to exit.\n") - 1);
    }

    free(table->table);
    free(table);
    close(fildes);
    return 0;
}