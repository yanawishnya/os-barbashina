#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

char alph[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
               't', 'u', 'v', 'w', 'x', 'y', 'z'};

int main(void) {
    const char first_file_name[] = "a";
    char pathname[MAX_SIZE] = "", pathname2[MAX_SIZE] = ""; // Вспомогательные строки для хранения названий файлов-связей
    strcat(pathname, first_file_name);
    strcat(pathname2, first_file_name);
    int counter = 0, counter2 = 1, rec_depth = 0;
    FILE *f = fopen(first_file_name, "w+"); // Создаем файл 'a' если его нет
    fclose(f);

    while (1) {
        if (counter2 < strlen(alph)) { // Изменяем название последующих файлов
            pathname2[counter] = alph[counter2++];
        } else {
            pathname2[++counter] = alph[0];
            counter2 = 1;
        }

        symlink(pathname, pathname2); // Создаем связь

        for (int i = 0; pathname2[i] != '\0'; i++) {
            pathname[i] = pathname2[i]; // Копируем вторую строку в первую для продолжения цикла создания связей
        }

        FILE *f;
        if ((f = fopen(pathname, "w+")) == NULL) { // Пытаемся открыть файл
            printf("Recursion depth: %i\n", rec_depth); // Выводим получившуюся глубину рекурсии
            exit(-1);
        } else {
            fclose(f); // Если файл успешно открылся, его надо закрыть
            rec_depth++; // Если файл открылся успешно, увеличиваем допустимую глубину рекурсии
        }
    }

    return 0;
}
