#ifndef __GUI_H__
#define __GUI_H__

#include "main.h"
#include "sys.h"

//battery icon size
#define BATTERY_X   (19) //40
#define BATTERY_Y   (8)  //20


//565 RGB 00000 000000 00000 16bit
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0xF800//0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0xF80F//0x4208      	//灰色2  1111111111011111


#if 1//defined(_BC_FES_)
#define CH_A_ON   1
#define CH_A_OFF  0
#define CH_B_ON   1
#define CH_B_OFF  0


typedef enum
{
    CH_FES_START = 0,
		CH_FES_A,
		CH_FES_B,
		CH_FES_OK,
		CH_FES_END
}GUI_FES_CH;

typedef enum
{
    MODE_FES_START = 0,
		MODE_FES_UPPER,
		MODE_FES_LOWER,
		MODE_FES_SWALLOW,
		MODE_FES_CUSTOM,
		MODE_FES_END
}GUI_FES_MODE_SELEC;

typedef enum
{
    MODE_FES_STIMULATE_START = 0,
		MODE_FES_STRENGTH_CHA,
		MODE_FES_STOP_START_CHA,
		MODE_FES_STRENGTH_CHB,
		MODE_FES_STOP_START_CHB,
		MODE_FES_STIMULATE_END
}GUI_FES_STIMULATE_INDEX;


typedef enum
{
    MENU_ID_FES_START = 0,
		MENU_ID_FES_MAIN,
		MENU_ID_FES_MODE_CHA,
		MENU_ID_FES_MODE_COMBINA, //连续动作模式
		MENU_ID_FES_STIMULATE_TIME,
		MENU_ID_FES_STIMULATE_CHA,
		MENU_ID_FES_CUSTOM_CHA,
		MENU_ID_FES_MODE_CHB,
		MENU_ID_FES_MODE_CHA_CHB,
		MENU_ID_FES_STIMULATE_CHB,
		MENU_ID_FES_CUSTOM_CHB,
		MENU_ID_FES_STIMULATE_CHA_CHB,
		MENU_ID_FES_END
}GUI_MENU_FES_INDEX;


//#else

typedef enum
{
    MENU_ID_START = 0,
		MENU_ID_MAIN,
		MENU_ID_STRENGTH,
		MENU_ID_STIMULATE,
		MENU_ID_CUSTOM,
		MENU_ID_END
}GUI_MENU_INDEX;


typedef enum
{
    CUSTOM_ID_START = 0,
		CUSTOM_ID_PLUSE,
		CUSTOM_ID_FREQU,
		CUSTOM_ID_STREN,
		CUSTOM_ID_TOTIM,
		CUSTOM_ID_STATIM,
		CUSTOM_ID_STOTIM,
		CUSTOM_ID_OK,
		CUSTOM_ID_END
}GUI_CUSTOM_INDEX;

typedef enum
{
    MODE_START = 0,
		MODE_A,
		MODE_B,
		MODE_C,
		MODE_CUSTOM,
		MODE_END
}GUI_MODE_SELEC;

#endif

typedef enum
{
		BATTERY_ICON_0,//POWER OFF 
		BATTERY_ICON_1,
		BATTERY_ICON_2,
		BATTERY_ICON_3
}GUI_BATTERY_ICON;

typedef struct
{
    uint8_t menu_id;
		uint8_t menu_mode_index;
		uint8_t current_strength;
		uint8_t time_sec;
		uint8_t time_min;
		uint8_t custom_index;
		uint32_t us_val;
	  uint32_t hz_val;
		uint8_t  ma_val;
		uint16_t start_time;
		uint16_t stop_time;
		uint16_t total_time;
		uint8_t  ok_flag;

    //FES
		uint8_t ch_a_flag;
		uint8_t ch_b_flag;
		uint8_t selec_index;
		uint8_t stimu_time;//min
		uint8_t cha_on_flag;
		uint8_t chb_on_flag;
		uint8_t cha_mode_index;
    uint8_t chb_mode_index;
		uint8_t cha_current_strength;
		uint8_t chb_current_strength;
    uint8_t chab_stimulate_index;
		uint8_t chab_stimu_cha_strength_flag;
		uint8_t chab_stimu_cha_strength_on_flag;
		uint8_t chab_stimu_chb_strength_flag;
		uint8_t chab_stimu_chb_strength_on_flag;
		uint8_t cha_time_sec;
		uint8_t cha_time_min;
		uint8_t chb_time_sec;
		uint8_t chb_time_min;
		
}GUI_PARA;


