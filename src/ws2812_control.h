#ifndef WS2812_CONTROL_H
#define WS2812_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// Initialize the WS2812 LED
void ws2812_init(void);

// Set the color of the WS2812 LED (GRB format)
void ws2812_set_color(uint32_t color);

// Set the color of the WS2812 LED using RGB values
void ws2812_set_rgb(uint8_t r, uint8_t g, uint8_t b);

// State management for debugprobe
typedef enum {
    WS2812_STATE_USB_DISCONNECTED = 0,  // USB not ready
    WS2812_STATE_USB_CONNECTED,          // USB ready, no DAP connection
    WS2812_STATE_DAP_CONNECTED,          // DAP debugger connected
    WS2812_STATE_DAP_RUNNING,            // Target running
    WS2812_STATE_UART_TX,                // UART transmitting
    WS2812_STATE_UART_RX,                // UART receiving
    WS2812_STATE_MAX
} ws2812_state_t;

// Set a specific state (with priority handling)
void ws2812_set_state(ws2812_state_t state, bool active);

// Update LED based on current states (call periodically)
void ws2812_update(void);

// Pre-defined colors (GRB format) for different states
#define WS2812_COLOR_OFF           0x000000

// USB states (GRB: Green-Red-Blue)
#define WS2812_COLOR_USB_DISCONNECTED  0x000800  // Dim Red (G=0, R=8, B=0)
#define WS2812_COLOR_USB_CONNECTED     0x080000  // Dim Green (G=8, R=0, B=0)

// DAP states - not used currently
#define WS2812_COLOR_DAP_CONNECTED     0x400000  // Medium Green (G=64, R=0, B=0)
#define WS2812_COLOR_DAP_RUNNING       0x000040  // Medium Blue (G=0, R=0, B=64)

// UART activity
#define WS2812_COLOR_UART_TX           0x101000  // Yellow (G=16, R=16, B=0)
#define WS2812_COLOR_UART_RX           0x100010  // Cyan (G=16, R=0, B=16)

// Combined states
#define WS2812_COLOR_UART_TXRX         0x001010  // Magenta (G=0, R=16, B=16)

#endif // WS2812_CONTROL_H