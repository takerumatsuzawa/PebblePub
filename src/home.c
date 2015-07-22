#include"home.h"
#include <pebble.h>
#include "time.h"
static Window *mainwindow;
static TextLayer *timetext,*leveltext,*border;
static BitmapLayer *imagelayer;
static GBitmap *imagebitmap;

#define  NUM_LEVEL_PKEY 1
// You can define defaults for values in persistent storage
#define NUM_LEVEL_DEFAULT 0
static int lv = NUM_LEVEL_DEFAULT;

// static void up_click_handler(ClickRecognizerRef recognizer, void *context) {// 上ボタンが押されたら
//   APP_LOG(APP_LOG_LEVEL_INFO, "Up pressed!");
//   APP_LOG(APP_LOG_LEVEL_INFO, "%d",lv);
// }
// static void select_click_handler(ClickRecognizerRef recognizer, void *context) {//決定ボタンが押されたら
//   APP_LOG(APP_LOG_LEVEL_INFO, "Select pressed!");
// }
// static void down_click_handler(ClickRecognizerRef recognizer, void *context) {//下ボタンがおされたら
//   APP_LOG(APP_LOG_LEVEL_INFO, "Down pressed!");
//   lv--;
//   APP_LOG(APP_LOG_LEVEL_INFO, "%d",lv);
  
// }

// static void click_config_provider(void *context) {//ボタンを押されたときのプロバイダー
//   // Register the ClickHandlers　クリックハンドラーを登録
//   window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
//   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
//   window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
// }


static void update_time(){
//  get a tm structure　時計の構造体を取得
  time_t temp=time(NULL);
  struct tm *tick_time=localtime(&temp);
  
  //Create a long-lived buffer　
  static char buffer[]="00:00";//一時的に０：００(バッファーというものにいれておく)
  //Write the current hours and minutes into the buffer　現在時刻をバッファーに書き込む
  if(clock_is_24h_style()==true){
    //Use 24hour format　24時間計算ならこちら
    strftime(buffer ,sizeof("00:00"), "%H:%M", tick_time);
  }else{
    //Use 12hour format　12時間計算ならこちら
    strftime(buffer ,sizeof("00:00"), "%I:%M", tick_time);
  }
  //display this time on the TextLayer　このテキストをバッファーを読み込んでディスプレイに表示する
    text_layer_set_text(timetext,buffer);
} 

static void update_level(){//レベルを書き換える
  // Get the count from persistent storage for use if it exists, otherwise use the default ローカル保存を呼び出し
  lv = persist_exists(NUM_LEVEL_PKEY) ? persist_read_int(NUM_LEVEL_PKEY) : NUM_LEVEL_DEFAULT;
  
  static char levelbody[18];
  snprintf(levelbody, sizeof(levelbody), "Lv.%d", lv);
  text_layer_set_text(leveltext, levelbody);
}
/////////////////////////////////////////////////////////////////////
static void window_load(Window *window){
  //キャラクター描画
imagebitmap = gbitmap_create_with_resource(RESOURCE_ID_SWORDBOY);
imagelayer = bitmap_layer_create(GRect(0, 0, 144, 168));
bitmap_layer_set_bitmap(imagelayer, imagebitmap);
layer_add_child(window_get_root_layer(mainwindow), bitmap_layer_get_layer(imagelayer));

  //timetextを作成
  timetext=text_layer_create(GRect(75, 0, 100, 40));//x,y,w,h　ｘ、ｙ＝（０，０）は左上 ラベルの四角の左上の座標を指定、横幅縦幅をきめる ウインドウの横は150が最大 縦は130？が最大
  text_layer_set_background_color(timetext,GColorClear);
  text_layer_set_text(timetext,"00:00");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(timetext,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(timetext,GTextAlignmentLeft);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(mainwindow),text_layer_get_layer(timetext));
  
   // Make sure the time is displayed from the start　開始してから正確な時間を表示する　ｕｐｄａｔｅ＿ｔｉｍｅを呼ぶ
  update_time();
  
  //leveltextを作成
  leveltext=text_layer_create(GRect(0, 0, 100, 40));//x,y,w,h 
  text_layer_set_background_color(leveltext,GColorClear);
  text_layer_set_text(leveltext,"Lv.0");
  update_level();
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(leveltext,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(leveltext,GTextAlignmentLeft);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(mainwindow),text_layer_get_layer(leveltext));
  
  //borderを作成
  border=text_layer_create(GRect(5, 5, 150, 42));//x,y,w,h 
  text_layer_set_background_color(border,GColorClear);
  text_layer_set_text(border,"___________");
  
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(border,fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(border,GTextAlignmentLeft);
  
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(mainwindow),text_layer_get_layer(border));
  
}
  

  static void window_unload(Window *window) {
  //menu_layer_destroy(menulayer);
  window_destroy(window);
    // Destroy GBitmap
  gbitmap_destroy(imagebitmap);
  // Destroy BitmapLayer
  bitmap_layer_destroy(imagelayer);
  mainwindow = NULL;
  // Save the count into persistent storage on app exit　ここで　ローカル保存
  persist_write_int(NUM_LEVEL_PKEY, lv);
}

void homewindow_push() {
  if(!mainwindow) {
    mainwindow = window_create();
    window_set_window_handlers(mainwindow, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(mainwindow, true);
  
    //ボタンが押されたときのコンフィグハンドラー ホームメニューデバッグ用
  //window_set_click_config_provider(mainwindow, click_config_provider);
}