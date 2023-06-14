#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "..\libs\include\MiniFB.h"
#include "..\libs\include\MiniFB_enums.h"

#define SOKOL_IMPL
#include "..\libs\include\sokol_log.h"
#include "..\libs\include\sokol_audio.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\libs\include\stb_image.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "draw.cpp"
#include "image.cpp"
#include "font.cpp"
#include "audio.cpp"
#include "input.cpp"


#define WINDOW_SIZE_X 400
#define WINDOW_SIZE_Y 400

#define WINDOW_FAC 2

#define FRAME_SX 200
#define FRAME_SY 200


#define WINDOW_SX (FRAME_SX * WINDOW_FAC)
#define WINDOW_SY (FRAME_SY * WINDOW_FAC)

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0xFF000000

#define COLOR_BG 0xFF0F1226

#define COLOR_BLUE 0xFF0000FF
#define COLOR_CYAN 0xFF00FFFF
#define COLOR_GREEN 0xFF00FF00
#define COLOR_RED 0xFFFF0000

#define COLOR_GAMEOVER 0xFFFF2E2E
#define COLOR_UI 0xFFFFD68F



#define ARR_LEN(arr) ((int) (sizeof(arr)/ sizeof(*arr)))
#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

#define STRINGIFY(a) str(a)
#define STRINGIFY_INTERNAL(a) #a

#define ANSI_RED "\e[31m"
#define ANSI_RESET "\e[0m"
#define ANSI_GRAY "\e[90m"

#define LOG(message) printf(ANSI_GRAY message ANSI_RESET  " (" __FILE__ ":" STRINGIFY_INTERNAL(__LINE__) ") \n")
#define LOG_ERROR(message) printf( ANSI_RED  "ERROR " message " (" __FILE__ ":" STRINGIFY_INTERNAL(__LINE__) ") \n" ANSI_RESET )


#define TILE_PX 10

#define TILE_GRASS 1
#define TILE_WATER 2
#define TILE_ROCKSOLID 3
#define TILE_SNOWY 4
#define TILE_SMALLROCK 5
#define TILE_LADDER 6
#define TILE_DIAMAND 7
#define TILE_LIMIT 8
// ou enum 
/*
enum tile_type_e {
    GRASS = 0,
    WATER = 1,
};*/

int nbr_de_coup = 0;

struct player_t {
    int posx; 
    int posy; 
    img_t img;
};

player_t player = {
    .posx = 2,
    .posy = 7,
};

void reset_player_pos(player_t* player) {
    player->posx = 2;
    player->posy = 7;
    nbr_de_coup = 0;
}

void draw_payer(player_t player) {
    DrawImageAlpha(player.posx * TILE_PX + 2, player.posy * TILE_PX, player.img);
}

bool isGrounded(player_t player, int* tiles) {
    if (tiles[(player.posy + 1) * 20 + player.posx] != 0 && tiles[(player.posy + 1) * 20 + player.posx] != TILE_LADDER && tiles[(player.posy + 1) * 20 + player.posx] != TILE_DIAMAND && tiles[(player.posy + 1) * 20 + player.posx] != TILE_LIMIT) {
        return true;
    }
    else {
        return false;
    }
}

bool onLadder(player_t player, int* tiles) {
    return (tiles[player.posy * 20 + player.posx] == TILE_LADDER);
}

bool isEmptyTiles(int posx, int posy, int* tiles){
    if (tiles[posy * 20 + posx] == TILE_LADDER || tiles[posy * 20 + posx] == 0 || tiles[posy * 20 + posx] == TILE_LIMIT || tiles[posy * 20 + posx] == TILE_DIAMAND) {
        return true;
    }
    else {
        return false;
    }
}

//Undo system
bool tiles_modified = false;



int tiles[20 * 20]; 

uint32_t* buffer;

