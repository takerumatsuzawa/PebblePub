#include <pebble.h>
#include "bedwindow.h"
  #include"wakewindow.h"
#include <stdio.h>
  #include <stdlib.h>
  
static Window *bed_window;//*wakewindow;
static TextLayer *whattext1,*whattext2,*timetext,*selecttext;
int hours=0,mins=0;
//int buf=1;
 #define  NUM_BUF_PKEY 1
// // You can define defaults for values in persistent storage
 #define NUM_BUF_DEFAULT 0
 int buf =NUM_BUF_DEFAULT;

void update_bedtime(){
  switch(buf){
    case 0:
    text_layer_set_text(timetext,"00:00");//成功
    break;
    case 1:
    text_layer_set_text(timetext,"00:30");
    break;
    case 2:
    text_layer_set_text(timetext,"01:00");
    break;
    case 3:
    text_layer_set_text(timetext,"01:30");
    break;
    case 4:
    text_layer_set_text(timetext,"02:00");
    break;
    case 5:
    text_layer_set_text(timetext,"02:30");
    break;
    case 6:
    text_layer_set_text(timetext,"03:00");
    break;
    case 7:
    text_layer_set_text(timetext,"03:30");
    break;
    case 8:
    text_layer_set_text(timetext,"04:00");
    break;
    case 9:
    text_layer_set_text(timetext,"04:30");
    break;
    case 10:
    text_layer_set_text(timetext,"05:00");
    break;
    case 11:
    text_layer_set_text(timetext,"05:30");
    break;
    case 12:
    text_layer_set_text(timetext,"06:00");
    break;
    case 13:
    text_layer_set_text(timetext,"06:30");
    break;
    case 14:
    text_layer_set_text(timetext,"07:00");
    break;
    case 15:
    text_layer_set_text(timetext,"07:30");
    break;
    case 16:
    text_layer_set_text(timetext,"08:00");
    break;
    case 17:
    text_layer_set_text(timetext,"08:30");
    break;
    case 18:
    text_layer_set_text(timetext,"09:00");
    break;
    case 19:
    text_layer_set_text(timetext,"09:30");
    break;
    case 20:
    text_layer_set_text(timetext,"10:00");
    break;
    case 21:
    text_layer_set_text(timetext,"10:30");
    break;
    case 22:
    text_layer_set_text(timetext,"11:00");
    break;
    case 23:
    text_layer_set_text(timetext,"11:30");
    break;
    case 24:
    text_layer_set_text(timetext,"12:00");
    break;
    case 25:
    text_layer_set_text(timetext,"12:30");
    break;
    case 26:
    text_layer_set_text(timetext,"13:00");
    break;
    case 27:
    text_layer_set_text(timetext,"13:30");
    break;
    case 28:
    text_layer_set_text(timetext,"14:00");
    break;
    case 29:
    text_layer_set_text(timetext,"14:30");
    break;
    case 30:
    text_layer_set_text(timetext,"15:00");
    break;
    case 31:
    text_layer_set_text(timetext,"15:30");
    break;
    case 32:
    text_layer_set_text(timetext,"16:00");
    break;
    case 33:
    text_layer_set_text(timetext,"16:30");
    break;
    case 34:
    text_layer_set_text(timetext,"17:00");
    break;
    case 35:
    text_layer_set_text(timetext,"17:30");
    break;
    case 36:
    text_layer_set_text(timetext,"18:00");
    break;
    case 37:
    text_layer_set_text(timetext,"18:30");
    break;
    case 38:
    text_layer_set_text(timetext,"19:00");
    break;
    case 39:
    text_layer_set_text(timetext,"19:30");
    break;
    case 40:
    text_layer_set_text(timetext,"20:00");
    break;
    case 41:
    text_layer_set_text(timetext,"20:30");
    break;
    case 42:
    text_layer_set_text(timetext,"21:00");
    break;
    case 43:
    text_layer_set_text(timetext,"21:30");
    break;
    case 44:
    text_layer_set_text(timetext,"22:00");
    break;
    case 45:
    text_layer_set_text(timetext,"22:30");
    break;
    case 46:
    text_layer_set_text(timetext,"23:00");
    break;
    case 47:
    text_layer_set_text(timetext,"23:30");
    break;
//     case 48:
//     text_layer_set_text(timetext,"13:00");
//     break;
    default:
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 上ボタンが押されたら
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  buf++;
  APP_LOG(APP_LOG_LEVEL_INFO, "Up pressed!");
  if(mins==0){
  mins=mins+30;
  }else{
  mins=0;
    hours++;
    
    if(hours>23){
      hours=0;
    }
  }
  if(buf>47){
    buf=0;
  }
  
  //APP_LOG(APP_LOG_LEVEL_INFO, "%d:%d",hours,mins);
  APP_LOG(APP_LOG_LEVEL_INFO, "%d",buf);
  update_bedtime();
  text_layer_set_text(selecttext,"Select to save");
}
//決定ボタンが押されたら
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Select pressed!");
     // Save the count into persistent storage on app exit　ここで　ローカル保存
  persist_write_int(NUM_BUF_PKEY, buf);
  APP_LOG(APP_LOG_LEVEL_INFO, "save buf %d",buf);
//   wakewindow=window_create();
  text_layer_set_text(selecttext,"your Bedtime was saved");//成功
}
//下ボタンがおされたら
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Down pressed!");
  //char buffer[]="00:00";//一時的に０：００(バッファーというものにいれておく)
  buf--;
    if(mins==30){
  mins=0;
  }else{
  mins=30;
      hours--;
    if(hours<0){
      hours=23;
    }
  }
  if(buf<0){
    buf=47;
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "%d:%d",hours,mins);
  APP_LOG(APP_LOG_LEVEL_INFO, "%d",buf);
  update_bedtime();
  text_layer_set_text(selecttext,"Select to save");
}


