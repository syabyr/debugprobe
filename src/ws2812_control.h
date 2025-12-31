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
#define WS2812_COLOR_USB_DISCONNECTED  0x001000  // Dim Red - USB not ready
#define WS2812_COLOR_USB_CONNECTED     0x100000  // Dim Green - USB ready

// DAP states
#define WS2812_COLOR_DAP_CONNECTED     0xFF0000  // Bright Green - DAP connected
#define WS2812_COLOR_DAP_RUNNING       0x0000FF  // Blue - Target running

// UART activity
#define WS2812_COLOR_UART_TX           0x80FF00  // Yellow - UART TX (Green + Red)
#define WS2812_COLOR_UART_RX           0xFF00FF  // Cyan - UART RX (Green + Blue)

// Combined states
#define WS2812_COLOR_UART_TXRX         0x00FFFF  // Magenta - UART TX+RX (Red + Blue)

#endif // WS2812_CONTROL_H