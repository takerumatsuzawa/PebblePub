#include <pebble.h>
#include "wakewindow.h"
static Window *wakewindow;
static TextLayer *waketext,*waketime,*nowtime,*grats;
  #define  NUM_BUF_PKEY 1
// // // You can define defaults for values in persistent storage
  #define NUM_BUFWAKE_DEFAULT 0
//  int buf =NUM_BUFWAKE_DEFAULT;
    bool woke=false;
#define  NUM_LEVEL_PKEY 1
// // // You can define defaults for values in persistent storage
  #define NUM_LEVEL_DEFAULT 0

 /*void update_waketime(){
    switch(buf){
    case 0:
    text_layer_set_text(waketime,"20:00");
    break;
    case 1:
    text_layer_set_text(timetext,"20:30");
    break;
//     case 2:
//     text_layer_set_text(timetext,"01:00");
//     break;
//     case 3:
//     text_layer_set_text(timetext,"01:30");
//     break;
//     case 4:
//     text_layer_set_text(timetext,"02:00");
//     break;
//     case 5:
//     text_layer_set_text(timetext,"02:30");
//     break;
//     case 6:
//     text_layer_set_text(timetext,"03:00");
//     break;
//     case 7:
//     text_layer_set_text(timetext,"03:30");
//     break;
//     case 8:
//     text_layer_set_text(timetext,"04:00");
//     break;
//     case 9:
//     text_layer_set_text(timetext,"04:30");
//     break;
//     case 10:
//     text_layer_set_text(timetext,"05:00");
//     break;
//     case 11:
//     text_layer_set_text(timetext,"05:30");
//     break;
//     case 12:
//     text_layer_set_text(timetext,"06:00");
//     break;
//     case 13:
//     text_layer_set_text(timetext,"06:30");
//     break;
//     case 14:
//     text_layer_set_text(timetext,"07:00");
//     break;
//     case 15:
//     text_layer_set_text(timetext,"07:30");
//     break;
//     case 16:
//     text_layer_set_text(timetext,"08:00");
//     break;
//     case 17:
//     text_layer_set_text(timetext,"08:30");
//     break;
//     case 18:
//     text_layer_set_text(timetext,"09:00");
//     break;
//     case 19:
//     text_layer_set_text(timetext,"09:30");
//     break;
//     case 20:
//     text_layer_set_text(timetext,"10:00");
//     break;
//     case 21:
//     text_layer_set_text(timetext,"10:30");
//     break;
//     case 22:
//     text_layer_set_text(timetext,"11:00");
//     break;
//     case 23:
//     text_layer_set_text(timetext,"11:30");
//     break;
//     case 24:
//     text_layer_set_text(timetext,"12:00");
//     break;
//     case 25:
//     text_layer_set_text(timetext,"12:30");
//     break;
//     case 26:
//     text_layer_set_text(timetext,"13:00");
//     break;
//     case 27:
//     text_layer_set_text(timetext,"13:30");
//     break;
//     case 28:
//     text_layer_set_text(timetext,"14:00");
//     break;
//     case 29:
//     text_layer_set_text(timetext,"14:30");
//     break;
//     case 30:
//     text_layer_set_text(timetext,"15:00");
//     break;
//     case 31:
//     text_layer_set_text(timetext,"15:30");
//     break;
//     case 32:
//     text_layer_set_text(timetext,"16:00");
//     break;
//     case 33:
//     text_layer_set_text(timetext,"16:30");
//     break;
//     case 34:
//     text_layer_set_text(timetext,"17:00");
//     break;
//     case 35:
//     text_layer_set_text(timetext,"17:30");
//     break;
//     case 36:
//     text_layer_set_text(timetext,"18:00");
//     break;
//     case 37:
//     text_layer_set_text(timetext,"18:30");
//     break;
//     case 38:
//     text_layer_set_text(timetext,"19:00");
//     break;
//     case 39:
//     text_layer_set_text(timetext,"19:30");
//     break;
//     case 40:
//     text_layer_set_text(timetext,"20:00");
//     break;
//     case 41:
//     text_layer_set_text(timetext,"20:30");
//     break;
//     case 42:
//     text_layer_set_text(timetext,"21:00");
//     break;
//     case 43:
//     text_layer_set_text(timetext,"21:30");
//     break;
//     case 44:
//     text_layer_set_text(timetext,"22:00");
//     break;
//     case 45:
//     text_layer_set_text(timetext,"22:30");
//     break;
//     case 46:
//     text_layer_set_text(timetext,"23:00");
//     break;
//     case 47:
//     text_layer_set_text(timetext,"23:30");
//     break;
    default:
    text_layer_set_text(waketime,"00:00");
    break;
  }
    
}*/
    