extern GUI_PARA gui_struct_para;
extern GUI_PARA gui_fes_struct_para;

		
void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0,u16 y0,u16 fc,u8 r);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p); //显示40*40 QQ图片
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void GUI_Display_Clear_No_Icon(void);
void GUI_Display_Ui_Charging_Battery_Icon(void);
void GUI_Display_Ui_Battery_Level_Icon(void);


#if 1//defined(_BC_FES_)
void GUI_FES_DrawImageNum_Adjust(u8 strength,u8 high_light,u8 index_ok);
void GUI_FES_DrawImageNum_Adjust_Ex(u8 time,u8 high_light,u8 index_ok);
void GUI_FES_DrawImageNum_Adjust_Two_Cha(u8 strength,u8 high_light,u8 index_ok);
void GUI_FES_DrawImageNum_Adjust_Two_Chb(u8 strength,u8 high_light,u8 index_ok);
void GUI_FES_Display_Ui_Main_Menu_Ch_Selec(uint8_t ch_selec,uint8_t ch_a_flag,uint8_t ch_b_flag);
void GUI_FES_Display_Ui_Mode_Selec(uint8_t ch_a_flag,uint8_t ch_b_flag,uint8_t mode_index,uint8_t two_ch_flag);
void GUI_FES_Display_Ui_Main_Ch_Mode_Back(uint8_t mode);
void GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(uint8_t chab_stimu_index);
void GUI_FES_Display_Ui_Main_Ch_Stimulate(uint8_t ch_a_flag,uint8_t ch_b_flag,uint8_t a_mode_index,uint8_t b_mode_index,uint8_t chab_stimu_index);
void GUI_FES_Display_Ui_Main_Ch_Stimulate_Switch(uint8_t selec_index);
void GUI_FES_Display_Ui_Mode_Selec_Switch(uint8_t selec_index,uint8_t mode_index);
void GUI_FES_Display_Ui_Stimu_Time(uint8_t selec_index,uint8_t stimu_time);
void GUI_FES_Display_Ui_Battery_Level_Icon(void);
void GUI_FES_Display_Ui_Charging_Battery_Icon(void);
void GUI_FES_Display_Ui_Stimu_Combina_Main(uint8_t is_stop);
void GUI_FES_Display_Ui_Stimu_Combina(uint8_t is_stop);


//#else
void GUI_VNS_Display_Ui_Custom_Pluse_Width_Ex(void);
void GUI_VNS_Display_Ui_Custom_Frequency_Ex(void);
void GUI_VNS_Display_Ui_Custom_Strength_Ex(void);
void GUI_VNS_Display_Ui_Custom_Total_Time_Ex(void);
void GUI_VNS_Display_Ui_Custom_Start_Time_Ex(void);
void GUI_VNS_Display_Ui_Custom_Stop_Time_Ex(void);
void GUI_VNS_Display_Ui_Battery_Level_Icon(void);
void GUI_VNS_Display_Ui_Charging_Battery_Icon(void);


void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color);
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
void GUI_Display_Logo_Picture(void);
void GUI_Display_Test(uint8_t num);
void GUI_Display_Ui_Main_Menu(void);
void GUI_VNS_Display_Ui_Custom_Menu(void);
void GUI_Display_Strength_Adjust_Val(u16 x,u16 y,u8 strength);
void GUI_VNS_DrawImageNum_Test_Adjust(u8 strength,u8 high_light,u8 index_ok);
void GUI_VNS_DrawImageNum_Adjust(u8 strength,u8 high_light,u8 index_ok);
void GUI_VNS_Display_Ui_Adjust_Strength(void);
void GUI_VNS_Display_Ui_Stimulate(void);
void GUI_Display_Ui_Main_Menu_Selec(uint8_t mode);
void GUI_Display_Countdown_Time(u16 x,u16 y,u8 min,u8 sec);
void GUI_Display_Countdown_Time_Ex(u16 x,u16 y,u8 min,u8 sec);
void GUI_VNS_Display_Ui_Custom_Selec(uint8_t custon_index);
void GUI_Display_Ui_Electrode_Connect(void);
#endif
#endif

