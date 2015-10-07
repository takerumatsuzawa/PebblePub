/*
//  wakewindow.c 1007
//  1007 松澤 バージョン1.1
//  メインメニューにawakeを追加　1007現在、β版を追加したのみ
*/


#include <pebble.h>
 static Window *s_main_window;//◯◯というウインドウを扱うことを宣言
 static TextLayer *s_output_layer;//s_output_layerというレイヤーを扱うことを宣言
static TextLayer *m_output_layer;

//0920松澤　ここから
// static int seane=0;
// static bool trans=false;
// static int num_samples = 3;
static int n_countdown=21;//デモ用カウント　寝ていると仮定している時間
static int timeout = 45;//充分な加速度が取れたあとのタイムアウトカウント
static int max = 0;//最大加速度値
bool count=false;//falseは加速度を取る前　trueは加速度を取る時
//ここまで

//セレクトボタンのあとに1秒毎に呼び出されるハンドラー
static void timer_handler(struct tm *tick_time, TimeUnits units_changed) {
  timeout--;
  //APP_LOG(APP_LOG_LEVEL_INFO, "timeout= %d",timeout);
  //タイムアウトダウンが終わったら
  if(timeout<=0)
    //ウインドウ終了
    window_stack_pop(true);
 }

//カウントダウンのあとに動作する　GOODとなったあとタイムアウトダウンへ
void morning(){
  
  //加速度サービスをやめる 最大値を常に取りたいならコメントアウトしてください
  //accel_data_service_unsubscribe();
  
  //１秒毎に実行される　timer_handlerを実行
  tick_timer_service_subscribe(SECOND_UNIT, timer_handler);
}

//sqrtや＾を使わず平方根を表現 自作sqrt（平方根をもとめる）式　　PEBBLECLOUDでは”#include <math.h>”も”sqrt"も不可　残念
double MySqrt(double x){
  int nMAX=10;
  double m = x;
  for(int n=1;n<=nMAX;n++){
        m = (m + x / m) / 2;
  }
  return m;
}

//snprintfではなぜかdoubleが使えない　intに変換するための関数　小数点以下はカット
int MyInt(double x) {
  return x;
}

//生データを処理するコールバック関数、現在100Hzで駆動させている
//データハンドラーのフォーマットに従い、*dataに3件のデータが格納される
static void data_handler(AccelData *data, uint32_t num_samples) {
  //長く使用可能なバッファー（バッファーとは一時格納の領域）
  static char s_buffer[128];
  
  //最大値を格納しておく
  static char MAX[128];
  
  
  //何もしていない場合のガル補正
  //data[0].z=data[0].z-1100;
  
  //マグニチュードが2000を越えたらGOODと表示
  int m = MyInt(MySqrt((data[0].x*data[0].x)+(data[0].y*data[0].y)+(data[0].z*data[0].z)));
  
  //何もしてない場合のガル補正
  //m=m-1400;
  
  //ガル単位に直す　km/h
  //m=(m/1000)*3.6;
  
  //ミリGからGに直す
  m=m/1000;
  int g=m;
  
  //岡本追加 0921
  //m=m/1000*980/1000*3.6;
  //1997　小野論文より　１５km/h〜３０km/hのパンチ力が出される
  m=0.2857*m+13.753;
  //

  //通常は何もしていないと１２００〜１４００　ここでgoodになる閾値を決められる
//   if (m>5000) {
  if (m>15) {
//     snprintf(s_buffer, sizeof(s_buffer), "%d \n\n [GOODMORNING!]", m);
    snprintf(s_buffer, sizeof(s_buffer), "%d", m);
    //,(m/1000)*3.6
    text_layer_set_text(s_output_layer, s_buffer);
    
    //最大値が出たら
    if(m>max){
      max=m;
    snprintf(MAX, sizeof(MAX), "MAX is %dkm/h\n [GOODMORNING!]", max);
    text_layer_set_text(m_output_layer, MAX);
    }
    
    //0920松澤
    morning();
    //
    
  } else {
    snprintf(s_buffer, sizeof(s_buffer),  "m= %d, G=%d ", m,g);
  }
  //データをテキストレイヤーに表示
  text_layer_set_text(s_output_layer, s_buffer);
}

