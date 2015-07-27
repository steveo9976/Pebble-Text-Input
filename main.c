#include <pebble.h>
#include "machinel.h"
 
//for the moving circle's dimension
#define BOX_SIZE 5
#define b1 300
#define b2 120
#define b3 -100
#define b4 -320
#define b5 -500
#define b6 -700
       
TextLayer *words [5];
TextLayer *letters [5];
static Window *s_main_window;
 
static double num = 0;
static int loc = 0;
static bool ready_to_input = false;
static bool word_pane = false;
char alphabet [26][2];
char input_word[25];
 
struct aos *string_arr;
 
//Initialized variables for the moving circle on the edge
static Layer *s_box_layer;
static PropertyAnimation *s_prop_animation;
GRect to_rect;
static void animation_started(Animation *animation, void *data) {}
static void animation_stopped(Animation *animation, bool finished, void *data) {}
 
//Destroying the animation instance for the moving circle
static void destroy_property_animation(PropertyAnimation **prop_animation) {
  if (*prop_animation == NULL) {
    return;
  }
  if (animation_is_scheduled((Animation*) *prop_animation)) {
    animation_unschedule((Animation*) *prop_animation);
  }
  property_animation_destroy(*prop_animation);
  *prop_animation = NULL;
}
//end of moving circle addition
 
 
static void data_handler(AccelData *data, uint32_t num_samples) {
        static bool letter_select = false;
        static bool word_select = false;
       
        //take the average of 3 samples
        int accel_x = (data[0].x + data[1].x + data[2].x)/3;
        int accel_y = (data[0].y + data[1].y + data[2].y)/3;
        int accel_z = (data[0].z + data[1].z + data[2].z)/3;
       
        //use accel_y to determine how fast the scroll is
        //0 - -400 : stable
        //adding to_rect updates for the moving circle's destination
        if (accel_y > b1) { //go to top of list?
    to_rect = GRect(0, 0, BOX_SIZE, BOX_SIZE);         
                num -= 1;                      
        } else if (accel_y <= b1 && accel_y > b2) {
    to_rect = GRect(0, ((b1-accel_y)*32)/(b1-b2), BOX_SIZE, BOX_SIZE);
                num -= 0.5;
        } else if (accel_y <= b2 && accel_y > b3) {
    to_rect = GRect(0, 32 + ((b2-accel_y)*32)/(b2-b3), BOX_SIZE, BOX_SIZE);
                num -= 0.25;
        } else if (accel_y <= b3 && accel_y > b4) {
    to_rect = GRect(0, 64 + ((b3-accel_y)*35)/(b3-b4), BOX_SIZE, BOX_SIZE);            
        } else if (accel_y <= b4 && accel_y > b5) {
    to_rect = GRect(0, 99 + ((b4-accel_y)*32)/(b4-b5), BOX_SIZE, BOX_SIZE);
                num += 0.25;
        } else if (accel_y <= b5 && accel_y > b6) {
    to_rect = GRect(0, 131 + ((b5-accel_y)*32)/(b5-b6), BOX_SIZE, BOX_SIZE);
                num += 0.5;
        } else {
    to_rect = GRect(0, 153, BOX_SIZE, BOX_SIZE);
                num += 1;
        }
       
        if (accel_y <= b2 && accel_y > b5) {
                //stable
                //letter select
                if (accel_z > -750) {
                        letter_select = true;
                } else {
                        if (letter_select) {
                                letter_select = false;
                                //vibes_short_pulse();
                                ready_to_input = true;
                        }
                }
               
                //change panes
                if (accel_x < -65) {
                        word_select = true;
                } else {
                        if (word_select) {
                                word_select = false;
                                word_pane = !word_pane;
                                //vibes_short_pulse();
                        }
                }
        }
       
        if (num < 0)
                        num = 0;
        if (!word_pane) {
                if (num > 25)
                        num = 25;
        } else {
                if (num > 4)
                        num = 4;
        }
       
        loc = (int)num;
       
        //animation launching for moving circle
  destroy_property_animation(&s_prop_animation);
  s_prop_animation = property_animation_create_layer_frame(s_box_layer, NULL, &to_rect);
  //CHANGE DURATION IF LAGGY.
  animation_set_duration((Animation*) s_prop_animation, 50);
  animation_set_curve((Animation*) s_prop_animation, AnimationCurveEaseInOut);
   
  //Show the data
  animation_set_handlers((Animation*) s_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler) animation_started,
    .stopped = (AnimationStoppedHandler) animation_stopped,
  }, NULL /* callback data */);
  animation_schedule((Animation*) s_prop_animation);           
        //end animation code sections
       
        //(APP_LOG_LEVEL_DEBUG, "num: %f%c loc: %d%c", num, (int)num+'A', loc, (int)loc+'A');
 
        //the input word
  text_layer_set_text(words[0], input_word);
       
        //struct aos *string_arr = create_aos (1);
       
        //destroy_aos(string_arr);
 
        for (int i = 0; i < 5; i++) {
                if (loc >= 2 && loc <= 23) {
                        text_layer_set_text(letters[i], alphabet[(loc - 2 + i)]);
                } else if (loc < 2) {
                        text_layer_set_text(letters[i], alphabet[i]);
                }  else if (loc > 23) {
                        text_layer_set_text(letters[i], alphabet[(26-5+ i)]);
                }
               
                if (i == loc && word_pane) {
                        text_layer_set_text_color(words[i], GColorWhite);
        text_layer_set_background_color(words[i], GColorBlack);
                } else {
        text_layer_set_text_color(words[i], GColorBlack);
        text_layer_set_background_color(words[i], GColorWhite);
                }
               
                int highlight = 2;
               
                if (loc < 2) { //highlight a,b stuff
                        highlight = loc;
                } else if (loc > 23) { //highlight y,z stuff
                        highlight = 4 - (25 - loc);
                }
               
                if (i == highlight && !word_pane) {
        text_layer_set_text_color(letters[i], GColorWhite);
        text_layer_set_background_color(letters[i], GColorBlack);
                        //text_layer_set_font(letters[i], bigfontName);
                } else {
                        text_layer_set_text_color(letters[i], GColorBlack);
        text_layer_set_background_color(letters[i], GColorWhite);      
                        //text_layer_set_font(letters[i], smallfontName);      
                }              
        }
       
        if (ready_to_input) {
                if (!word_pane) { //concats to the selection
                        input_word[strlen(input_word)] = 'A' + loc;
                       
                        if (strlen(input_word) >= 2) {
                        //      search_fun(string_arr, input_word);
                               
                               
                               
                                int len = aos_length(string_arr);
                //char str[4][456];
        for(int i = 0; i < len; i++){
    if( (string_arr->strings[i])[0] == input_word[0] && (string_arr->strings[i])[1] == input_word[1]){
                        int count = 1;
      for(int j = i; j < i + 4; j++){
                                if (string_arr->strings[j] == NULL)
                                        break;
                                text_layer_set_text(words[count], string_arr->strings[j]);
        //str[count] = string_arr->strings[j];
                                count++;
      }
    }  
  }
                               
                               
                               
                               
                               
 
                               
                               
                        }
                       
                        ready_to_input = false;
                } else { //reset everything and select the word from main bae
                        //word_pane = false;
                        ready_to_input = false;
                        aos_add(string_arr, input_word);
                        int length = (int)strlen(input_word);
                        for (int i = 0; i < length ; i++)
                                input_word[i] = '\0';
                        for (int i = 0; i < 5; i++) {
                                  text_layer_set_text(words[i], "");
                        }
                }
        }
       
}
 
