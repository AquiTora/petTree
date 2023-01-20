//Использование двоичного дерева поиска
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "tree.h"

#define SLEN 40

char menu(void);
void addpet(Tree * pt);
void droppet(Tree * pt);
void showpets(const Tree * pt);
void findpet(const Tree * pt);
void printitem(Item item);
void uppercase(char * str);
char * s_gets(char * st, int n);

int main(void)
{
    Tree pets;
    char choice;

    InitializeTree(&pets);
    while((choice = menu()) != 'q')
    {
        switch(choice)
        {
            case 'a' : addpet(&pets);
              break;
            case 'l' : showpets(&pets);
              break;
            case 'f' : findpet(&pets);
              break;
            case 'n' : printf("%d животных в клубе \n", TreeItemCount(&pets));
              break;
            case 'd' : droppet(&pets);
              break;
            default : puts("Ошибка в switch");
        }
    }
    DeleteAll(&pets);
    puts("Программа завершена.");

    return 0;
}

char menu(void)
{
    int ch;

    puts("Программа членства в клубе Nerfville Pet Club");
    puts("Введите букву, соответствующую вашему выбору:");
    puts("a) добавление животного l) вывод списка животных");
    puts("n) количество животных  f) поиск животных");
    puts("d) удаление животного   q) выход");
    while((ch = getchar()) != EOF)
    {
        while(getchar() != '\n')
          continue;
        ch = tolower(ch);
        if(strchr("alrfndq", ch) == NULL)
          puts("Введите букву a, l, f, n, d или q:");
        else
          break;
    }
    if(ch == EOF)
      ch = 'q';

    return ch;
}
void addpet(Tree * pt)
{
    Item temp;

    if(TreeIsFull(pt))
      puts("В клубе больше нет мест!");
    else
    {
        puts("Введите кличку животного:");
        s_gets(temp.petName, SLEN);

        temp.list = (Kind *)malloc(sizeof(Kind));
        if(temp.list == NULL)
        {
            fprintf(stderr, "Ошибка выделения памяти.\n");
            exit(1);
        }
        puts("Введите название животного:");
        s_gets(temp.list->petKind, SLEN);

        uppercase(temp.petName);
        uppercase(temp.list->petKind);

        InitializeList(&temp);

        AddItem(&temp, pt);
    }
}
void showpets(const Tree * pt)
{
    if(TreeIsEmpty(pt))
      puts("Записи отсутствуют!");
    else
      Traverse(pt, printitem);
}
void printitem(Item item)
{
    Kind * buf = item.list;

    printf("С кличкой: %-19s\n", item.petName);
    printf("Есть:\n");
    while(buf->next != NULL)
    {
        printf("%s\n", buf->petKind);
        buf = buf->next;
    }
    printf("%s\n", buf->petKind);
}
void findpet(const Tree * pt)
{
    Item temp;

    if(TreeIsEmpty(pt))
    {
        puts("Запись отсутствует!");
        return;
    }

    puts("Введите кличку животного, которое хотите найти:");
    s_gets(temp.petName, SLEN);
    temp.list = (Kind *)malloc(sizeof(Kind));
    if(temp.list == NULL)
    {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(1);
    }
    puts("Введите вид животного:");
    s_gets(temp.list->petKind, SLEN);
    uppercase(temp.petName);
    uppercase(temp.list->petKind);
    printf("%s по имени %s ", temp.list->petKind, temp.petName);

    if(InTree(&temp, pt))
      printf("Является членом клуба.\n");
    else
      printf("не ялвяется членом клуба.\n");
    free(temp.list);
}
void droppet(Tree * pt)
{
    Item temp;

    if(TreeIsEmpty(pt))
    {
        puts("Записи отсутсвуют!");
        return;
    }

    puts("Введите кличку животных, которых нужно исключить из клуба:");
    s_gets(temp.petName, SLEN);
    uppercase(temp.petName);

    printf("%s ", temp.petName);

    if(DeleteItem(&temp, pt))
      printf("Исключен(а) из клуба.\n");
    else
      printf("не является членом клуба.\n");
}
void uppercase(char * str)
{
    while(*str)
    {
        *str = toupper(*str);
        str++;
    }
}
char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);

    if(ret_val)
    {
        find = strchr(st, '\n');
        if(find)
          *find = '\0';
        else
          while(getchar() != '\n')
            continue;
    }

    return ret_val;
}