void resize_bitmap(uint32_t* dest, int dest_sx, int dest_sy, uint32_t* src, int src_sx, int src_sy)
{
    for (int y = 0; y < dest_sy; y++) {
        for (int x = 0; x < dest_sx; x++) {
            int src_x = x * src_sx / dest_sx;
            int src_y = y * src_sy / dest_sy;
            dest[y * dest_sx + x] = src[src_y * src_sx + src_x];
        }
    }
}

void DrawAllWindowPixel(uint32_t* buffer, uint32_t color) {
    for (int i = 0; i < FRAME_SX * FRAME_SY; i++)
    {
        buffer[i] = color;
    }
}


enum ser_mode {
    SER_MODE_READ = 0,
    SER_MODE_WRITE = 1,
};

struct serializer_t {
    ser_mode mode;
    int buffer_capacity;
    int buffer_used; //also buffer read position
    uint8_t* buffer;
};

void serialize(serializer_t* ser, void* ptr, size_t size) {
    if (ser->mode == SER_MODE_READ) {
        size_t remaining_bytes = ser->buffer_capacity - ser->buffer_used;
        if (remaining_bytes < size) {
            LOG_ERROR("Serializer tries to read past end of buffer");
            exit(-1);
        }
        memcpy(ptr, ser->buffer + ser->buffer_used, size);
        ser->buffer_used += size;
        //fread(ptr, size, 1, file);
    }
    else {
        size_t remaining_capacity = ser->buffer_capacity - ser->buffer_used;
        if (remaining_capacity < size) {
            LOG_ERROR("Serializer out of memory");
            exit(-1);
        }
        memcpy(ser->buffer + ser->buffer_used, ptr, size);
        ser->buffer_used += size;
        //fwrite(ptr, size, 1, file);
    }
}

void serialize_level(serializer_t* ser) {
    //FILE* f = fopen("save.level", ser->mode == SER_MODE_READ ? "rb" : "wb");
    serialize(ser, tiles, sizeof(tiles));
}

void save() {
    LOG("Save");

    serializer_t ser = {};
    ser.mode = SER_MODE_WRITE;
    ser.buffer_capacity = 4000;
    ser.buffer = (uint8_t*)malloc(ser.buffer_capacity);

    serialize_level(&ser);

    FILE* f = fopen("save.level", "wb");
    fwrite(ser.buffer, ser.buffer_used, 1, f);
    fclose(f);
}

void save_on_file(const char *txt) {
    LOG("Save");

    serializer_t ser = {};
    ser.mode = SER_MODE_WRITE;
    ser.buffer_capacity = 4000;
    ser.buffer = (uint8_t*)malloc(ser.buffer_capacity);

    serialize_level(&ser);

    FILE* f = fopen(txt, "wb");
    fwrite(ser.buffer, ser.buffer_used, 1, f);
    fclose(f);
}


void load() {
    LOG("Load");
    span_t file_data = load_entire_file("save.level");
    serializer_t ser = {};
    ser.mode = SER_MODE_READ;
    ser.buffer = file_data.ptr;
    ser.buffer_capacity = file_data.size;
    ser.buffer_used = 0; // just to be explicit;
    serialize_level(&ser);
}

void load_on_file(const char* txt) {
    LOG("Load");
    span_t file_data = load_entire_file(txt);
    serializer_t ser = {};
    ser.mode = SER_MODE_READ;
    ser.buffer = file_data.ptr;
    ser.buffer_capacity = file_data.size;
    ser.buffer_used = 0; // just to be explicit;
    serialize_level(&ser);
}



int history_steps_count = 0;
serializer_t history_steps[256];
//std::vector<serializer_t> history_steps;


void history_commit() {
    serializer_t ser = {};
    ser.mode = SER_MODE_WRITE;
    ser.buffer_capacity = 4000;
    ser.buffer = (uint8_t*)malloc(ser.buffer_capacity);

    serialize_level(&ser);
    history_steps[history_steps_count++] = ser;
}

