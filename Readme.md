# 	Erik's RP2040 PIO based frequency counter

As part of my "2nd Gen" 10Mhz GPS conditioned oscillator and timepiece I went looking for an "easy" Pi Pico based Frequency/Pulse counter. There were many styles, PWM, DMA, Software, Inverse timer based solutions. None of these would meet my need to reliably count a 10Mhz pulse stream.

The closest candidate was Richard Kendall's implementation:
https://rjk.codes/post/building-a-frequency-counter/https://rjk.codes/post/building-a-frequency-counter/
As he pointed out his counter still had up to 4 counts of jitter, when fed a precision 10Mhz signal, and gated by a GPS... I spent a lot of time trying to get to the bottom of that jitter, but was not successful. In the end I started from scratch, applying everything I had learnt chasing Richard's solution. I thank him for providing a platform to beat my head against.  😊

Onwards, my solution was ultimately quite simple, a single SM, not requiring DMA nor any system overhead. It counts reliably up to about 15Mhz. I expect you could take it further if you bumped the system speed.

The code is extensively commented. 

There are two 'C' routines, An init routines that configures and loads the PIO State Machine, and an interrupt routines that reads the PIO FIFO to capture the saved count from the previous count cycle. There is no need for DMA as the "data rate" from the counter in one int32 per count cycle (One second if using a GPS PPS)

The PIO code uses two inputs, the clock that is being measured, the "f_pin"; and the gating signal, 'pps_pin'. The comments should provide adequate guidance as to what's happening.

I hope this saves others time and heart burn.

Erik Piip
erikpiip@gmail.com
2024.05.30
