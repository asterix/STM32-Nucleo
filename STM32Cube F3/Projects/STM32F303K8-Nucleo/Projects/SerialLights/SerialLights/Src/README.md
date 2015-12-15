Playing around with WS2812B
===

Here is an implentation to send RGB encoded color data to WS2812B LEDs. 
The implementation can be generalized to make some sort of a C library.

The timing constraints are a little tight for timer+interrupts while the 3 DMA channel approach seemed like an overkill.
I haven't gotten around to trying the timer+interrupt mechanism in assembly to make the signaling blocking and tight. 

Might do it soon.