void undo() {
    if (history_steps_count <= 0) {
        LOG("Nothing to Undo");
        return;
    }

    if (history_steps_count >= ARR_LEN(history_steps) - 1) {
        LOG("Warning undo stack is full !!!");
        return;
    }

    serializer_t ser = history_steps[--history_steps_count];
    ser.mode = SER_MODE_READ;
    ser.buffer_capacity = ser.buffer_used;
    ser.buffer_used = 0;

    serialize_level(&ser);
    //free(ser.buffer);
}

bool editmode = false;
int selectedTiles = 0;

int last_lvl_load = 0;
bool isPlayingPickupSound = false;

img_t rocksolid;
img_t snowyground;
img_t smallrock;
img_t ladder;
img_t diamand;
img_t limit;
img_t player_img;
img_t font;
img_t background;

sound_clip_t moving_sound;
sound_clip_t background_sound;
sound_clip_t pickup_sound;

void load_assets() {
    LoadImage(rocksolid, "assets/rocksolid.png");
    LoadImage(snowyground, "assets/snowyground.png");
    LoadImage(smallrock, "assets/smallrock.png");
    LoadImage(ladder, "assets/ladder.png");
    LoadImage(diamand, "assets/diamand.png");
    LoadImage(limit, "assets/limit.png");
    LoadImage(player_img, "assets/player.png");
    player.img = player_img;

    LoadImage(font, "assets/font_map.png");
    LoadImage(background, "assets/background.png");

    moving_sound = load_sound_clip("assets/move_sound.wav");
    background_sound = load_sound_clip("assets/The_Empire.wav");
    pickup_sound = load_sound_clip("assets/pickup.wav");
}

void undo_input() {
    if (was_key_just_pressed(KB_KEY_Z) && is_key_down(KB_KEY_LEFT_CONTROL)) {
        undo();
    }

    if (mouse_was_just_released(MOUSE_LEFT) && tiles_modified) {
        history_commit();
        tiles_modified = false;
    }
}

void draw_tiles() {
    //Draw Side for Edit mode palette
    for (int posy = 0; posy < 20; posy++) {
        tiles[posy * 20 + 17] = 0;
        tiles[posy * 20 + 18] = 0;
        tiles[posy * 20 + 19] = 0;
    }

    //draw palette 
    if (editmode) {
        tiles[6 * 20 + 18] = TILE_ROCKSOLID;
        tiles[8 * 20 + 18] = TILE_SNOWY;
        tiles[12 * 20 + 18] = TILE_SMALLROCK;
        tiles[14 * 20 + 18] = TILE_LADDER;
        tiles[16 * 20 + 18] = TILE_DIAMAND;
        tiles[18 * 20 + 18] = TILE_LIMIT;
    }

    undo_input();

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            rect_t tile_rect = (rect_t){ x * TILE_PX, y * TILE_PX, TILE_PX, TILE_PX };
            //draw tile 
            int tile_type = tiles[y * 20 + x];
            if (tile_type == TILE_ROCKSOLID) {
                DrawImageAlpha(tile_rect.x, tile_rect.y, rocksolid);
            }
            else if (tile_type == TILE_WATER) {
                DrawRect(tile_rect, COLOR_BLUE);
            }
            else if (tile_type == TILE_SNOWY) {
                DrawImageAlpha(tile_rect.x, tile_rect.y, snowyground);
            }
            else if (tile_type == TILE_SMALLROCK) {
                DrawImageAlpha(tile_rect.x, tile_rect.y, smallrock);
            }
            else if (tile_type == TILE_LADDER) {
                DrawImageAlpha(tile_rect.x, tile_rect.y, ladder);
            }
            else if (tile_type == TILE_DIAMAND) {
                DrawImageAlpha(tile_rect.x, tile_rect.y, diamand);
            }
            else if (tile_type == TILE_LIMIT) {
                DrawImageAlpha(tile_rect.x, tile_rect.y, limit);
            }

            if (editmode) {
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 6 * TILE_PX, 6 * TILE_PX + TILE_PX, 0xFFE299);
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 8 * TILE_PX, 8 * TILE_PX + TILE_PX, 0xFFE299);
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 10 * TILE_PX, 10 * TILE_PX + TILE_PX, 0xFFE299);
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 12 * TILE_PX, 12 * TILE_PX + TILE_PX, 0xFFE299);
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 14 * TILE_PX, 14 * TILE_PX + TILE_PX, 0xFFE299);
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 16 * TILE_PX, 16 * TILE_PX + TILE_PX, 0xFFE299);
                DrawOutlineRect(18 * TILE_PX, 18 * TILE_PX + TILE_PX, 18 * TILE_PX, 18 * TILE_PX + TILE_PX, 0xFFE299);

                if (mouse_x >= tile_rect.x && mouse_x < tile_rect.x + TILE_PX && mouse_y >= tile_rect.y && mouse_y < tile_rect.y + TILE_PX) {
                    DrawOutlineRect(tile_rect.x, tile_rect.x + TILE_PX, tile_rect.y, tile_rect.y + TILE_PX, 0x08FFFFFF);
                    if (is_mouse_down(MOUSE_LEFT)) {
                        if (y == 6 && x == 18) {
                            selectedTiles = TILE_ROCKSOLID;
                        }
                        if (y == 8 && x == 18) {
                            selectedTiles = TILE_SNOWY;
                        }
                        if (y == 10 && x == 18) {
                            selectedTiles = 0;
                        }
                        if (y == 12 && x == 18) {
                            selectedTiles = TILE_SMALLROCK;
                        }
                        if (y == 14 && x == 18) {
                            selectedTiles = TILE_LADDER;
                        }
                        if (y == 16 && x == 18) {
                            selectedTiles = TILE_DIAMAND;
                        }
                        if (y == 18 && x == 18) {
                            selectedTiles = TILE_LIMIT;
                        }
                        tiles[y * 20 + x] = selectedTiles;
                        tiles_modified = true;
                    }
                    if (mouse_was_just_pressed(MOUSE_RIGHT)) {
                        if (tiles[y * 20 + x] == TILE_SNOWY) {
                            tiles[y * 20 + x] = 0;
                        }
                        tiles_modified = true;
                    }
                }
            }
        }
    }

}

