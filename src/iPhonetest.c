#include <pebble.h>
#include "time.h"
static Window *mainwindow;//,*timeset,*evening,*wakeup,*grad,*morning;//mainwindow　timeset wakeupwindow ウインドウを定義
static TextLayer *timetext,*leveltext,*border;//画面１　timetext　時間 leveltext 良い習慣を心がけているレベル  border境目
//static TextLayer *whattext1,*whattext2,*timetext;//画面２
//
//
static TextLayer *information;//実績通知
//static bool *s1,*s2,*s3,*s4,*s5,*s6,*s7,*s8,*s9,*s10;//実績１０個
//static TextLayer *t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,*t9,*t10;//実績用レイヤー
//static WakeupId s_wakeup_id = -1;
//static time_t s_wakeup_timestamp = 0;

#define  NUM_LEVEL_PKEY 1
// You can define defaults for values in persistent storage
#define NUM_LEVEL_DEFAULT 0
static int lv = NUM_LEVEL_DEFAULT;

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
  static char levelbody[18];
  snprintf(levelbody, sizeof(levelbody), "Lv.%d", lv);
  text_layer_set_text(leveltext, levelbody);
}
static void mainwindow_load(Window *window){
//  *s1=true;
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
  
  
  //informationを作成
  information=text_layer_create(GRect(0, 130, 150, 40));//x,y,w,h 
  text_layer_set_background_color(information,GColorClear);
  text_layer_set_text(information,"information");
  //ウォッチっぽくなるようにレイアウトを改善
  text_layer_set_font(information,fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(information,GTextAlignmentLeft);
  //レイヤーをウインドウの子に加える
  layer_add_child(window_get_root_layer(mainwindow),text_layer_get_layer(information));
}
static void mainwindow_unload(Window *window) {
  text_layer_destroy(timetext);
   text_layer_destroy(leveltext);
   text_layer_destroy(border);
}
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {// 上ボタンが押されたら
  APP_LOG(APP_LOG_LEVEL_INFO, "Up pressed!");
     // Switch to countdown window　ここで遷移　スウィッチ
  lv++;
  APP_LOG(APP_LOG_LEVEL_INFO, "%d",lv);
  update_level();
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {//決定ボタンが押されたら
  APP_LOG(APP_LOG_LEVEL_INFO, "Select pressed!");
  //time_push();
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
  static void window_unload(Window *window) {
  text_layer_destroy(timetext);
}
  
static void init(){
  //メインウィンドウの要素を作成し、ポインタに割り当てる　ウインドウ作成　ハンドラー定義
  mainwindow=window_create();//
  //ウインドウの中身の要素を操作するハンドラーをセット　消したり出したり
  window_set_window_handlers(mainwindow,(WindowHandlers){
    .load=mainwindow_load,
    .unload=mainwindow_unload
  });
  
   // Get the count from persistent storage for use if it exists, otherwise use the default ローカル保存を呼び出し
  lv = persist_exists(NUM_LEVEL_PKEY) ? persist_read_int(NUM_LEVEL_PKEY) : NUM_LEVEL_DEFAULT;
  
  //決定ボタンが押されたとき，ウォッチのウインドウに表示する　アニメーションはｔｒｕｅ
  window_stack_push(mainwindow,true);
  
  //ボタンが押されたときのコンフィグハンドラー
  window_set_click_config_provider(mainwindow, click_config_provider);
  
}
  
static void deinit(){
    // Save the count into persistent storage on app exit　ここで　ローカル保存
  persist_write_int(NUM_LEVEL_PKEY, lv);
  //ウインドウを削除するときに呼び出される
  window_destroy(mainwindow);
}


int main(void){
init();
  app_event_loop();
  deinit();
}