static void click_config_provider(void *context) {//ボタンを押されたときのプロバイダー
  // Register the ClickHandlers　クリックハンドラーを登録
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void window_load(){
  // Get the count from persistent storage for use if it exists, otherwise use the default ローカル保存を呼び出し
  buf = persist_exists(NUM_BUF_PKEY) ? persist_read_int(NUM_BUF_PKEY) : NUM_BUF_DEFAULT;
   //whattext1を作成
  whattext1=text_layer_create(GRect(0, 0, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(whattext1,GColorClear);
  text_layer_set_text(whattext1,"What time is");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(whattext1,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(whattext1,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(bed_window),text_layer_get_layer(whattext1));
  
  //whattext2を作成
  whattext2=text_layer_create(GRect(0, 28, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(whattext2,GColorClear);
  text_layer_set_text(whattext2,"your Bedtime?");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(whattext2,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(whattext2,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(bed_window),text_layer_get_layer(whattext2));
  
  
  //timetextを作成
  timetext=text_layer_create(GRect(0, 56, 100, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(timetext,GColorClear);
  text_layer_set_text(timetext,"00:00");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(timetext,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(timetext,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(bed_window),text_layer_get_layer(timetext));
  update_bedtime();
  
  //selecttextを作成
  selecttext=text_layer_create(GRect(0, 88, 120, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(selecttext,GColorClear);
  text_layer_set_text(selecttext,"Select to save");
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(timetext,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(selecttext,GTextAlignmentCenter);
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(bed_window),text_layer_get_layer(selecttext));
   // Make sure the time is displayed from the start　開始してから正確な時間を表示する　ｕｐｄａｔｅ＿ｔｉｍｅを呼ぶ
  //update_time();
}
static void window_unload(){
  text_layer_destroy(whattext1); text_layer_destroy(whattext2); text_layer_destroy(timetext);text_layer_destroy(selecttext); 
}

void bedwindow_push() {
  if(!bed_window) {
    bed_window = window_create();
    window_set_window_handlers(bed_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(bed_window, true);
  //ボタンが押されたときのコンフィグハンドラー
  window_set_click_config_provider(bed_window, click_config_provider);
}




