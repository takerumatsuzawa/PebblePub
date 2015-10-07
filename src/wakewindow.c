#include <pebble.h>
#include "wakewindow.h"
static Window *wakewindow;
static TextLayer *waketext,*waketime,*nowtime,*grats;

 // #define  NUM_BUF_PKEY 1
  //#define NUM_BUFWAKE_DEFAULT 0
 
  //static int wk=NUM_BUFWAKE_DEFAULT;
bool woke=false;

//ストレージ内のlvを扱う
#define  NUM_LEVEL_PKEY 0
#define NUM_LEVEL_DEFAULT 0
//static int lv = NUM_LEVEL_DEFAULT;
static int lv = NUM_LEVEL_PKEY;

//ストレージ内のbedtimeを扱う
#define  NUM_BEDTIME_PKEY 1
#define NUM_BEDTIME_DEFAULT 0
static int bedtime = NUM_BEDTIME_DEFAULT;

 
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
    
  //ここで起きるべき時刻にメッセージを表示　数字を変えれば表示される時刻が変更される
  //デバッグ用
    if(tick_time->tm_hour==20){
      text_layer_set_text(nowtime,"Wake up!");
      woke=true;
    }else{
      text_layer_set_text(nowtime,buffer);
      woke=false;
    }
  //デバッグ用ここまで
  
  //bedtimeで決めた時間によって起きる時間を決める
