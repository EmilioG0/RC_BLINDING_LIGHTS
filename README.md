# RC_BLINDING_LIGHTS
Arduino nano based RC car lights with turn signals and exhaust backfire!!

This sketch will work on a 3 channel receiver and an arduino nano.


Channel 1 signal (steering) will be on Digital pin 2 on the arduino nano.

Channel 2 signal (Throtle) will be on Digital pin 10 on the arduino nano.

Channel 3 signal (on/off lights toggle) will be on Digital pin 12 on the arduino nano.


As for the leds used, it is not necesary to use resistors since they will be powered by PWM, but 
you can use them if you want. 


The leds outputs pins are the following:


Digital pin 5 are the rear lights (PWM)

Digital pin 6 are the headlights (PWM)

Digital pin 7 are the left turn lights (PWN)

Digital pin 8 are the right turn lights (PWM)

Digital pin 3 are the exhaust flame lights (PWM)

DIgital pin 4 are the reverse lights (PWM)


Once everything is wired up, you should first monitor all the receiver signals and adjust accordingly the value on the sketch 
for the on/off switch value (Channel 3), the Throttle value (Channel 2) and the steering value for left and right (Channel 1).

Once you test and tweak these values, then just suppress the monitor display fucntions and the delay after the print fuctions. 

I really hope you enjoy this sketch as much as I did. If you have any suggestions let me know.

If you like my sketch, find it usefull and would like to contribute, feel free to do so at the following paypal link:

https://paypal.me/EmilioG0?locale.x=es_ES

Thank you


