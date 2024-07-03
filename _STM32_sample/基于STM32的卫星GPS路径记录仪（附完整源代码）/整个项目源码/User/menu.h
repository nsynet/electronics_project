#ifndef __MENU_H
#define __MENU_H

#include "common.h"

//�˵���Ŀ�ṹ��
struct stru_MENU_ITEM
{
    u8 Index;       //�˵�����, ���ڲ˵����к��,����������Ų���ͬ
    u8 isChecked;   //�˵�ǰ�Ƿ���ѡ�з���
    u8 isBar;       //�����˵��Ƿ��Ǻ��
    char * MenuString;  //�˵���ʾ����
    void (*SubFunc)();  //ѡ�в˵�ʱִ�еĺ���
    struct stru_MENU *ChildMenu;  //�Ӳ˵�
};

//�˵��ṹ��
struct stru_MENU
{
    u8 X;           //�˵���ʾ������X 
    u8 Y;           //�˵���ʾ������Page 
    struct stru_MENU *ParentMenu;    //���˵�
    u8 ParentMenuIndex;             //���˵�������
    u8 MenuItemsCount;              //�����˵���Ŀ��(������bar)
    u8 MenuTotalCount;              //�����˵���Ŀ��(����bar)
    struct stru_MENU_ITEM *MenuItem;    //�˵���Ŀ  
};


void Menu_Init(void);
void ShowMenu(struct stru_MENU* pMenu);
u8   GetMenuNum(struct stru_MENU* pMenu);
void Menu(void);


void NullFunc(void);
void About(void);
extern void BrowseRecord(void);
void MenuFuncDeleteAll(void);
extern void LOG_KMLToTF(void);
extern void LOG_DumpFlashToTF(void);

#endif