//  switch(bedtime){
//   case 0:
//   if(tick_time->tm_hour==4){}
//     text_layer_set_text(waketime,"wake at 04:00");
//     woke=true;
//     break;
//     case 1:
//   if((tick_time->tm_hour==4&&tick_time->tm_min>30) ||tick_time->tm_hour==5&&tick_time->tm_min<30){}
//     text_layer_set_text(waketime,"wake at 04:30");
//     woke=true;
//     break;
//     case 2:
//   if(tick_time->tm_hour==5){}
//     text_layer_set_text(waketime,"wake at 05:00");
//     woke=true;
//     break;
//     case 3:
//   if((tick_time->tm_hour==5&&tick_time->tm_min>30) ||tick_time->tm_hour==6&&tick_time->tm_min<30){}
//     text_layer_set_text(waketime,"wake at 05:30");
//     woke=true;
//     break;
//     case 4:
//   if(tick_time->tm_hour==6){}
//     text_layer_set_text(waketime,"wake at 06:00");
//     woke=true;
//     break;
//     case 5:
//   if((tick_time->tm_hour==6&&tick_time->tm_min>30) ||tick_time->tm_hour==7&&tick_time->tm_min<30){}
//     text_layer_set_text(waketime,"wake at 06:30");
//     woke=true;
//     break;
//     case 6:
//   if(tick_time->tm_hour==7){}
//     text_layer_set_text(waketime,"wake at 07:00");
//     woke=true;
//     break;
//     case 7:
//   if((tick_time->tm_hour==7&&tick_time->tm_min>30) ||tick_time->tm_hour==8&&tick_time->tm_min<30){}
//     text_layer_set_text(waketime,"wake at 07:30");
//     woke=true;
//     break;
//       case 8:
//   if(tick_time->tm_hour==8){}
//     text_layer_set_text(waketime,"wake at 08:00");
//     woke=true;
//     break;
//       case 9:
//   if((tick_time->tm_hour==8&&tick_time->tm_min>30) ||tick_time->tm_hour==9&&tick_time->tm_min<30){}
//     text_layer_set_text(waketime,"wake at 08:30");
//     woke=true;
//     break;
//       case 10:
//   if(tick_time->tm_hour==9){}
//     text_layer_set_text(waketime,"wake at 09:00");
//     woke=true;
//     break;
//     default:
//     woke=false;
//     break;
//    }
  //条件ここまで
 }

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void window_load(){    
  //waketimeを作成
  waketime=text_layer_create(GRect(0, 40, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(waketime,GColorClear);
  text_layer_set_text(waketime,"time");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(waketime,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(waketime,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(wakewindow),text_layer_get_layer(waketime));
  
  //Get the count from persistent storage for use if it exists, otherwise use the default ローカル保存を呼び出し
  //bedtimeを呼び出します
  bedtime = persist_exists(NUM_BEDTIME_PKEY) ? persist_read_int(NUM_BEDTIME_PKEY) : NUM_BEDTIME_DEFAULT;
  //ログにてbedtimeを確認
  APP_LOG(APP_LOG_LEVEL_INFO, "%d",bedtime);
  
  switch(bedtime){
      case 0:
    text_layer_set_text(waketime,"wake at 04:00");
    break;
      case 1:
    text_layer_set_text(waketime,"wake at 04:30");
    break;
      case 2:
    text_layer_set_text(waketime,"wake at 05:00");
    break;
      case 3:
    text_layer_set_text(waketime,"wake at 05:30");
    break;
      case 4:
    text_layer_set_text(waketime,"wake at 06:00");
    break;
      case 5:
    text_layer_set_text(waketime,"wake at 06:30");
    break;
      case 6:
    text_layer_set_text(waketime,"wake at 07:00");
    break;
      case 7:
    text_layer_set_text(waketime,"wake at 07:30");
    break;
      case 8:
    text_layer_set_text(waketime,"wake at 08:00");
    break;
      case 9:
    text_layer_set_text(waketime,"wake at 08:30");
    break;
      case 10:
    text_layer_set_text(waketime,"wake at 09:00");
    break;
    default:
      break;
    }
  
  //nowtimeを作成
  nowtime=text_layer_create(GRect(0, 80, 150, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(nowtime,GColorClear);
  text_layer_set_text(nowtime,"nowtime");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(nowtime,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(nowtime,GTextAlignmentCenter);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(wakewindow),text_layer_get_layer(nowtime));
   // Make sure the time is displayed from the start　開始してから正確な時間を表示する　ｕｐｄａｔｅ＿ｔｉｍｅを呼ぶ
  update_time();
  
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
  //test↓/////////////////////デバッグ用（デモ用）上ボタンを押すだけで、レベルがプラス１されるとする
  //lv=NUM_LEVEL_PKEY;
  //     lv++;
  //     APP_LOG(APP_LOG_LEVEL_INFO, "level= %d",lv);
  //test↑/////////////////////
  //PEBBLEをバイブレーションさせる
  //vibes_long_pulse();
  
  // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
  //   static const uint32_t segments[] ={ 
  //   200 , 100 , 400 , 200,
  //   200 , 100 , 400 , 200,
  //   200 , 100 , 400 , 200};
  //   VibePattern pat = {
  //    . durations = segments ,
  //    . num_segments = ARRAY_LENGTH ( segments ),
  //  };
  //   vibes_enqueue_custom_pattern(pat);
  //バイブレーション　ここまで
  
  }

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {//決定ボタンが押されたら
  APP_LOG(APP_LOG_LEVEL_INFO, "Select pressed!");
  
  //起きるべき時間になった（woke==true）のとき、決定ボタンが押されたら
  if(woke==true){
    //lvを読み込みます
    //lv = persist_exists(NUM_LEVEL_PKEY) ? persist_read_int(NUM_LEVEL_PKEY) : NUM_LEVEL_DEFAULT;
    lv=NUM_LEVEL_PKEY;
    //起きるべき時間に決定ボタンが押されるとCongrats-morning!!が表示される
    text_layer_set_text(grats,"Congrats-morning!!");
    //レベルがひとつ上がる
    lv++;
    APP_LOG(APP_LOG_LEVEL_INFO, "level= %d",lv);
     // Save the count into persistent storage on app exit　ローカル保存
    persist_write_int(NUM_LEVEL_PKEY, lv);
  }
  
  //1007松澤　ここから　新たなウインドウで感覚運動遊びをさせる
  //window_stack_push(s_countdown_window, false);
  //ここまで
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
  text_layer_destroy(waketime); 
  text_layer_destroy(nowtime);
  text_layer_destroy(grats);
  wakewindow=NULL;
  
  //閉じられる時にレベルを保存
  persist_write_int(NUM_LEVEL_PKEY, lv);
  APP_LOG(APP_LOG_LEVEL_INFO, "level= %d",lv);
}

void wake_push() {
  if(!wakewindow) {
    wakewindow = window_create();

    window_set_window_handlers(wakewindow, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    //ログにてbedtimeを確認
    APP_LOG(APP_LOG_LEVEL_INFO, "bedtime= %d",bedtime);
  }
  
  window_stack_push(wakewindow, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  //ボタンが押されたときのコンフィグハンドラー
  window_set_click_config_provider(wakewindow, click_config_provider);
}
