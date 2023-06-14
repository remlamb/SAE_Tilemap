#define WINDOW_FAC 2

//------INPUTS
float monitorScaleX, monitorScaleY;
int mouse_x, mouse_y;
bool mouse_button_state[MOUSE_BTN_7 + 1];
bool mouse_button_state_prev[MOUSE_BTN_7 + 1];

bool keys_state[KB_KEY_LAST + 1]; // KB_KEY_LAST ->define du dernier Key 
bool keys_state_prev[KB_KEY_LAST + 1];

void tick_input()
{
    memcpy(keys_state_prev, keys_state, sizeof(keys_state));
    memcpy(mouse_button_state_prev, mouse_button_state, sizeof(mouse_button_state));
}

void on_keyboard_event(struct mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed)
{
    keys_state[key] = isPressed;
}

bool is_key_down(mfb_key key) {
    return keys_state[key];
}

bool was_key_just_pressed(mfb_key key) {
    return !keys_state_prev[key] && keys_state[key];
}

bool was_key_just_released(mfb_key key) {
    return keys_state_prev[key] && !keys_state[key];
}

void on_mouse_move(struct mfb_window* window, int x, int y) {
    mouse_x = x / WINDOW_FAC / monitorScaleX;
    mouse_y = y / WINDOW_FAC / monitorScaleY;
}

void on_mouse_button(struct mfb_window* window, mfb_mouse_button button, mfb_key_mod mod, bool isPressed) {
    mouse_button_state[button] = isPressed;
}

bool mouse_was_just_pressed(mfb_mouse_button button) {
    return !mouse_button_state_prev[button] && mouse_button_state[button];
}

bool mouse_was_just_released(mfb_mouse_button button) {
    return mouse_button_state_prev[button] && !mouse_button_state[button];
}


bool is_mouse_down(mfb_mouse_button button) {
    return mouse_button_state[button];
}