void player_on_goal() {
    //On Goal Event
    if (tiles[(player.posy) * 20 + player.posx] == TILE_DIAMAND) {
        if (!isPlayingPickupSound) {
            audio_play_sound_clip(pickup_sound);
            isPlayingPickupSound = true;
        }
        DrawTextCentered(font, "NICE JOB", 30, -10, COLOR_CYAN);
    }
    else {
        isPlayingPickupSound = false;
    }
}

void background_sound_onloop() {
    if (playing_sound_count == 0) {
        background_sound = load_sound_clip("assets/The_Empire.wav");
        audio_play_sound_clip(background_sound);
    }
}

void player_input() {
    //switch to edit mode
    if (was_key_just_pressed(KB_KEY_TAB)) {
        editmode = !editmode;
    }

    //refactorisation
    int movement_x = 0; 
    int movement_y = 0;

    if (was_key_just_pressed(KB_KEY_RIGHT)) {
        movement_x = 1;
    }

    if (was_key_just_pressed(KB_KEY_LEFT)) {
        movement_x = -1;
    }

    if (was_key_just_pressed(KB_KEY_UP)) {
        movement_y = -1;
    }

    if (was_key_just_pressed(KB_KEY_DOWN)) {
        movement_y = +1;
    }

    if (movement_x != 0 || movement_y != 0) {
        //Apply Gravity 
        if (!isGrounded(player, tiles) && !onLadder(player, tiles)) {
            player.posy += 1;
        }
        //movement on ladder
        else if (onLadder(player, tiles) && isEmptyTiles(player.posx + movement_x, player.posy + movement_y, tiles)) {
            player.posx += movement_x;
            player.posy += movement_y;
        }
        //X axis movement
        else {
            if (isEmptyTiles(player.posx + movement_x, player.posy, tiles)) {
                player.posx += movement_x;
            }

        }
        audio_play_sound_clip(moving_sound);
        nbr_de_coup++;
    }

    draw_payer(player);

    //Save - Load Special Editable lvl
    if (was_key_just_pressed(KB_KEY_S)) {
        save();
    }
    if (was_key_just_pressed(KB_KEY_L)) {
        load();
        reset_player_pos(&player);
    }

    //Player tiles position is Diamond goal
    player_on_goal();

    //Play Background sound in loop 
    background_sound_onloop();

    //load lvl
    if (was_key_just_pressed(KB_KEY_1)) {
        load_on_file("lvl_one");
        reset_player_pos(&player);
        last_lvl_load = 1;
        editmode = false;
    }
    if (was_key_just_pressed(KB_KEY_2)) {
        load_on_file("lvl_two");
        if (last_lvl_load != 5) {
            reset_player_pos(&player);
        }
        last_lvl_load = 2;
        editmode = false;
    }
    if (was_key_just_pressed(KB_KEY_3)) {
        load_on_file("lvl_three");
        reset_player_pos(&player);
        last_lvl_load = 3;
        editmode = false;
    }
    if (was_key_just_pressed(KB_KEY_4)) {
        load_on_file("lvl_four");
        reset_player_pos(&player);
        last_lvl_load = 4;
        editmode = false;
    }
    if (was_key_just_pressed(KB_KEY_5)) {
        load_on_file("lvl_five");
        if (last_lvl_load != 2) {
            reset_player_pos(&player);
        }
        last_lvl_load = 5;
        editmode = false;
    }
}

