
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_video.h>
#include <math.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 10 audios can be played at a time.
const int RESERVE_SAMPLES = 10;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
enum {
    SCENE_MENU = 1,
    SCENE_START = 2,
    // [HACKATHON 3-7]
    // TODO: Declare a new scene id.
    SCENE_SETTINGS = 3,
    SCENE_PICK = 4,
    SCENE_END1 = 5,
    SCENE_END2 = 6,
    SCENE_END3 = 7,
    SCENE_INTRO1 = 8,
    SCENE_INTRO2 = 9,
    SCENE_INTRO3 = 10,
    SCENE_INTRO4 = 11
};

/* Input states */

// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...
float volume = 1;
/* Variables for allegro basic routines. */
int score = 0;
float plane_speed = 1.0f;
ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_100;
ALLEGRO_FONT* font_pirulen_60;
ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;


ALLEGRO_BITMAP* black_background;
/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_BITMAP* img_intro1;
ALLEGRO_BITMAP* img_intro2;
ALLEGRO_BITMAP* img_pg_intro1;
ALLEGRO_BITMAP* img_pg_intro2;
ALLEGRO_BITMAP* img_pg_intro3;
ALLEGRO_BITMAP* img_pg_intro4;
ALLEGRO_BITMAP* img_pre_pg1;
ALLEGRO_BITMAP* img_pre_pg2;
ALLEGRO_BITMAP* img_next_pg1;
ALLEGRO_BITMAP* img_next_pg2;
ALLEGRO_BITMAP* img_home_pg1;
ALLEGRO_BITMAP* img_home_pg2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;

ALLEGRO_SAMPLE* setting_bgm;
ALLEGRO_SAMPLE_ID setting_bgm_id;
ALLEGRO_BITMAP* img_plus;
ALLEGRO_BITMAP* img_plus2;
ALLEGRO_BITMAP* img_minus;
ALLEGRO_BITMAP* img_minus2;
#define MAX_TIME 20;
#define MAX_BLOOD 40;
/* Start Scene resources*/
double last_count_time = 0.f;
int cur_length = MAX_TIME;
ALLEGRO_BITMAP* start_img_loading;
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_plane1;
ALLEGRO_BITMAP* start_img_plane2;
ALLEGRO_BITMAP* start_img_plane3;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_BITMAP* end2_background;
ALLEGRO_BITMAP* end3_background;
ALLEGRO_BITMAP* img_explosion[9];
ALLEGRO_BITMAP* menu_img_background;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
ALLEGRO_SAMPLE* beat_bgm;
ALLEGRO_SAMPLE_ID beat_bgm_id;
ALLEGRO_BITMAP* img_heart;
ALLEGRO_BITMAP* img_enemy[4];
ALLEGRO_BITMAP* img_blood;
ALLEGRO_SAMPLE_ID laser_bgm_id;
ALLEGRO_SAMPLE* laser_bgm;
ALLEGRO_SAMPLE* shot_bgm;
ALLEGRO_SAMPLE_ID shot_bgm_id;
ALLEGRO_SAMPLE* explosion_bgm;
ALLEGRO_SAMPLE_ID explosion_bgm_id;
ALLEGRO_SAMPLE* beep_bgm;
ALLEGRO_SAMPLE_ID beep_bgm_id;
ALLEGRO_BITMAP* img_timeup;
ALLEGRO_SAMPLE* lose_bgm;
ALLEGRO_SAMPLE_ID lose_bgm_id;
ALLEGRO_SAMPLE* win_bgm;
ALLEGRO_SAMPLE_ID win_bgm_id;
ALLEGRO_SAMPLE* flip_bgm;
ALLEGRO_SAMPLE_ID flip_bgm_id;
float time_up_size = 1.f;
int first = 1;
int end = 0;
/* Pick Scene resources*/
ALLEGRO_BITMAP* pick_img_background;
ALLEGRO_BITMAP* pick_img_plane1;
ALLEGRO_BITMAP* pick_img_plane2;
ALLEGRO_BITMAP* pick_img_plane3;
ALLEGRO_SAMPLE* pick_bgm;
ALLEGRO_SAMPLE_ID pick_bgm_id;
ALLEGRO_SAMPLE* confirm_bgm;
ALLEGRO_SAMPLE_ID confirm_bgm_id;
ALLEGRO_SAMPLE* air_bgm;
ALLEGRO_SAMPLE_ID air_bgm_id;

int character = 0;
int change = 1;

ALLEGRO_BITMAP* img_bullet;

typedef struct {
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x, y axes.
    float vx, vy;
    // Should we draw this object on the screen.
    bool hidden;
    bool explosion;
    int life;
    int bullet;
    int type;
    // The pointer to the object’s image.
    ALLEGRO_BITMAP* img;
} MovableObject;
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 3
#define MAX_BULLET 5
#define MAX_LIFE 3
int BULLET;
int blood = 40;
int step = 0;
MovableObject plane;
MovableObject enemies[MAX_ENEMY];
int enemies_get_shot[MAX_ENEMY] = {0};
MovableObject bullets[MAX_BULLET];
const float MAX_COOLDOWN = 0.2f;
double last_shoot_timestamp = 0.0f;

double last_plane_OFB = 0.0f;

/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv) {
    // Set random seed for better random outcome.
    srand(time(NULL));
    allegro5_init();
    game_log("Allegro5 initialized");
    game_log("Game begin");
    // Initialize game variables.
    game_init();
    game_log("Game initialized");
    // Draw the first frame.
    game_draw();
    game_log("Game start event loop");
    // This call blocks until the game is finished.
    game_start_event_loop();
    game_log("Game end");
    game_destroy();
    return 0;
}

