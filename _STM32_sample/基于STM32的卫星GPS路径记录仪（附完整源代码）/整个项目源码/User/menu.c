/*-------------------------------------------------------
      �˵����� By wowbanui

 �汾��ʷ:
 Rev.201006     ��ʼ�汾

---------------------------------------------------------*/


#include "common.h"
#include "menu.h"
#include "keyboard.h"
#include "S1D15E06.h"
#include "W25Q16.h"
#include "logger.h"


// ϵͳ�Ӳ˵�
struct stru_MENU_ITEM SysMenuItems[]={
    //Index,isChecked,  isBar,  *String,        (*Func)(),      *ChildMenu
    { 1,    0,          0,      "�����ֿ�16",   NullFunc,       0},
    { 2,    0,          0,      "�����ֿ�12",   NullFunc,       0},
    { 3,    0,          0,      "   ....   ",   NullFunc,       0},
    { 4,    0,          0,      "   ....   ",   NullFunc,       0},
    { 5,    0,          0,      "   ....   ",   NullFunc,       0},
    { 6,    0,          0,      "   ....   ",   NullFunc,          0}
};
struct stru_MENU SysMenu={
    62,1,NULL,1,6,6,SysMenuItems
};

// ��¼����Ӳ˵�
struct stru_MENU_ITEM LogTimeMenuItems[]={
    //Index,isChecked,  isBar,  *String,        (*Func)(),      *ChildMenu
    { 1,    0,          0,      "   1 ��   ",   NullFunc,       0},
    { 2,    0,          0,      "   2 ��   ",   NullFunc,       0},
    { 3,    0,          0,      "   3 ��   ",   NullFunc,       0},
    { 4,    0,          0,      "   4 ��   ",   NullFunc,       0},
    { 5,    0,          0,      "   5 ��   ",   NullFunc,       0},
    { 6,    0,          0,      "   20 ��  ",   About,          0}
};
struct stru_MENU LogTimeMenu={
    62,4,NULL,2,6,6,LogTimeMenuItems
};

// �ϴ���¼�Ӳ˵�
struct stru_MENU_ITEM UploadMenuItems[]={
    //Index,isChecked,  isBar,  *String,        (*Func)(),      *ChildMenu
    { 1,    0,          0,      "TF�� (kml)",    LOG_KMLToTF,  0},
    { 2,    0,          0,      "TF��(ת��)",    LOG_DumpFlashToTF,      0},    // Logger.c
    { 0,    0,          1,      "-",             NullFunc,      0},
    { 3,    0,          0,      "���� (bin)",    NullFunc,      0},
    { 4,    0,          0,      "���� (kml)",    NullFunc,      0},
};
struct stru_MENU UploadMenu={
    62,8,NULL,3,4,5,UploadMenuItems
};

// ���˵�
struct stru_MENU_ITEM MainMenuItems[]={
    //Index,isChecked,  isBar,  *String,        (*Func)(),      *ChildMenu
    { 1,    0,          0," ϵ  ͳ  \x02", NullFunc,            &SysMenu},
    { 2,    0,          0,"��¼��� \x02", NullFunc,            &LogTimeMenu},
    { 0,    0,          1,"-",             NullFunc,            0},
    { 3,    0,          0,"ת����¼ \x02", NullFunc,            &UploadMenu},
    { 4,    0,          0," �����¼ ",    BrowseRecord,        0},     // �� Logger.c ��
    { 5,    0,          0," �ռ���� ",    MenuFuncDeleteAll,   0},
    { 0,    0,          1,"-",             NullFunc,            0},
    { 6,    0,          0," ���� ... ",    About,               0}
};
struct stru_MENU MainMenu={
    0,0,NULL,NULL,6,8,MainMenuItems
};

//------------------
struct stru_MENU *pMenu=&MainMenu;

static u8 MenuIndex;

void NullFunc(void){}


void Menu_Init(void)
{
    SysMenu.ParentMenu=&MainMenu;
    UploadMenu.ParentMenu=&MainMenu;
    LogTimeMenu.ParentMenu=&MainMenu;
}