static void update_time(){
  static char buffer[]="000000";
     n_countdown--;
  snprintf(buffer, sizeof(buffer),  "%d ", n_countdown);
  
  //カウントダウンが０以下なら
  if(n_countdown<=0&&count==false){
    //PEBBLEをバイブレーションさせる
  // Vibe pattern: オンとオフの交互で、単位はms
  static const uint32_t segments[] ={ 
  200 , 100 , 400 , 200,
  200 , 100 , 400 , 200,
  200 , 100 , 400 , 200};

  VibePattern pat = {
   . durations = segments ,
   . num_segments = ARRAY_LENGTH ( segments ),
 };
  vibes_enqueue_custom_pattern(pat);
    //バイブレーション　ここまで
    
  text_layer_set_text(s_output_layer, "Wakeup!Punch! \nPressSlectButton");
    
  }else if(n_countdown>0&&count==false){
    //カウントダウン中　０より大なら秒数を表示
    text_layer_set_text(s_output_layer, buffer);
  }
}

//0920松澤ここから
 static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
   update_time();
 }
//ここまで

//0920松澤　ここから
//ダウンボタンが押されたときの処理
static void wakeup_downclick_handler(ClickRecognizerRef recognizer,void *context){
  //タイマーによって動作するサービスを使用
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}
//ここまで

//0920松澤　ここから
//セレクトボタンが押されたときの処理
static void wakeup_selectclick_handler(ClickRecognizerRef recognizer,void *context){
  if(n_countdown<=0){
    count=true;
    //バイブストップ
    vibes_cancel();
    
    int num_samples = 3;
    accel_data_service_subscribe(num_samples, data_handler);//data_handlerで呼び出される
    //サンプリングレート 大きいほど正確になり、１sごとの更新が多くなる　デフォルトは25Hz　(10 25 50 100のどれか)
    accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
  }
}
//ここまで

//0920松澤　ここから
//ボタンイベントを登録する
static void wakeup_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_DOWN,wakeup_downclick_handler);//ダウンボタンシングルクリックのハンドラー
  window_single_click_subscribe(BUTTON_ID_SELECT,wakeup_selectclick_handler);//セレクトボタンシングルクリックのハンドラー
}
//ここまで

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  window_set_click_config_provider(window, wakeup_click_config_provider);//クリックを受け取る
  //window_set_click_config_provider(window,window_single_click_subscribe);//クリックを受け取る

  //出力用のテキストレイヤーを作る　s_output_layerというもの
  s_output_layer = text_layer_create(GRect(5,0,window_bounds.size.w-10, window_bounds.size.h));//どの位置にレイヤーを置くか設定
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));//テキストを記入する際のフォントと文字の大きさを設定
  text_layer_set_text(s_output_layer,"Sleep \nDEMO!! \nPressDownButton");//テキストレイヤーに記入する　記入先のレイヤー名と記入内容を設定
  text_layer_set_overflow_mode(s_output_layer,GTextOverflowModeWordWrap);//テキストレイヤーの記入方法を設定　今回は枠外にあふれたら下へ改行する
  layer_add_child(window_layer,text_layer_get_layer(s_output_layer));//レイヤーの子に設定する　
  
  //最大値表示用レイヤーを作る 追加
  m_output_layer = text_layer_create(GRect(5,80,window_bounds.size.w-10, window_bounds.size.h));//どの位置にレイヤーを置くか設定
  text_layer_set_font(m_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));//テキストを記入する際のフォントと文字の大きさを設定
  text_layer_set_text(m_output_layer,"");//テキストレイヤーに記入する　記入先のレイヤー名と記入内容を設定
  text_layer_set_overflow_mode(m_output_layer,GTextOverflowModeWordWrap);//テキストレイヤーの記入方法を設定　今回は枠外にあふれたら下へ改行する
  layer_add_child(window_layer,text_layer_get_layer(m_output_layer));//レイヤーの子に設定する　
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
  
  //追加
  text_layer_destroy(m_output_layer);
  //
}

void awake_push() {
  if(!s_main_window) {
    s_main_window = window_create();

    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}

// static void init() {
//   // Create main Window
//   s_main_window = window_create();
//   window_set_window_handlers(s_main_window, (WindowHandlers) {
//     .load = main_window_load,
//     .unload = main_window_unload
//   });
//   //ウインドウをプッシュ（画面にウインドウを適用する⇄ポップ）
//   window_stack_push(s_main_window, true);
// }

// static void deinit() {
//   window_destroy(s_main_window);//s_main_windowというウインドウを除去
//   accel_data_service_unsubscribe();//加速度計データイベントサービスを解除
// }

// int main(void) {
//   init();
//   app_event_loop();
//   deinit();
// }