void allegro5_init(void) {
    if (!al_init())
        game_abort("failed to initialize allegro");

    // Initialize add-ons.
    if(!al_init_video_addon())
        game_abort("failed to initialize video add-on");
    if (!al_init_primitives_addon())
        game_abort("failed to initialize primitives add-on");
    if (!al_init_font_addon())
        game_abort("failed to initialize font add-on");
    if (!al_init_ttf_addon())
        game_abort("failed to initialize ttf add-on");
    if (!al_init_image_addon())
        game_abort("failed to initialize image add-on");
    if (!al_install_audio())
        game_abort("failed to initialize audio add-on");
    if (!al_init_acodec_addon())
        game_abort("failed to initialize audio codec add-on");
    if (!al_reserve_samples(RESERVE_SAMPLES))
        game_abort("failed to reserve samples");
    if (!al_install_keyboard())
        game_abort("failed to install keyboard");
    if (!al_install_mouse())
        game_abort("failed to install mouse");
    // TODO: Initialize other addons such as video, ...

    // Setup game display.
    game_display = al_create_display(SCREEN_W, SCREEN_H);
    if (!game_display)
        game_abort("failed to create display");
    al_set_window_title(game_display, "I2P(I)_2020 Final Project <109062312>");

    // Setup update timer.
    game_update_timer = al_create_timer(1.0f / FPS);
    if (!game_update_timer)
        game_abort("failed to create timer");

    // Setup event queue.
    game_event_queue = al_create_event_queue();
    if (!game_event_queue)
        game_abort("failed to create event queue");

    // Malloc mouse buttons state according to button counts.
    const unsigned m_buttons = al_get_mouse_num_buttons();
    game_log("There are total %u supported mouse buttons", m_buttons);
    // mouse_state[0] will not be used.
    mouse_state = malloc((m_buttons + 1) * sizeof(bool));
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

    // Register display, timer, keyboard, mouse events to the event queue.
    al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
    al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(game_event_queue, al_get_keyboard_event_source());
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    
    
    // Start the timer to update and draw the game.
    al_start_timer(game_update_timer);
}

