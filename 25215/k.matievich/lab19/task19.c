#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <fnmatch.h>


int main() {
    write(1, "Введите шаблон имени файла:\n", strlen("Введите шаблон имени файла:\n"));
    char buf[BUFSIZ];
    ssize_t bytesread = read(0, buf, BUFSIZ);
    
    if (buf[bytesread - 1] == '\n') {
        buf[bytesread - 1] = '\0';
    } else {
        buf[bytesread] = '\0';
    }

    char curdir[PATH_MAX];
    getcwd(curdir, PATH_MAX);

    DIR* dir = opendir(curdir);

    struct dirent* curfile = readdir(dir);
    int cnt = 0;
    while(curfile != NULL) {

        if (fnmatch(buf, curfile->d_name, 0) == 0) {
            write(1, curfile->d_name, strlen(curfile->d_name));
            write(1, "\n", 2);
            cnt++;
        }
        curfile = readdir(dir);
    }
    closedir(dir);
    
    return 0;
}