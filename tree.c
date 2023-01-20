//tree.c - функция поддержки дерева
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//Локальный тип данных
typedef struct pair {
    Trnode * parent;
    Trnode * child;
} Pair;

//Прототипы локльных функций
static Trnode * MakeNode(const Item * pi);
static bool ToLeft(const Item * i1, const Item * i2);
static bool ToRight(const Item * i1, const Item * i2);
static void AddNode(Trnode * new_node, Trnode * root);
static void InOrder(const Trnode * root, void (* pfun)(Item item));
static Pair SeekItem(const Item * pi, const Tree * ptree);
static bool SeekList(Kind * pk, const Kind * pi);
static bool AddList(Kind * tr, Kind * pi);
static void DeleteNode(Trnode **ptr);
static void DeleteAllNodes(Trnode * ptr);

//Определение функций
void InitializeTree(Tree * ptree)
{
    ptree->root = NULL;
    ptree->size = 0;
}
void InitializeList(Item * pi)
{
    pi->list->next = NULL;
}
bool TreeIsEmpty(const Tree * ptree)
{
    if(ptree->root == NULL)
      return true;
    else
      return false;
}
bool TreeIsFull(const Tree * ptree)
{
    if(ptree->size == MAXITEMS)
      return true;
    else
      return false;
}
int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}
bool AddItem(const Item * pi, Tree * ptree)
{
    Trnode * new_node;

    if(TreeIsFull(ptree))
    {
        fprintf(stderr, "Дерево переполненно\n");
        return false;    //преждевременный возврат
    }
    if(SeekItem(pi, ptree).child != NULL)//Тут надо будет переделать
    {
        if(SeekList(SeekItem(pi, ptree).child->item.list, pi->list))
        {
            fprintf(stderr, "Попытка добавления дублированного элемента\n");
            return false;    //преждевременный возврат
        }
        else
        {
            if(AddList(SeekItem(pi, ptree).child->item.list, pi->list))
              return true;
            else
            {
                fprintf(stderr, "Ошибка записи нового вида животного.");
                exit(1);
            }
        }
    }
    new_node = MakeNode(pi);    //указывает на новый узел
    if(new_node == NULL)
    {
        fprintf(stderr, "Не удалось создать узел\n");
        return false;
    }
    //Успешное создание нового узла
    ptree->size++;

    if(ptree->root == NULL)             //Случай 1: дерево пустое
      ptree->root = new_node;           //Новый узел - корень дерева
    else                                //Случай 2: дерево не пустое
      AddNode(new_node, ptree->root);   //Добавление узла к дереву

    return true;                        //Возврат в случае успеха
}
bool InTree(const Item * pi, const Tree * ptree)
{
    if(SeekItem(pi, ptree).child == NULL)
      return false;
    else if(SeekList(SeekItem(pi, ptree).child->item.list, pi->list))
      return true;
    else
      return false;
}
bool DeleteItem(const Item * pi, Tree * ptree)
{
    Pair look;

    look = SeekItem(pi, ptree);

    if(look.child == NULL)
      return false;
    if(look.parent == NULL)
      DeleteNode(&ptree->root);
    else if(look.parent->left == look.child)
      DeleteNode(&look.parent->left);
    else
      DeleteNode(&look.parent->right);
    ptree->size--;

    return true;
}
void Traverse(const Tree * ptree, void (* pfun)(Item item))
{
    if(ptree != NULL)
      InOrder(ptree->root, pfun);
}
void DeleteAll(Tree * ptree)
{
    if(ptree != NULL)
      DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}

//Локальные функции
static void InOrder(const Trnode * root, void (* pfun)(Item item))
{
    if(root != NULL)
    {
        InOrder(root->left, pfun);
        (*pfun)(root->item);
        InOrder(root->right, pfun);
    }
}
static void DeleteAllNodes(Trnode * root)
{
    Trnode * pright;

    if(root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}
static void AddNode(Trnode * new_node, Trnode * root)
{
    if(ToLeft(&new_node->item, &root->item))
    {
        if(root->left == NULL)              //Пустое поддерево
          root->left = new_node;            //Поэтому добавить сюда узел
        else
          AddNode(new_node, root->left);    //Иначе обработать поддерево
    }
    else if(ToRight(&new_node->item, &root->item))
    {
        if(root->right == NULL)
          root->right = new_node;
        else
          AddNode(new_node, root->right);
    }
    else
    {
        fprintf(stderr, "Ошибка местоположения в AddNode()\n");
        exit(1);
    }
}
static bool ToLeft(const Item * i1, const Item * i2)
{
    int comp1;

    if((comp1 = strcmp(i1->petName, i2->petName)) < 0)
      return true;
    else
      return false;
}
static bool ToRight(const Item * i1, const Item * i2)
{
    int comp1;

    if((comp1 = strcmp(i1->petName, i2->petName)) > 0)
      return true;
    else
      return false;
}
static Trnode * MakeNode(const Item * pi)
{
    Trnode * new_node;

    new_node = (Trnode *) malloc(sizeof(Trnode));
    if(new_node != NULL)
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}
static Pair SeekItem(const Item * pi, const Tree * ptree)
{
    Pair look;

    look.parent = NULL;
    look.child = ptree->root;

    if(look.child == NULL)
      return look;

    while(look.child != NULL)
    {
        if(ToLeft(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if(ToRight(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
          break;
    }
    return look;
}
static bool SeekList(Kind * pk, const Kind * pi)
{
    List head = pk;

    while(head->next != NULL)
    {
        if(strcmp(head->petKind, pi->petKind) == 0)
            return true;
        else
          head = head->next;
    }

    return false;
}
static bool AddList(Kind * tr, Kind * pi)
{
    List head = tr;

    while(head->next != NULL)
    {
        head = head->next;
    }
    if(head->next = pi)
      return true;
}
static void DeleteNode(Trnode **ptr)
{
    Trnode * temp;

    if((*ptr)->left == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if((*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else
    {
        for(temp = (*ptr)->left; temp->right != NULL; temp = temp->right)
          continue;
        temp->right = (*ptr)->right;
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
}