void game_init(void) {
    /* Shared resources*/
    font_pirulen_100 = al_load_font("pirulen.ttf", 100, 0);
    if (!font_pirulen_100)
        game_abort("failed to load font: pirulen.ttf with size 100");
    font_pirulen_60 = al_load_font("pirulen.ttf", 60, 0);
    if (!font_pirulen_60)
        game_abort("failed to load font: pirulen.ttf with size 60");
    
    font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
    if (!font_pirulen_32)
        game_abort("failed to load font: pirulen.ttf with size 32");

    font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
    if (!font_pirulen_24)
        game_abort("failed to load font: pirulen.ttf with size 24");

    end2_background = load_bitmap_resized("end2.jpg", SCREEN_W, SCREEN_H);
    end3_background = load_bitmap_resized("end3.jpg", SCREEN_W, SCREEN_H);
    black_background = load_bitmap_resized("black.png", SCREEN_W, SCREEN_H);
    img_intro1 = load_bitmap_resized("intro1.png", 150, 150);
    img_intro2 = load_bitmap_resized("intro2.png", 150, 150);
    img_pg_intro1= load_bitmap_resized("pg1.png", SCREEN_W, SCREEN_H);
    img_pg_intro2 = load_bitmap_resized("pg2.png", SCREEN_W, SCREEN_H);
    img_pg_intro3 = load_bitmap_resized("pg3.png", SCREEN_W, SCREEN_H);
    img_pg_intro4 = load_bitmap_resized("pg4.png", SCREEN_W, SCREEN_H);
    img_pre_pg1= load_bitmap_resized("pre_page1.png", 60, 60);
    img_pre_pg2 = load_bitmap_resized("pre_page2.png", 60, 60);
    img_next_pg1 = load_bitmap_resized("next_page1.png", 60, 60);
    img_next_pg2 = load_bitmap_resized("next_page2.png", 60, 60);
    img_home_pg1 = load_bitmap_resized("home_page1.png", 60, 60);
    img_home_pg2 = load_bitmap_resized("home_page2.png", 60, 60);
    /* Menu Scene resources*/
    main_img_background = load_bitmap_resized("background.jpg", SCREEN_W, SCREEN_H);

    main_bgm = al_load_sample("S31-Night Prowler.ogg");
    if (!main_bgm)
        game_abort("failed to load audio: S31-Night Prowler.ogg");

 
    img_settings = al_load_bitmap("settings.png");
    if (!img_settings)
        game_abort("failed to load image: settings.png");
    img_settings2 = al_load_bitmap("settings2.png");
    if (!img_settings2)
        game_abort("failed to load image: settings2.png");
    /* Pick Scene resources*/
    pick_img_background = load_bitmap_resized("pick.jpg", SCREEN_W, SCREEN_H);
    if (!pick_img_background)
        game_abort("failed to load image: pick.jpg");
    pick_img_plane1 = load_bitmap_resized("plane1.png", 150, 250);
    if (!pick_img_plane1)
        game_abort("failed to load image: plane1.png");
    pick_img_plane2 = load_bitmap_resized("plane2_blur.png", 150, 250);
    if (!pick_img_plane2)
        game_abort("failed to load image: plane2_blur.png");
    pick_img_plane3 = load_bitmap_resized("plane3_blur.png", 150, 250);
    if (!pick_img_plane3)
        game_abort("failed to load image: plane3_blur.png");
    pick_bgm = al_load_sample("pick_bgm.ogg");
    if (!pick_bgm)
        game_abort("failed to load audio: pick_bgm.ogg");
    confirm_bgm= al_load_sample("confirm_bgm.ogg");
    if (!confirm_bgm)
        game_abort("failed to load audio: confirm_bgm.ogg");
    air_bgm = al_load_sample("air_sound.ogg");
    if (!air_bgm)
        game_abort("failed to load audio: air_sound.ogg");
    flip_bgm = al_load_sample("flip.mp3");
    if (!flip_bgm)
        game_abort("failed to load audio: flip.mp3");
    /* Setting Scene resources*/
    img_plus = load_bitmap_resized("plus.png",150,150);
    if (!img_plus)
        game_abort("failed to load image: settings.png");
    img_plus2 = load_bitmap_resized("plus2.png",150,150);
    if (!img_plus2)
        game_abort("failed to load image: plus2.png");
    img_minus = load_bitmap_resized("minus.png",150,150);
    if (!img_minus)
        game_abort("failed to load image: minus.png");
    img_minus2 = load_bitmap_resized("minus2.png",150,150);
    if (!img_minus2)
        game_abort("failed to load image: minus2.png");
    /* Start Scene resources*/
    start_img_background = load_bitmap_resized("galaxy.jpg", SCREEN_W, SCREEN_H);
    img_blood=load_bitmap_resized("blood.png", 10, 40);
    if (!img_blood)
        game_abort("failed to load image: blood.png");
    start_img_loading = load_bitmap_resized("loading.png", 10, 15);
    if (!start_img_loading)
        game_abort("failed to load image: loading.png");
    start_img_plane = al_load_bitmap("rocket-2.png");
    if (!start_img_plane)
        game_abort("failed to load image: rocket-2.png");
    start_img_plane1= load_bitmap_resized("pln1.png", 75, 57);
    if (!start_img_plane1)
        game_abort("failed to load image: pln1.png");
    start_img_plane2 = load_bitmap_resized("pln2.png", 75, 57);
    if (!start_img_plane2)
        game_abort("failed to load image: pln2.png");
    start_img_plane3 = load_bitmap_resized("pln3.png", 75, 57);
    if (!start_img_plane3)
        game_abort("failed to load image: pln3.png");

    img_timeup = load_bitmap_resized("timesup.png", time_up_size*2, time_up_size);

    start_img_enemy = al_load_bitmap("smallfighter0006.png");
    if (!start_img_enemy)
        game_abort("failed to load image: smallfighter0006.png");
    
    beep_bgm = al_load_sample("beep.mp3");
    if (!beep_bgm)
        game_abort("failed to load audio: beep.mp3");
    laser_bgm = al_load_sample("laser.ogg");
    if (!laser_bgm)
        game_abort("failed to load audio: laser.ogg");
    shot_bgm = al_load_sample("shot.ogg");
    if (!shot_bgm)
        game_abort("failed to load audio: shot.ogg");
    explosion_bgm = al_load_sample("explosion.ogg");
    if (!explosion_bgm)
        game_abort("failed to load audio: explosion.ogg");
    img_heart= load_bitmap_resized("heart.png", 50, 50);
    if (!img_heart)
        game_abort("failed to load image: heart.png");
    beat_bgm = al_load_sample("heartbeat.ogg");
    if (!beat_bgm)
        game_abort("failed to load audio: heartbeat.ogg");
    start_bgm = al_load_sample("mythica.ogg");
    if (!start_bgm)
        game_abort("failed to load audio: mythica.ogg");
    lose_bgm = al_load_sample("lose.mp3");
    if (!lose_bgm)
        game_abort("failed to load audio: lose.mp3");
    win_bgm = al_load_sample("win.mp3");
    if (!win_bgm)
        game_abort("failed to load audio: win.mp3");
    img_explosion[0]= load_bitmap_resized("explosion00.png", 60, 60);
    img_explosion[1] = load_bitmap_resized("explosion01.png", 60, 60);
    img_explosion[2] = load_bitmap_resized("explosion02.png", 60, 60);
    img_explosion[3] = load_bitmap_resized("explosion03.png", 60, 60);
    img_explosion[4] = load_bitmap_resized("explosion04.png", 60, 60);
    img_explosion[5] = load_bitmap_resized("explosion05.png", 60, 60);
    img_explosion[6] = load_bitmap_resized("explosion06.png", 60, 60);
    img_explosion[7] = load_bitmap_resized("explosion07.png", 60, 60);
    img_explosion[8] = load_bitmap_resized("explosion08.png", 60, 60);
    setting_bgm = al_load_sample("bell.ogg");
    if (!setting_bgm)
        game_abort("failed to load audio: bell.ogg");
    img_enemy[0] = load_bitmap_resized("enemy1.png", 50, 50);
    if (!img_enemy[0])
        game_abort("failed to load image: enemy1.png");
    img_enemy[1] = load_bitmap_resized("enemy2.png", 50, 50);
    if (!img_enemy[1])
        game_abort("failed to load image: enemy2.png");
    img_enemy[2] = load_bitmap_resized("enemy3.png", 50, 50);
    if (!img_enemy[2])
        game_abort("failed to load image: enemy3.png");
    img_enemy[3] = load_bitmap_resized("enemy4.png", 50, 50);
    if (!img_enemy[3])
        game_abort("failed to load image: enemy4.png");
    /* Menu Scene resources*/
    menu_img_background = al_load_bitmap("wood.jpg");
    if (!menu_img_background)
        game_abort("failed to load image: wood.jpg");

    

    // Change to first scene.
    game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
    bool done = false;
    ALLEGRO_EVENT event;
    int redraws = 0;
    while (!done) {
        al_wait_for_event(game_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // Event for clicking the window close button.
            game_log("Window close button clicked");
            done = true;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Event for redrawing the display.
            if (event.timer.source == game_update_timer)
                // The redraw timer has ticked.
                redraws++;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Event for keyboard key down.
            game_log("Key with keycode %d down", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Event for keyboard key up.
            game_log("Key with keycode %d up", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Event for mouse key down.
            game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Event for mouse key up.
            game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                // Event for mouse move.
                // game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if (event.mouse.dz != 0) {
                // Event for mouse scroll.
                game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
            }
        }
        

        // Redraw
        if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
            // if (redraws > 1)
            //     game_log("%d frame(s) dropped", redraws - 1);
            // Update and draw the next frame.
            game_update();
            game_draw();
            redraws = 0;
        }
    }
}
void game_update(void) {
    if (active_scene == SCENE_START&&end==0) {
        if (plane.life == 1&&change==1) {
            if (!al_play_sample(beat_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &beat_bgm_id))
                game_abort("failed to play audio (bgm)");
            al_stop_sample(&start_bgm_id);
            game_log("stop audio (bgm)");
            change = 0;
        }
        double now2 = al_get_time();
        if (last_count_time == 0) {
            last_count_time = al_get_time();
        }
        else if(now2-last_count_time>=2) {
            last_count_time = now2;
            cur_length--;
        }
        //change the speed of enemy plane
        for (int i = 0; i < MAX_ENEMY; i++) {
            double now = al_get_time();
            enemies[i].vx = sin(now*(i+1))*1.5+sin(now* (i + 3))*1.5;
            enemies[i].vy = fabs(sin(now * (i + 1) ) / 2) * 1.5+ fabs( sin( now * (i + 3) ) / 2) * 1.5;
        }
        //
        plane.vx = plane.vy = 0;
        if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
            plane.vy -= plane_speed;
        if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
            plane.vy += plane_speed;
        if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
            plane.vx -= plane_speed;
        if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
            plane.vx += plane_speed;
        // 0.71 is (1/sqrt(2)).
        plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
        plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
        
        if (plane.x < plane.w / 2)
            plane.x = plane.w / 2;
        else if (plane.x > SCREEN_W - plane.w / 2)
            plane.x = SCREEN_W - plane.w / 2;
        if (plane.y < plane.h / 2)
            plane.y = plane.h / 2;
        else if (plane.y > SCREEN_H - plane.h / 2)
            plane.y = SCREEN_H - plane.h / 2;
        
        int i;
        /*for (i = 0; i < MAX_ENEMY; i++) {


        }*/

        //enemy boundary check
        for (i = 0; i < MAX_ENEMY; i++) {
            if (enemies[i].hidden == true)
                continue;
            enemies[i].x += enemies[i].vx;
            enemies[i].y += enemies[i].vy;
            if (enemies[i].y + enemies[i].h / 2 > SCREEN_H || enemies[i].x + enemies[i].w / 2 > SCREEN_W || enemies[i].x + enemies[i].w / 2 < 0){
                last_plane_OFB = al_get_time();
                enemies[i].hidden = true;
            }
        }

        //enemy get-bullet
        for (i = 0; i < MAX_ENEMY; i++) {

            for (int j = 0; j < plane.bullet; j++) {
                if (!enemies[i].hidden&&!bullets[j].hidden &&((pnt_in_rect(bullets[j].x + bullets[j].w / 2, bullets[j].y + bullets[j].h / 2, enemies[i].x, enemies[i].y, enemies[j].w, enemies[j].h))\
                    || pnt_in_rect(bullets[j].x + bullets[j].w / 2, bullets[j].y - bullets[j].h / 2, enemies[i].x, enemies[i].y, enemies[j].w, enemies[j].h)\
                    || pnt_in_rect(bullets[j].x - bullets[j].w / 2, bullets[j].y + bullets[j].h / 2, enemies[i].x, enemies[i].y, enemies[j].w, enemies[j].h)\
                    || pnt_in_rect(bullets[j].x - bullets[j].w / 2, bullets[j].y - bullets[j].h / 2, enemies[i].x, enemies[i].y, enemies[j].w, enemies[j].h))){
                    //enemies_get_shot[i]++;
                    enemies[i].life--;
                    bullets[j].hidden = true;
                    if (enemies[i].life <= 0) {
                        if (!al_play_sample(explosion_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &explosion_bgm_id))
                            game_log("failed to play audio (explosion_bgm)");
                        score+=enemies[i].type;
                        enemies[i].life = 2;
                        enemies[i].hidden = true;

                    }
                    else {
                        if (!al_play_sample(shot_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &shot_bgm_id))
                            game_log("failed to play audio (shot_bgm)");
                    }
                }
            }
            if (!enemies[i].hidden && enemies[i].y > 505) {
                blood -= 10;
                enemies[i].hidden = true;
                if (!al_play_sample(explosion_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &explosion_bgm_id))
                    game_log("failed to play audio (explosion_bgm)");
                enemies[i].life = 2;
                //int Max_bld=MAX_BLOOD;
                if (blood <= 30 && blood > 20 &&step==0) {
                    step++;
                    start_img_background = load_bitmap_resized("step1.jpg", SCREEN_W, SCREEN_H);
                    if (!start_img_background)
                        game_abort("failed to load image: step1.jpg");
                }
                else if (blood <= 20 && blood > 10 &&step==1) {
                    step++;
                    start_img_background = load_bitmap_resized("step2.jpg", SCREEN_W, SCREEN_H);
                    if (!start_img_background)
                        game_abort("failed to load image: step2.jpg");
                }
                else if (blood <= 10 && blood > 0 && step == 2) {
                    step++;
                    start_img_background = load_bitmap_resized("step3.jpg", SCREEN_W, SCREEN_H);
                    if (!start_img_background)
                        game_abort("failed to load image: step3.jpg");
                }
                if (blood <= 0) {
                    end = 3;
                    al_stop_sample(&beep_bgm_id);
                    game_log("stop audio (bgm)");
                    if (!al_play_sample(lose_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &lose_bgm_id))
                        game_log("failed to play audio (bgm)");
                    game_change_scene(SCENE_END3);
                }
            }
        }   
        for (i = 0; i < MAX_ENEMY; i++) {
            if (!bullets[i].hidden && (pnt_in_rect(enemies[i].x + enemies[i].w / 2, enemies[i].y + enemies[i].h / 2, plane.x, plane.y, plane.w, plane.h))\
                || (pnt_in_rect(enemies[i].x + enemies[i].w / 2, enemies[i].y - enemies[i].h / 2, plane.x, plane.y, plane.w, plane.h))\
                || (pnt_in_rect(enemies[i].x - enemies[i].w / 2, enemies[i].y + enemies[i].h / 2, plane.x, plane.y, plane.w, plane.h))\
                || (pnt_in_rect(enemies[i].x - enemies[i].w / 2, enemies[i].y - enemies[i].h / 2, plane.x, plane.y, plane.w, plane.h))) {
                plane.life--;
                if (!al_play_sample(explosion_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &explosion_bgm_id))
                    game_log("failed to play audio (explosion_bgm)");
                if (plane.life == 1) {
                    change = 1;
                }
                enemies[i].life = 0;
                enemies[i].hidden = true;
                //game_log("%d\n", plane.life);
                if (plane.life == 0) {
                    end = 2;
                    al_stop_sample(&beep_bgm_id);
                    game_log("stop audio (bgm)");
                    if (!al_play_sample(lose_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &lose_bgm_id))
                        game_log("failed to play audio (bgm)");
                    game_change_scene(SCENE_END2);
                }
            }
        }
        //
        for (i = 0; i < plane.bullet; i++) {
            if (bullets[i].hidden == true)
                continue;
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;
            if (bullets[i].y + bullets[i].h/2 < 0)
                bullets[i].hidden = true;
        }

        
        double now = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
            for (i = 0; i < plane.bullet; i++) {
                if (bullets[i].hidden==true) {
                    last_shoot_timestamp = now;
                    bullets[i].hidden = false;
                    bullets[i].x = plane.x;
                    bullets[i].y = plane.y - plane.h/2;
                    if (!al_play_sample(laser_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &laser_bgm_id))
                        game_log("failed to play audio (laser_bgm)");
                    break;
                }
            }
        }
        //enemy plane relive
        for (i = 0; i < MAX_ENEMY; i++) {
            double now = al_get_time();
            if (enemies[i].hidden == true && now - last_plane_OFB >= MAX_COOLDOWN+0.3) {
                int ran = rand() % 4;
                enemies[i].hidden = false;
                enemies[i].img = img_enemy[ran];
                enemies[i].type = ran + 1;
                enemies[i].w = al_get_bitmap_width(enemies[i].img);
                enemies[i].h = al_get_bitmap_height(enemies[i].img);
                enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
                enemies[i].y = 80;
                enemies[i].life = 2;
            }
        }
        //
        if (cur_length <= 3&&first==1) {
            first = 0;
            if (!al_play_sample(beep_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &beep_bgm_id))
                game_abort("failed to play audio (beep_bgm)");
        }
        else if (cur_length <= 0) {
            //al_draw_text(font_pirulen_60, al_map_rgb(41, 36, 33), SCREEN_W / 2 - 100, SCREEN_H/2, 0, "GAMEOVER");
            end = 1;
            al_stop_sample(&beep_bgm_id);
            game_log("stop audio (bgm)");
            if (!al_play_sample(win_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &win_bgm_id))
                game_log("failed to play audio (beep_bgm)");
            game_change_scene(SCENE_END1);
        }

    }
    
}

