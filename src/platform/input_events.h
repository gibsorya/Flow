// ============================================================
// src/platform/input_events.h
// ============================================================
// Input as plain data. This header defines WHAT happened;
// window.h defines the machinery that produces it. Systems
// that consume input include this file only — they never need
// to know the window contract exists.
//
// No includes beyond <cstdint>. Ever.

#pragma once
#include <cstdint>

// ---- Event kinds ---------------------------------------------

enum class EventType : uint16_t {
    None = 0,

    // window
    WindowClosed,
    WindowResized,       // width, height
    WindowFocus,         // flag: 1 = gained, 0 = lost

    // keyboard
    KeyDown,             // key; flag: 1 = OS key-repeat
    KeyUp,               // key
    CharTyped,           // codepoint (UTF-32, post-IME text input)

    // mouse
    MouseMoved,          // x, y
    MouseButtonDown,     // button, x, y
    MouseButtonUp,       // button, x, y
    MouseScrolled,       // scrollX, scrollY
};

// ---- Engine-owned keycodes -----------------------------------
// Backends translate native codes (GLFW_KEY_*, VK_*, X11
// keysyms) into these. Native codes never cross the boundary.
// Grow this enum as you need keys; keep Count last.

enum class Key : uint16_t {
    Unknown = 0,

    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    Num0, Num1, Num2, Num3, Num4,
    Num5, Num6, Num7, Num8, Num9,

    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    Escape, Space, Enter, Tab, Backspace, Delete,
    Left, Right, Up, Down,
    LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,

    Count   // array-sizing sentinel: bool keyDown[(int)Key::Count]
};

// ---- Mouse buttons -------------------------------------------

enum MouseButton : uint16_t {
    MOUSE_LEFT   = 0,
    MOUSE_RIGHT  = 1,
    MOUSE_MIDDLE = 2,
};

// ---- The event -----------------------------------------------
// One fixed-size POD for all kinds (tagged-union style). Which
// fields are meaningful depends on 'type' — see EventType
// comments. Trivially copyable: an Event[] can be memcpy'd,
// written to disk for replay, or handed across threads.

struct Event {
    EventType type   = EventType::None;
    Key       key    = Key::Unknown;
    uint16_t  button = 0;         // MouseButton values
    uint16_t  flag   = 0;         // key-repeat / focus-gained
    uint32_t  codepoint = 0;      // CharTyped only
    float     x = 0.0f, y = 0.0f; // mouse position (pixels)
    float     scrollX = 0.0f, scrollY = 0.0f;
    uint32_t  width = 0, height = 0;   // WindowResized only
};

static_assert(sizeof(Event) <= 40, "keep events small; queues copy them");