void ShowMenu(struct stru_MENU* pMenu)
{
    u8 i;
    u8 MenuPosX=pMenu->X,MenuPosY=pMenu->Y;
    u8 MenuCount=pMenu->MenuTotalCount;

    LCD_GUI_Rect(MenuPosX,
                 MenuPosY,
                 MenuPosX+66,
                 MenuPosY+(pMenu->MenuItemsCount)*2+(pMenu->MenuTotalCount)+1,
                 0);
    MenuPosY++;

    for (i=0;i<MenuCount;i++)
    {
        if (pMenu->MenuItem[i].isBar)
        {
            LCD_GUI_HR(MenuPosX+3,MenuPosX+63,MenuPosY);
            MenuPosY++;
        }
        else
        {
            if ((pMenu->MenuItem[i].Index)==MenuIndex)
                LCD_SetGray(GRAY_WHITE,GRAY_BLACK);
            else
                LCD_SetGray(GRAY_BLACK,GRAY_GW);
            LCD_PutStr12(MenuPosX+3,MenuPosY,pMenu->MenuItem[i].MenuString);
            MenuPosY+=3;
        }
    }
}

u8 GetMenuNum(struct stru_MENU* pMenu)
{
    u8 i; 
    // u8 j=pMenu-> MenuTotalCount;
    for (i=0;i<7;i++)
    {
        if (MenuIndex==pMenu->MenuItem[i].Index)
            return i;   
    }
    return 0;
}

void Menu(void)
{   
//    u8 i,
    u8 GetMenuNumber;
    MenuIndex=1;

    Menu_Init(); 
           
    LCD_GUI_BG1();
    pMenu=&MainMenu;
    ShowMenu(pMenu);

        while(1)
        {
            switch(KBD_GetKey())
            {
                case KEY_MENU:
                    if (pMenu->ParentMenu==NULL)
                        return;
                    else
                    {
                        MenuIndex=pMenu->ParentMenuIndex;
                        pMenu= pMenu->ParentMenu;
                        LCD_GUI_BG1();
                        ShowMenu(pMenu);  
                    }
                    break;
                case KEY_DOWN:
                    MenuIndex++;
                    if (MenuIndex>(pMenu->MenuItemsCount)) MenuIndex=1;
                    ShowMenu(pMenu);
                    break;
                case KEY_UP:
                    MenuIndex--;
                    if (MenuIndex<1) MenuIndex=pMenu->MenuItemsCount;
                    ShowMenu(pMenu);
                    break;
                case KEY_ENTER:
                    GetMenuNumber=GetMenuNum(pMenu);
                    if(pMenu->MenuItem[GetMenuNumber].ChildMenu)
                    {
                        pMenu=pMenu->MenuItem[GetMenuNumber].ChildMenu;
                        MenuIndex=1;
                        ShowMenu(pMenu);
                    }
                    else
                    {
                        (*pMenu->MenuItem[GetMenuNumber].SubFunc)();
                        return;
                    }
                    break;
            }
        }
}



void About(void)
{      
//    u8 i;
            
    LCD_GUI_Rect(5,1,155,23,0);
    LCD_SetGray(GRAY_BLACK,GRAY_GW); 
    LCD_PutStr12(20,4,"GPS ��¼�� �ڶ���");
    LCD_PutStr12(20,8,"wowbanui 201108");
    LCD_PutStr12(20,12,"Rev. 0.5.110811");

    LCD_GUI_Button(120,18,"ȷ��",0);

    while(1)
    {
        if(KBD_GetKey()==KEY_ENTER) return; 
    }
}

void MenuFuncDeleteAll(void)
{
    u8 i;
            
    LCD_GUI_Rect(20,2,158,15,0);
    LCD_SetGray(GRAY_BLACK,GRAY_GW); 
    LCD_PutStr12(30,4,"ȷ��Ҫɾ����������?");
    LCD_GUI_Button(75,9," �� ",0);
    LCD_GUI_Button(120,9," �� ",0);

    while(1)
    {
        switch(KBD_GetKey())
        {
        case KEY_ENTER:
            return;

        case KEY_UP:
            for (i=0;i<25;i++)
            {
                W25Q_EraseBlock(BLOCK_EARSE_START_ADDR+i*64*1024);
            }
            LOG_MountFlash(); //����ɨ��Flash
            return;
        }    
    }
}