void game_draw(void) {
    if (active_scene == SCENE_MENU) {
        al_draw_bitmap(main_img_background, 0, 0, 0);
        //al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Space Shooter");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter key to start");
        
        if (pnt_in_rect(mouse_x, mouse_y, 665, 515, 150, 150))
            al_draw_bitmap(img_intro2, 590, 440, 0);
        else
            al_draw_bitmap(img_intro1, 590, 440, 0);
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 29, 29, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    } else if (active_scene == SCENE_START) {
        int i,time=MAX_TIME;
        al_draw_bitmap(start_img_background, 0, 0, 0);
        
        al_draw_textf(font_pirulen_32, al_map_rgb(250, 250, 250), SCREEN_W / 2 - 225, 30, 0, "YOUR Score:%4d", score);
        al_draw_text(font_pirulen_24, al_map_rgb(250, 250, 250), 450- time*11, 90, 0, "Time left: ");
        al_draw_text(font_pirulen_24, al_map_rgb(250, 250, 250), 150, SCREEN_H - 50, 0, "EARTH's HP: ");
        for (i = 0; i < blood; i++) {
            al_draw_bitmap(img_blood,380 + i * 7, SCREEN_H-53, 0);
        }
        for (i = 0; i < cur_length; i++) {
            al_draw_bitmap(start_img_loading, 680 + i * 11-time*11, 97, 0);
        }
        for (i = 0; i < plane.life; i++) {
            al_draw_bitmap(img_heart, 650 + i * 45, 25, 0);
        }
        for (i = 0; i < plane.bullet; i++)
            draw_movable_object(bullets[i]);
        //for (i = 0; i < 4; i++)
        //    printf("%d\n", bullets[i].hidden);
        draw_movable_object(plane);
        for (i = 0; i < MAX_ENEMY; i++)
            draw_movable_object(enemies[i]);
        
    }
    
    else if (active_scene == SCENE_SETTINGS) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(menu_img_background, 0, 0, 0);
        
        if (pnt_in_rect(mouse_x, mouse_y, 255, 350, 150, 150))
            al_draw_bitmap(img_plus2, 180, 275, 0);
        else
            al_draw_bitmap(img_plus, 180, 275, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 555, 350, 150, 150))
            al_draw_bitmap(img_minus2, 480, 275, 0);
        else
            al_draw_bitmap(img_minus, 480, 275, 0);
        
        al_draw_text(font_pirulen_60, al_map_rgb(41, 36, 33), SCREEN_W / 2-175, 100, 0 , "Setting");
        al_draw_textf(font_pirulen_24, al_map_rgb(41, 36, 33), SCREEN_W / 2-225 , SCREEN_H/2-75, 0, "YOUR current Volume: %.0f",volume*5);
        al_draw_text(font_pirulen_24, al_map_rgb(41, 36, 33), SCREEN_W / 2 - 175, 450, 0, "Volume Range: 0 - 10");
        al_draw_text(font_pirulen_24, al_map_rgb(250, 250, 250), 20, SCREEN_H - 50, 0, "Press backspace key to menu");
       
    }
    else if (active_scene == SCENE_PICK) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(pick_img_background, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(250, 250, 250), SCREEN_W / 2 - 300, 50, 0, "Choose Your Character");
        al_draw_text(font_pirulen_32, al_map_rgb(250, 250, 250), 50, SCREEN_H - 100, 0, "Press enter key to continue");
        al_draw_bitmap(pick_img_plane1, 120, 140, 0);
        al_draw_bitmap(pick_img_plane2, 320, 140, 0);
        al_draw_bitmap(pick_img_plane3, 520, 140, 0);
    }
    else if (active_scene == SCENE_END1) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(black_background, 0, 0, 0);
        al_draw_bitmap(img_timeup, 0 , 0, 0);
        if (time_up_size > 250) {
            al_draw_textf(font_pirulen_32, al_map_rgb(250, 250, 250), 20, SCREEN_H - 150, 0, "Your final score is %d point(s)",score);
            al_draw_text(font_pirulen_24, al_map_rgb(250, 250, 250), 110, SCREEN_H - 75, 0, "Press enter key to continue");
        }
        if (time_up_size < 380) {
            time_up_size += 2;
            img_timeup = load_bitmap_resized("timesup.png", time_up_size * 2, time_up_size);
        }
    }
    else if (active_scene == SCENE_END2) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(end2_background, 0, 0, 0);
        al_draw_textf(font_pirulen_32, al_map_rgb(250, 250, 250), 20, SCREEN_H - 150, 0, "Your final score is %d point(s)", score);
        al_draw_text(font_pirulen_24, al_map_rgb(250, 250, 250), 110, SCREEN_H - 75, 0, "Press enter key to continue");
    }
    else if (active_scene == SCENE_END3) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(end3_background, 0, 0, 0);
        al_draw_text(font_pirulen_100, al_map_rgb(250, 250, 250), 20, 150, 0, "GAMEOVER");
        al_draw_textf(font_pirulen_32, al_map_rgb(250, 250, 250), 30, SCREEN_H - 200, 0, "Your final score is %d point(s)", score);
        al_draw_text(font_pirulen_24, al_map_rgb(250, 250, 250), 110, SCREEN_H - 75, 0, "Press enter key to continue");
    }
    else if (active_scene == SCENE_INTRO1) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(img_pg_intro1, 0, 0, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 530, 60, 60))
            al_draw_bitmap(img_next_pg2, 700, 500, 0);
        else
            al_draw_bitmap(img_next_pg1, 700, 500, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 60, 60, 60))
            al_draw_bitmap(img_home_pg2, 700, 30, 0);
        else
            al_draw_bitmap(img_home_pg1, 700, 30, 0);

    }
    else if (active_scene == SCENE_INTRO2) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(img_pg_intro2, 0, 0, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 60, 530, 60, 60))
            al_draw_bitmap(img_pre_pg2, 30, 500, 0);
        else
            al_draw_bitmap(img_pre_pg1, 30, 500, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 530, 60, 60))
            al_draw_bitmap(img_next_pg2, 700, 500, 0);
        else
            al_draw_bitmap(img_next_pg1, 700, 500, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 60, 60, 60))
            al_draw_bitmap(img_home_pg2, 700, 30, 0);
        else
            al_draw_bitmap(img_home_pg1, 700, 30, 0);

    }
    else if (active_scene == SCENE_INTRO3) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(img_pg_intro3, 0, 0, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 60, 530, 60, 60))
            al_draw_bitmap(img_pre_pg2, 30, 500, 0);
        else
            al_draw_bitmap(img_pre_pg1, 30, 500, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 530, 60, 60))
            al_draw_bitmap(img_next_pg2, 700, 500, 0);
        else
            al_draw_bitmap(img_next_pg1, 700, 500, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 60, 60, 60))
            al_draw_bitmap(img_home_pg2, 700, 30, 0);
        else
            al_draw_bitmap(img_home_pg1, 700, 30, 0);

    }
    else if (active_scene == SCENE_INTRO4) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(img_pg_intro4, 0, 0, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 60, 530, 60, 60))
            al_draw_bitmap(img_pre_pg2, 30, 500, 0);
        else
            al_draw_bitmap(img_pre_pg1, 30, 500, 0);
        if (pnt_in_rect(mouse_x, mouse_y, 730, 60, 60, 60))
            al_draw_bitmap(img_home_pg2, 700, 30, 0);
        else
            al_draw_bitmap(img_home_pg1, 700, 30, 0);

    }
    al_flip_display();
}

