#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "probe_config.h"
#include "ws2812_control.h"

static PIO ws2812_pio = pio0;
static uint ws2812_sm = 0;
static bool ws2812_initialized = false;

// State tracking
static bool state_flags[WS2812_STATE_MAX] = {0};
static uint32_t current_color = WS2812_COLOR_OFF;

void ws2812_init(void) {
#ifdef WS2812_GPIO
    if (ws2812_initialized) return;
    
    // Initialize the PIO program for WS2812
    uint offset = pio_add_program(ws2812_pio, &ws2812_program);
    ws2812_sm = pio_claim_unused_sm(ws2812_pio, true);

    ws2812_program_init(ws2812_pio, ws2812_sm, offset, WS2812_GPIO, 800000.0f, false);
    ws2812_initialized = true;
    
    // Initialize all states to false
    for (int i = 0; i < WS2812_STATE_MAX; i++) {
        state_flags[i] = false;
    }
#endif
}

static inline void ws2812_put_pixel(uint32_t pixel_grb) {
#ifdef WS2812_GPIO
    pio_sm_put_blocking(ws2812_pio, ws2812_sm, pixel_grb << 8u);
#endif
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}

void ws2812_set_color(uint32_t color) {
#ifdef WS2812_GPIO
    if (!ws2812_initialized) return;
    current_color = color;
    ws2812_put_pixel(color);
#endif
}

void ws2812_set_rgb(uint8_t r, uint8_t g, uint8_t b) {
#ifdef WS2812_GPIO
    if (!ws2812_initialized) return;
    uint32_t color = urgb_u32(r, g, b);
    current_color = color;
    ws2812_put_pixel(color);
#endif
}

// Forward declaration
static void ws2812_update_internal(void);

void ws2812_set_state(ws2812_state_t state, bool active) {
#ifdef WS2812_GPIO
    if (!ws2812_initialized) return;
    if (state < WS2812_STATE_MAX) {
        state_flags[state] = active;
        // Automatically update LED when state changes
        ws2812_update_internal();
    }
#endif
}

// Priority-based color selection
// Priority (highest to lowest):
// 1. UART activity (TX+RX, TX, RX) - shows data transfer
// 2. DAP Running - target is executing
// 3. DAP Connected - debugger attached
// 4. USB Connected - ready for use
// 5. USB Disconnected - not ready
static void ws2812_update_internal(void) {
#ifdef WS2812_GPIO
    if (!ws2812_initialized) return;
    
    uint32_t new_color = WS2812_COLOR_OFF;
    
    // Check states in priority order (lowest to highest)
    // Base state: USB connection
    if (state_flags[WS2812_STATE_USB_DISCONNECTED]) {
        new_color = WS2812_COLOR_USB_DISCONNECTED;
    }
    
    if (state_flags[WS2812_STATE_USB_CONNECTED]) {
        new_color = WS2812_COLOR_USB_CONNECTED;
    }
    
    // DAP states override USB states
    if (state_flags[WS2812_STATE_DAP_CONNECTED]) {
        new_color = WS2812_COLOR_DAP_CONNECTED;
    }
    
    if (state_flags[WS2812_STATE_DAP_RUNNING]) {
        new_color = WS2812_COLOR_DAP_RUNNING;
    }
    
    // UART activity has highest priority (temporary indication)
    if (state_flags[WS2812_STATE_UART_TX] && state_flags[WS2812_STATE_UART_RX]) {
        new_color = WS2812_COLOR_UART_TXRX;
    } else if (state_flags[WS2812_STATE_UART_TX]) {
        new_color = WS2812_COLOR_UART_TX;
    } else if (state_flags[WS2812_STATE_UART_RX]) {
        new_color = WS2812_COLOR_UART_RX;
    }
    
    // Only update if color changed
    if (new_color != current_color) {
        current_color = new_color;
        ws2812_put_pixel(new_color);
    }
#endif
}

void ws2812_update(void) {
    ws2812_update_internal();
}