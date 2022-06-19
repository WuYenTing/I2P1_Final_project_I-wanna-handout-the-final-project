#include "GameWindow.h"

bool draw = false;
int window = 1;

const char *title = "I wanna handout the final project!";

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_SAMPLE_INSTANCE *menu_Sound;

int Game_establish() {
    int msg = 0;

    game_init();

    game_begin();

    while ( msg != GAME_TERMINATE ) {
        msg = game_run();
        if ( msg == GAME_TERMINATE )
            printf( "Game Over\n" );
    }

    game_destroy();
    return 0;
}

void game_init() {
    printf( "Game Initializing...\n" );
    al_init();
    // init audio
    al_install_audio();
    al_init_acodec_addon();
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    // create event queue
    event_queue = al_create_event_queue();
    // Initialize Allegro settings
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon
    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event
    // Register event
    al_register_event_source(event_queue, al_get_display_event_source( display ));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    fps  = al_create_timer( 1.0 / FPS );
    al_register_event_source(event_queue, al_get_timer_event_source( fps )) ;
    // initialize the icon on the display
    ALLEGRO_BITMAP *icon = al_load_bitmap("./image/character01/char01_move1.png");
    al_set_display_icon(display, icon);
}

void game_begin() {
    // Load sound
    song = al_load_sample("./sound/menu_music.wav");
    al_reserve_samples(20);
    menu_Sound = al_create_sample_instance(song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(menu_Sound, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(menu_Sound, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(menu_Sound, 1) ;
    al_play_sample_instance(menu_Sound);
    al_start_timer(fps);
    // initialize the menu before entering the loop
    menu_init();

}
void game_update(){
    if( judge_next_window ==1 && window ==1 ){
            // not back menu anymore, therefore destroy it
            menu_destroy();
            al_destroy_sample_instance(menu_Sound);
            // initialize next scene
            select_charater_init();
            judge_next_window = false;
            window = 2;
    }else if( judge_next_window ==1 && window == 2 ){
            select_charater_destroy();
            game_scene_init();
            judge_next_window = false;
            window = 3;
    }
    else if(judge_next_window ==0 && window == 3){
        charater_update();
        doc_update();
        cof_update();
        beer_update();
        pills_update();
    }
}
int process_event(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // process the event of other component
    if( window == 1 ){
        menu_process(event);
    }else if( window == 2 ){
        select_charater_process(event);
    }else if( window == 3 ){
        charater_process(event);
        doc_process();
        cof_process();
        beer_process();
        pills_process();
    }

    // Shutdown our program
    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;
    else if(event.type == ALLEGRO_EVENT_TIMER)
        if(event.timer.source == fps)
            draw = true;
    if(draw) game_update();
    return 0;
}
void game_draw(){
    if( window == 1 ){
        menu_draw();
    }else if( window == 2 ){
        select_charater_draw();
    }else if( window == 3 ){
        game_scene_draw();
    }

    al_flip_display();
}
int game_run() {
    int error = 0;
    if( draw ){
        game_draw();
        draw = false;
    }
    if ( !al_is_event_queue_empty(event_queue) ) {
        error = process_event();
    }
    return error;
}

void game_destroy() {
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    game_scene_destroy();
}