void game_destroy(void) {
    // Destroy everything you have created.
    // Free the memories allocated by malloc or allegro functions.
    // Destroy shared resources.
    int i;
    al_destroy_font(font_pirulen_32);
    al_destroy_font(font_pirulen_24);

    /* Menu Scene resources*/
    al_destroy_bitmap(main_img_background);
    al_destroy_sample(main_bgm);
    
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);
    al_destroy_bitmap(img_intro1);
    al_destroy_bitmap(img_intro2);
    al_destroy_bitmap(img_plus);
    al_destroy_bitmap(img_plus2);
    al_destroy_bitmap(img_minus);
    al_destroy_bitmap(img_minus2);
    al_destroy_bitmap(start_img_loading);
    al_destroy_bitmap(pick_img_background);
    al_destroy_bitmap(start_img_plane1);
    al_destroy_bitmap(start_img_plane2);
    al_destroy_bitmap(start_img_plane3);
    al_destroy_bitmap(pick_img_plane1);
    al_destroy_bitmap(pick_img_plane2);
    al_destroy_bitmap(pick_img_plane3);
    /* Start Scene resources*/
    al_destroy_bitmap(start_img_background);
    al_destroy_bitmap(img_blood);
    al_destroy_bitmap(start_img_plane);
    al_destroy_bitmap(start_img_enemy);
    al_destroy_bitmap(black_background);
    al_destroy_bitmap(img_timeup);
    al_destroy_bitmap(img_pg_intro1);
    al_destroy_bitmap(img_pg_intro2);
    al_destroy_bitmap(img_pg_intro3);
    al_destroy_bitmap(img_pg_intro4);
    al_destroy_bitmap(img_pre_pg1);
    al_destroy_bitmap(img_pre_pg2);
    al_destroy_bitmap(img_next_pg1);
    al_destroy_bitmap(img_next_pg2);
    al_destroy_bitmap(img_home_pg1);
    al_destroy_bitmap(img_home_pg2);
    al_destroy_bitmap(end2_background);
    al_destroy_bitmap(end3_background);
    al_destroy_sample(start_bgm);
    al_destroy_sample(pick_bgm);
    al_destroy_sample(setting_bgm);
    al_destroy_sample(beat_bgm);
    al_destroy_sample(beep_bgm);
    al_destroy_sample(air_bgm);
    al_destroy_sample(laser_bgm);
    al_destroy_sample(shot_bgm);
    al_destroy_sample(explosion_bgm);
    al_destroy_sample(lose_bgm);
    al_destroy_sample(flip_bgm);

    for (i = 0; i < 9; i++) {
        al_destroy_bitmap(img_explosion[i]);
    }
    
    al_destroy_bitmap(img_bullet);
    for (i = 0; i < 4; i++) {
        al_destroy_bitmap(img_enemy[i]);
    }

    al_destroy_timer(game_update_timer);
    al_destroy_event_queue(game_event_queue);
    al_destroy_display(game_display);
    free(mouse_state);
}