void draw_ui() {
    DrawText(font, "Nbr de coup : " + std::to_string(nbr_de_coup), 0, 0, COLOR_UI);
    if (!editmode) {
        DrawText(font, "Tab : edit mod", 128, 0, COLOR_UI);
        DrawTextCentered(font, "lv.1 - 5 : Press 1 - 5", 30, FRAME_SY / 2 - 10, COLOR_UI);
    }
    else {
        DrawText(font, "Tab : play mod", 128, 0, COLOR_UI);
        DrawTextCentered(font, "S : save, L : load", 30, FRAME_SY / 2 - 10, COLOR_UI);
    }

}

int main()
{
    //Audio Desc 
    saudio_desc audio_desc = {};
    audio_desc.stream_cb = audio_callback;//stream callback
    audio_desc.logger.func = slog_func;

    saudio_setup(audio_desc);

    load_assets();

    struct mfb_window* window = mfb_open_ex("Game", WINDOW_SIZE_X, WINDOW_SIZE_Y, WF_RESIZABLE);
    if (!window) {
        return 0;
    }


	buffer = (uint32_t*)malloc(WINDOW_SIZE_X * WINDOW_SIZE_Y * 4);
    resizeBuffer = (uint32_t*)malloc(FRAME_SX * FRAME_SY * 4);

    //background
    DrawAllWindowPixel(resizeBuffer, COLOR_BG);

    mfb_set_keyboard_callback(window, on_keyboard_event);
    mfb_set_mouse_move_callback(window, on_mouse_move);
    mfb_set_mouse_button_callback(window, on_mouse_button);
    mfb_get_monitor_scale(window, &monitorScaleX, &monitorScaleY);

    audio_play_sound_clip(background_sound);

    //load lvl One
    load_on_file("lvl_one");
    reset_player_pos(&player);
    last_lvl_load = 1;
    editmode = false;

    do {
        int state;
        DrawAllWindowPixel(resizeBuffer, COLOR_BG);
        DrawImageAlpha(0, 0, background);
        draw_tiles();   
        player_input();
        draw_ui();  
        tick_input();

        resize_bitmap(buffer, WINDOW_SIZE_X, WINDOW_SIZE_Y, resizeBuffer, FRAME_SX, FRAME_SY);
        state = mfb_update_ex(window, buffer, WINDOW_SIZE_X, WINDOW_SIZE_Y);
        if (state < 0) {
            window = NULL;
            break;
        }

    } while (mfb_wait_sync(window));
}