//Defining the aesthetics of the animated circle
static void update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 1, GCornersAll);
}
 
static void main_window_load(Window *window) {
  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items late
  Layer *window_layer = window_get_root_layer(window);
  for (int i=0; i< 5; i++){
    letters[i] = text_layer_create(GRect(0, 33*i, 36, 33*i + 33));
    words[i] = text_layer_create(GRect(36, 33*i, 144, 33*i + 33));
    text_layer_set_text(letters[i], "A");
    text_layer_set_text(words[i], "");
    text_layer_set_text_color(words[i], GColorBlack);
    text_layer_set_background_color(words[i], GColorWhite);
                if (i == 2) {
        text_layer_set_text_color(letters[i], GColorWhite);
        text_layer_set_background_color(letters[i], GColorBlack);
                } else {
                        text_layer_set_text_color(letters[i], GColorBlack);
        text_layer_set_background_color(letters[i], GColorWhite);      
                }
                text_layer_set_text_alignment(letters[i], GTextAlignmentCenter);
                       
                text_layer_set_font(letters[i], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));       
                text_layer_set_font(words[i], fonts_get_system_font(FONT_KEY_GOTHIC_24));      
               
    layer_add_child(window_layer, text_layer_get_layer(letters[i]));
    layer_add_child(window_layer, text_layer_get_layer(words[i]));    
  }
       
        //Moving ball animation launch here
        s_box_layer = layer_create(GRect(0, 0, BOX_SIZE, BOX_SIZE));
  //Setting up the box and its aesthetics, and pushing the box to the window
  layer_set_update_proc(s_box_layer, update_proc);
  layer_add_child(window_layer, s_box_layer);
 
  //Setting the starting position to the middle neutral position.
  to_rect = GRect(0, 77, BOX_SIZE, BOX_SIZE);
 
  //Running and launching the animation
  s_prop_animation = property_animation_create_layer_frame(s_box_layer, NULL, &to_rect);
  animation_schedule((Animation*) s_prop_animation);
        //Moving ball code ends here
}
 
void main_window_unload(Window *window) {
        //Destroying the animation and the moving circle
  destroy_property_animation(&s_prop_animation);
  layer_destroy(s_box_layer);
}
 
static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
       
        string_arr = create_aos (2);
       
        for (int i = 0; i < 26; i++) {
                alphabet[i][0] = i + 'A';
                alphabet[i][1] = '\0';
        }
       
        for (int i = 0; i < (int)strlen(input_word); i++)
                input_word[i] = '\0';
       
        // Subscribe to the accelerometer data service
        int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);
 
  // Choose update rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_25HZ);
}
 
static void deinit() {
  window_destroy(s_main_window);
  accel_data_service_unsubscribe();
        destroy_aos(string_arr);
}
 
int main(void) {
  init();
  app_event_loop();
  deinit();
}