static void update_time(){
//  get a tm structure　時計の構造体を取得
  time_t temp=time(NULL);
  struct tm *tick_time=localtime(&temp);
  
  //Create a long-lived buffer　
  static char buffer[]="00:00";//一時的に０：００(バッファーというものにいれておく)
  //Write the current hours and minutes into the buffer　現在時刻をバッファーに書き込む
  if(clock_is_24h_style()==true){
    //Use 24hour format　24時間計算ならこちら
    strftime(buffer ,sizeof("00:00:00"), "%H:%M:%S", tick_time);
  }else{
    //Use 12hour format　12時間計算ならこちら
    strftime(buffer ,sizeof("00:00:00"), "%I:%M:%S", tick_time);
  }
  //display this time on the TextLayer　このテキストをバッファーを読み込んでディスプレイに表示する
    
    if(tick_time->tm_hour==7){
      text_layer_set_text(nowtime,"I woke!");
      woke=true;
    }else{
      text_layer_set_text(nowtime,buffer);
      woke=false;
    }
    
    //APP_LOG(APP_LOG_LEVEL_INFO, "%d",tick_time->tm_hour);
} 
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void window_load(){
   //waketextを作成
  waketext=text_layer_create(GRect(0, 0, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(waketext,GColorClear);
  text_layer_set_text(waketext,"Let's wake up at");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(waketext,fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(waketext,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(wakewindow),text_layer_get_layer(waketext));
  
  //Get the count from persistent storage for use if it exists, otherwise use the default ローカル保存を呼び出し
   int buf = persist_exists(NUM_BUF_PKEY) ? persist_read_int(NUM_BUF_PKEY) : NUM_BUFWAKE_DEFAULT;
   APP_LOG(APP_LOG_LEVEL_INFO, "%d",buf);
  
  //waketimeを作成
  waketime=text_layer_create(GRect(0, 40, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(waketime,GColorClear);
  text_layer_set_text(waketime,"time");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(waketime,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(waketime,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(wakewindow),text_layer_get_layer(waketime));
  
  //update_waketime();
    /*switch(buf){
    case 0:
    text_layer_set_text(waketime,"wake at 07:00");
    break;
    case 1:
    text_layer_set_text(waketime,"wake at 07:30");
    break;
      case 2:
    text_layer_set_text(waketime,"wake at 08:00");
    break;
      case 3:
    text_layer_set_text(waketime,"wake at 08:30");
    break;
      case 4:
    text_layer_set_text(waketime,"wake at 09:00");
    break;
      case 5:
    text_layer_set_text(waketime,"wake at 09:30");
    break;
      case 6:
    text_layer_set_text(waketime,"wake at 10:00");
    break;
      case 7:
    text_layer_set_text(waketime,"wake at 10:30");
    break;
      case 8:
    text_layer_set_text(waketime,"wake at 11:00");
    break;
      case 9:
    text_layer_set_text(waketime,"wake at 11:30");
    break;
      case 10:
    text_layer_set_text(waketime,"wake at 12:00");
    break;
      case 11:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 12:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 13:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 14:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 15:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 16:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 17:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 18:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 19:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 20:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 21:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 22:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 23:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 24:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      case 25:
    text_layer_set_text(waketime,"wake at 12:30");
    break;
      
    default:
      break;
    }*/
  text_layer_set_text(waketime,"07:00");
  //nowtimeを作成
  nowtime=text_layer_create(GRect(0, 80, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(nowtime,GColorClear);
  text_layer_set_text(nowtime,"wake?");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(nowtime,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(nowtime,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(wakewindow),text_layer_get_layer(nowtime));
   // Make sure the time is displayed from the start　開始してから正確な時間を表示する　ｕｐｄａｔｅ＿ｔｉｍｅを呼ぶ
  update_time();
  
  //switch(buf)
  
  //gratsを作成
  grats=text_layer_create(GRect(0, 120, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(grats,GColorClear);
  text_layer_set_text(grats,"-.- zzz");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(grats,fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(grats,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(wakewindow),text_layer_get_layer(grats));

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {// 上ボタンが押されたら
  APP_LOG(APP_LOG_LEVEL_INFO, "Up pressed!");
//  update_level();
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {//決定ボタンが押されたら
  APP_LOG(APP_LOG_LEVEL_INFO, "Select pressed!");
  // Get the count from persistent storage for use if it exists, otherwise use the default ローカル保存を呼び出し
  int lv = persist_exists(NUM_LEVEL_PKEY) ? persist_read_int(NUM_LEVEL_PKEY) : NUM_LEVEL_DEFAULT;
  if(woke==true){
    text_layer_set_text(grats,"Congrats-morning!!");
    lv++;
     // Save the count into persistent storage on app exit　ここで　ローカル保存
  persist_write_int(NUM_LEVEL_PKEY, lv);
  }
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {//下ボタンがおされたら
  APP_LOG(APP_LOG_LEVEL_INFO, "Down pressed!");
}
static void click_config_provider(void *context) {//ボタンを押されたときのプロバイダー
  // Register the ClickHandlers　クリックハンドラーを登録
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void window_unload(){
  text_layer_destroy(waketext); text_layer_destroy(waketime); text_layer_destroy(nowtime);
 
}

void wake_push() {
  if(!wakewindow) {
    wakewindow = window_create();
//    window_set_background_color(bed_window, GColorJaegerGreen);
    window_set_window_handlers(wakewindow, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(wakewindow, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  //ボタンが押されたときのコンフィグハンドラー
  window_set_click_config_provider(wakewindow, click_config_provider);
}