void game_change_scene(int next_scene) {
    game_log("Change scene from %d to %d", active_scene, next_scene);
    
    if (active_scene == SCENE_MENU) {
        al_stop_sample(&main_bgm_id);
        game_log("stop audio (bgm)");

    } else if (active_scene == SCENE_START) {
        al_stop_sample(&start_bgm_id);
        game_log("stop audio (bgm)");
        al_stop_sample(&beat_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_SETTINGS) {
        al_stop_sample(&setting_bgm_id);
        game_log("stop audio (bgm)");

    }
    else if (active_scene == SCENE_PICK) {
        al_stop_sample(&pick_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_END1) {
        al_stop_sample(&win_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_END2) {
        al_stop_sample(&lose_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_END3) {
        al_stop_sample(&lose_bgm_id);
        game_log("stop audio (bgm)");
    }
    if ((active_scene == SCENE_INTRO1 || active_scene == SCENE_INTRO2 || active_scene == SCENE_INTRO3 || active_scene == SCENE_INTRO4) && next_scene==SCENE_MENU) {
        al_stop_sample(&pick_bgm_id);
        game_log("stop audio (bgm)");
    }
    active_scene = next_scene;
    
    if (active_scene == SCENE_MENU) {
        if (!al_play_sample(main_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
            game_abort("failed to play audio (bgm)");
    } else if (active_scene == SCENE_START) {
        int i;
        change = 1;
        first = 1;
        blood = MAX_BLOOD;
        score = 0;
        step = 0;
        end = 0;
        time_up_size = 1.f;
        start_img_background = load_bitmap_resized("galaxy.jpg", SCREEN_W, SCREEN_H);
        if (!start_img_background)
            game_abort("failed to load image: galaxy.jpg");
        cur_length = MAX_TIME;
        plane.img = start_img_plane;
        if (character == 0) {
            plane.img = start_img_plane1;
            plane.life = 3;
            plane.bullet = 4;
            img_bullet = al_load_bitmap("bullet1.png");
            if (!img_bullet)
                game_abort("failed to load image: bullet1.png");
        }
        else if (character == 1) {
            plane.img = start_img_plane2;
            plane.life = 2;
            plane.bullet = 4;
            img_bullet = al_load_bitmap("bullet2.png");
            if (!img_bullet)
                game_abort("failed to load image: bullet2.png");
        }
        else {
            plane.img = start_img_plane3;
            plane.life = 3;
            plane.bullet = 5;
            img_bullet = al_load_bitmap("bullet3.png");
            if (!img_bullet)
                game_abort("failed to load image: bullet3.png");
        }
        plane.x = 400;
        plane.y = 500;
        plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);
        plane.explosion = false;
        for (i = 0; i < MAX_ENEMY; i++) {
            int ran = rand() % 4;
            enemies[i].img = img_enemy[ran];
            enemies[i].type = ran + 1;
            enemies[i].w = al_get_bitmap_width(enemies[i].img);
            enemies[i].h = al_get_bitmap_height(enemies[i].img);
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = 80;
            enemies[i].life = 2;
            enemies[i].explosion = false;
        }
        
        for (i = 0; i < plane.bullet; i++) {
            bullets[i].img = img_bullet;
            bullets[i].w = al_get_bitmap_width(bullets[i].img);
            //printf("%d\n", bullets[i].w);
            bullets[i].h = al_get_bitmap_height(bullets[i].img);
            bullets[i].vx = 0;
            bullets[i].vy = -6;
            bullets[i].hidden = true;
        }
        if (!al_play_sample(start_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
            game_abort("failed to play audio (bgm)");
    }
    else if (active_scene == SCENE_SETTINGS) {
        if (!al_play_sample(setting_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
            game_abort("failed to play audio (bgm)");
    }
    else if (active_scene == SCENE_PICK) {
        if (!al_play_sample(pick_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &pick_bgm_id))
            game_abort("failed to play audio (bgm)");
    }
}

void on_key_down(int keycode) {
    int check = 0;
    if (active_scene == SCENE_MENU) {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_PICK);
    }
    
    else if (active_scene == SCENE_SETTINGS) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_MENU);
    }
    else if (active_scene == SCENE_PICK) {
        if (keycode == ALLEGRO_KEY_LEFT && character == 1) {
            character--;
            pick_img_plane1 = load_bitmap_resized("plane1.png", 150, 250);
            pick_img_plane2 = load_bitmap_resized("plane2_blur.png", 150, 250);
            check = 1;
            plane_speed = 1.5;
        }
        else if (keycode == ALLEGRO_KEY_LEFT && character == 2) {
            character--;
            pick_img_plane2 = load_bitmap_resized("plane2.png", 150, 250);
            pick_img_plane3 = load_bitmap_resized("plane3_blur.png", 150, 250);
            check = 1;
            plane_speed = 2;
        }
        else if (keycode == ALLEGRO_KEY_RIGHT && character == 0) {
            character++;
            pick_img_plane1 = load_bitmap_resized("plane1_blur.png", 150, 250);
            pick_img_plane2 = load_bitmap_resized("plane2.png", 150, 250);
            check = 1;
            plane_speed = 2;
        }
        else if (keycode == ALLEGRO_KEY_RIGHT && character == 1) {
            character++;
            pick_img_plane2 = load_bitmap_resized("plane2_blur.png", 150, 250);
            pick_img_plane3 = load_bitmap_resized("plane3.png", 150, 250);
            check = 1;
            plane_speed = 1;
        }
        if (check == 1) {
            if (!al_play_sample(air_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &air_bgm_id))
                game_abort("failed to play audio (bgm)");
        }
        if (keycode == ALLEGRO_KEY_ENTER) {
            if (!al_play_sample(confirm_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &confirm_bgm_id))
                game_abort("failed to play audio (bgm)");
            game_change_scene(SCENE_START);
        }
    }
    else if (active_scene == SCENE_END1|| active_scene == SCENE_END2|| active_scene == SCENE_END3) {
        if (keycode == ALLEGRO_KEY_ENTER) {
            game_change_scene(SCENE_MENU);
        }
    }
}

void on_mouse_down(int btn, int x, int y) {
    
    if (active_scene == SCENE_MENU) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W - 29, 29, 38, 38))
            // button location (SCREEN_W - 48, 10, 38, 38).
                game_change_scene(SCENE_SETTINGS);
            if (pnt_in_rect(x, y, 665, 515, 150, 150)) {
                // button location (SCREEN_W - 48, 10, 38, 38).
                if (!al_play_sample(pick_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &pick_bgm_id))
                    game_abort("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO1);
            }
        }
    }
    else if (active_scene == SCENE_SETTINGS) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, 255, 350, 150, 150)&&volume<2) {
                volume += 0.2;
                al_stop_sample(&setting_bgm_id);
                game_log("stop audio (bgm)");
                if (!al_play_sample(setting_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
                    game_abort("failed to play audio (bgm)");
            }
            else if (pnt_in_rect(x, y, 555, 350, 150, 150)&&volume-0.2>0) {
                volume -= 0.2;
                al_stop_sample(&setting_bgm_id);
                game_log("stop audio (bgm)");
                if (!al_play_sample(setting_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
                    game_abort("failed to play audio (bgm)");
            }
        }
    }
    else if (active_scene == SCENE_INTRO1) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, 730, 530, 60, 60)) {
                if (!al_play_sample(flip_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &flip_bgm_id))
                    game_log("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO2);
            }
            if (pnt_in_rect(x, y, 730, 60, 60, 60))
                game_change_scene(SCENE_MENU);
        }
    }
    else if (active_scene == SCENE_INTRO2) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, 60, 530, 60, 60)) {
                if (!al_play_sample(flip_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &flip_bgm_id))
                    game_log("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO1);
            }
            if (pnt_in_rect(x, y, 730, 530, 60, 60)) {
                if (!al_play_sample(flip_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &flip_bgm_id))
                    game_log("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO3);
            }
            if (pnt_in_rect(x, y, 730, 60, 60, 60))
                game_change_scene(SCENE_MENU);
        }
    }
    else if (active_scene == SCENE_INTRO3) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, 60, 530, 60, 60)) {
                if (!al_play_sample(flip_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &flip_bgm_id))
                    game_log("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO2);
            }
            if (pnt_in_rect(x, y, 730, 530, 60, 60)) {
                if (!al_play_sample(flip_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &flip_bgm_id))
                    game_log("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO4);
            }
            if (pnt_in_rect(x, y, 730, 60, 60, 60))
                game_change_scene(SCENE_MENU);
        }
    }
    else if (active_scene == SCENE_INTRO4) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, 60, 530, 60, 60)) {
                if (!al_play_sample(flip_bgm, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &flip_bgm_id))
                    game_log("failed to play audio (bgm)");
                game_change_scene(SCENE_INTRO3);
            }
            if (pnt_in_rect(x, y, 730, 60, 60, 60))
                game_change_scene(SCENE_MENU);
        }
    }
}

void draw_movable_object(MovableObject obj) {
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp)
        game_abort("failed to load image: %s", filename);
    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    if (!resized_bmp)
        game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    game_log("resized image: %s", filename);

    return resized_bmp;
}

// [HACKATHON 3-3]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    
    if (px <= x + (w / 2) && px >= x - (w / 2) && py >= y - (h / 2) && py <= y + (h / 2)) {
        
        return true;
    }
    else {
        
        return false;
    }
}



void game_abort(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
    fprintf(stderr, "error occured, exiting after 2 secs");
    // Wait 2 secs before exiting.
    al_rest(2);
    // Force exit program.
    exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
    static bool clear_file = true;
    // Write log to file for later debugging.
    FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
    if (pFile) {
        va_list arg2;
        va_copy(arg2, arg);
        vfprintf(pFile, format, arg2);
        va_end(arg2);
        fprintf(pFile, "\n");
        fclose(pFile);
    }
    vprintf(format, arg);
    printf("\n");
    clear_file = false;
#endif
}