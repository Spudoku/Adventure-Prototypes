#include "engine.h"

#pragma optimize(on)
#pragma static-locals(on)




unsigned char p1_collisions;
jmp_buf start_location;
void engine_Boot(){
    // install drivers
    joy_install(&atrstd_joy);  

    //init graphics and sounds
    gfx_Init();
    init_sound();

    // initialize the entities
    engine_InitSingletons();     
}


void engine_InitSingletons(){
    
    playerConstructor();
    



    // test
    //debug manual assign for now
    playerEnt.playerEntity._worldCoords.x = 624;
    playerEnt.playerEntity._worldCoords.y = 560;
    playerEnt.player_LastPos.x = 624;
    playerEnt.player_LastPos.y = 560;

    chaliceEnt.chaliceEntity._worldCoords.x = 600;
    chaliceEnt.chaliceEntity._worldCoords.y = 560;
    dragon_Init(&dragonSingleton);  //TEMP
    dragon_TrackEntity(&dragonSingleton, &playerEnt.playerEntity);
    // TODO: semi random spawn locations?
    dragonSingleton.myEntity._worldCoords.x = 300;
    dragonSingleton.myEntity._worldCoords.y = 860;
    
    cameraConstructor(&playerEnt.playerEntity);

    chalice_constructor();

    chaliceEnt.chalice_destination.y = 608;
    chaliceEnt.chalice_destination.x = 568;
    chaliceEnt.chalice_minDistToDest = 8;
    // chalice_destination.x = 
    GTIA_WRITE.hitclr = 1; 
}

//should produce a final gamestate...
//the idea is to have an array of frametask ptrs to run in order
void engine_StateUpdate(){

    // this is generally just decrementing stuff
    sound_StateUpdate();

    playerEnt.playerEntity.frameTask(&(playerEnt.playerEntity));

    dragonSingleton.myEntity.frameTask(&(dragonSingleton.myEntity));
    chaliceEnt.chaliceEntity.frameTask(&chaliceEnt.chaliceEntity);
    
    camera.cameraEntity.frameTask(&(camera.cameraEntity));
}

void engine_Render(){
    camera.cameraEntity.renderer(&camera.cameraEntity); //camera goes first

    playerEnt.playerEntity.renderer(&(playerEnt.playerEntity));

    chaliceEnt.chaliceEntity.renderer(&(chaliceEnt.chaliceEntity));

    dragonSingleton.myEntity.renderer(&(dragonSingleton.myEntity));

    
};

//stub to demonstrate where events would be dispatched during each tick
void engine_EventDispatcher(){

    unsigned char temp_collisions;


    // if dragon collides with player...
    temp_collisions = player_to_player_collisions(TEMP_dragon_anticIndex);
    
    // if dragon collides with player avatar...
    if (collision_with_index(temp_collisions,TEMP_player_anticIndex)) {
        // sound_generic_buzz();
        dragon_OnCollision(&(dragonSingleton.myEntity), &playerEnt.playerEntity);
        player_OnCollide(&playerEnt.playerEntity, &(dragonSingleton.myEntity));
    }

    temp_collisions = player_to_player_collisions(TEMP_player_anticIndex);
    if (collision_with_index(temp_collisions,TEMP_item_anticIndex)) {
        player_pickup_item(&(chaliceEnt.chaliceEntity));
    }

    // player character collisions
    // currently this checks if any playfield is collided with
    temp_collisions = player_to_playfield_collisions(TEMP_player_anticIndex);
    if (temp_collisions) {
        if (!(collision_with_index(temp_collisions,3))) {
            player_OnCollide(&playerEnt.playerEntity, NULL);
        } else {
             orb_singleton.entity.OnCollision(NULL, NULL);
        }
        
    } 

    chalice_check_desintation();
   
    GTIA_WRITE.hitclr = 1; 
};


// play the sad sound when you get got
// for now, just reset the game
void end_game() {

    trigger_warm_reset();
}

// the 'good ending'
void end_game_good() {
    trigger_warm_reset();
}

// TODO: fix this!!!!!!!!!
void trigger_warm_reset(void) {
    // // 1. Set the Warmstart flag (WARMST) at 0x0008 to non-zero
    // *(unsigned char*)0x0008 = 0x01;

    // // 2. Jump to the OS reset vector. 
    // // On the Atari, the reset vector is at 0xE474.
    // // In cc65, we can use an assembly wrapper or a function pointer.
    // ((void (*)(void))0xE474)();
    // exit(0);
    longjmp(start_location, 1); // Jumps back to setjmp()
}

// need to point to this for RESET
void game_loop() {
    setjmp(start_location);
   
    engine_Boot();
    while (1) {

        
        //process gamestate
        engine_EventDispatcher();
        
        
    
        engine_StateUpdate();

        waitvsync();

        engine_Render();

        
        
    }
    
}

// following from Google Gemini
// TODO: test this to make sure it works!
void setup_reset_handler() {
// Disable interrupts while we mess with vectors
    __asm__("sei");

    // Set the low and high bytes of the reset vector ($0302)
    // to point to the cc65 startup code
    POKEW(0x0302, (unsigned int)game_loop);

    // Re-enable interrupts
    __asm__("cli");
}