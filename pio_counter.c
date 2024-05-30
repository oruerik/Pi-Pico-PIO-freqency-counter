#include "pio_counter.h"
#include "pio_counter.pio.h"
#include "pps.pio.h"
#include "buf.h"

#define PPS_PIN 2
#define SIGNAL_PIN 15

// External routeen declarations
void Display_Update(void);
void Fatal_Error(uint8_t *msg);

// Internal declarations
void fcount_bg_task(void);

volatile absolute_time_t ppsTime;

volatile int32_t prevCount = 0;
volatile int32_t pulseCountSnapshot = 0;

// global data
volatile uint32_t pulseCount = 0;   // Target for the DMA stream from the PPS SM

volatile uint32_t delta = 0;        // Actual frequency counted (Calculated).

volatile double avg;
volatile int32_t seconds = 0;

PIO pio = pio0;

#define SC_COUNTER 0

//  ========== PPS Interupt handler  ===========

__attribute__ ((long_call, section (".time_critical"))) void pps_callback()  // Time critical not really needed!
//void pps_callback()
    {
    pulseCountSnapshot = pio_sm_get(pio, SC_COUNTER);    // Grab snapshot from PIO
    pio_interrupt_clear(pio, 0);

    delta = prevCount - pulseCountSnapshot;
    prevCount = pulseCountSnapshot;

    seconds++;
    Display_Update();
    }

//  ========= pio_counter init =================

void pio_counter_init(void) 
    {
//    bool clkset = set_sys_clock_khz(250000, false);       // Running at standard speed, we can measure frequency
//    if(clkset)    printf("Clock is set at 250Mhz");       // up to 15Mhz... I don't need anything faster.

    // setup PPS IRQ/ISR
    irq_set_exclusive_handler(PIO0_IRQ_0, pps_callback);
    irq_set_enabled(PIO0_IRQ_0, true);

    // load PIO programs
    uint offset = pio_add_program(pio, &counter_program);
    counter_program_init(pio, SC_COUNTER, offset, SIGNAL_PIN, PPS_PIN);

    // enable state machines
    pio_sm_set_enabled(pio, SC_COUNTER, true);
    }
//  ============ End of set up ================
