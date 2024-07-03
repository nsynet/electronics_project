#ifndef __MENU_H
#define __MENU_H

#include "common.h"

//菜单条目结构体
struct stru_MENU_ITEM
{
    u8 Index;       //菜单索引, 由于菜单中有横隔,所以与数组号不相同
    u8 isChecked;   //菜单前是否有选中符号
    u8 isBar;       //该条菜单是否是横隔
    char * MenuString;  //菜单显示文字
    void (*SubFunc)();  //选中菜单时执行的函数
    struct stru_MENU *ChildMenu;  //子菜单
};

//菜单结构体
struct stru_MENU
{
    u8 X;           //菜单显示的坐标X 
    u8 Y;           //菜单显示的坐标Page 
    struct stru_MENU *ParentMenu;    //父菜单
    u8 ParentMenuIndex;             //父菜单索引号
    u8 MenuItemsCount;              //包含菜单条目数(不包括bar)
    u8 MenuTotalCount;              //包含菜单条目数(包括bar)
    struct stru_MENU_ITEM *MenuItem;    //菜单条目  
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
