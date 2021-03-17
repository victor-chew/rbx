# RBX

### What is RBX?

[RBX](https://rbx.randseq.org/) (*Robotics Brick Extension*) is a robotics kit designed for young tinkerers. It consists of a set of Lego-compatible "bricks" made with common  [Arduino](https://www.arduino.cc/)  components, such as LED, pushbutton, servo, motor etc. The brick housing are printed using a 3D printer. The RBX bricks are hooked up to a microcontroller via standardized "ports". Programming is done via a variant of [Javascript](https://duktape.org) on a browser-based IDE.

[![rbx mcu assembled](https://rbx.randseq.org/images/rbx/bricks/rbx-microcontroller.jpg "Microcontroller")](https://rbx.randseq.org/images/rbx/rbx-mcu-assembled.jpg) [![rbx battery aa](https://rbx.randseq.org/images/rbx/bricks/rbx-battery-aa.jpg "Battery (4 x AA)")](https://rbx.randseq.org/images/rbx/bricks/rbx-battery-aa.jpg) [![rbx battery 18650](https://rbx.randseq.org/images/rbx/bricks/rbx-battery-18650.jpg "Battery (18650)")](https://rbx.randseq.org/images/rbx/bricks/rbx-battery-18650.jpg) [![rbx motor power](https://rbx.randseq.org/images/rbx/bricks/rbx-motor-power.jpg "Motor Power)")](https://rbx.randseq.org/images/rbx/bricks/rbx-motor-power.jpg) [![rbx led](https://rbx.randseq.org/images/rbx/bricks/rbx-led.jpg "LED")](https://rbx.randseq.org/images/rbx/bricks/rbx-led.jpg) [![rbx pushbutton](https://rbx.randseq.org/images/rbx/bricks/rbx-pushbutton.jpg "Pushbutton")](https://rbx.randseq.org/images/rbx/bricks/rbx-pushbutton.jpg) [![rbx buzzer](https://rbx.randseq.org/images/rbx/bricks/rbx-buzzer.jpg "Buzzer")](https://rbx.randseq.org/images/rbx/bricks/rbx-buzzer.jpg) [![rbx photoresistor](https://rbx.randseq.org/images/rbx/bricks/rbx-photoresistor.jpg "Photoresistor")](https://rbx.randseq.org/images/rbx/bricks/rbx-photoresistor.jpg) [![rbx oled](https://rbx.randseq.org/images/rbx/bricks/rbx-oled.jpg "OLED Display")](https://rbx.randseq.org/images/rbx/bricks/rbx-oled.jpg) [![rbx rotary switch](https://rbx.randseq.org/images/rbx/bricks/rbx-rotary-switch.jpg "Rotary Switch")](https://rbx.randseq.org/images/rbx/bricks/rbx-rotary-switch.jpg) [![rbx servo](https://rbx.randseq.org/images/rbx/bricks/rbx-servo.jpg "Servo")](https://rbx.randseq.org/images/rbx/bricks/rbx-servo.jpg) [![rbx motor](https://rbx.randseq.org/images/rbx/bricks/rbx-motor.jpg "Motor")](https://rbx.randseq.org/images/rbx/bricks/rbx-motor.jpg) [![rbx sonar](https://rbx.randseq.org/images/rbx/bricks/rbx-sonar.jpg "Sonar")](https://rbx.randseq.org/images/rbx/bricks/rbx-sonar.jpg) [![rbx color sensor](https://rbx.randseq.org/images/rbx/bricks/rbx-color-sensor.jpg "Color Sensor")](https://rbx.randseq.org/images/rbx/bricks/rbx-color-sensor.jpg)

### Target audience

The target audience is 8-12 year old children who need something between the simplicity of block-based programming environment such as Lego Mindstorm, and a full-blown IDE such as  [Arduino](https://www.arduino.cc/)  or  [PlatformIO](https://platformio.org/). Not only is Javascript the de-facto scripting language, it also eliminates the long compile-build-run cycle, which is more suited for experimentation with impatient young minds.

### Making hardware accessible

One of the first obstacles for a young student trying to break into microcontroller programming is hooking up the desired circuitry on a breadboard. Anything beyond the the introductory "Hello World" LED example requires a ton of wiring, which is quite a lot to ask for a young child. When servos or motors are involved, a separate power rail is required, which makes things even more complicated.

RBX uses various JST connectors to connect to the microcontroller. These are called "ports", which maps to the microcontroller pins. The ports come in 2, 3 and 4-pin variety, and each port has a unique identifier, Other connectors are used for providing power to motors or servos. In this way, hooking up stuff becomes a plug-and-play affair. Since the connectors need to be matching and oriented correctly, this makes it less likely for modules to be hooked up the wrong way.

### Microntroller

For the microcontroller, the ESP32 is chosen. It is incredibly powerful, and comes with built-in WIFI and bluetooth. The development board is also very affordable, available for only a few dollars online. It has a 2-core processor, and has enough RAM and flash storage to run a Javascript interpreter. It also has many pins available, which lets the microcontroller connect to many components simultaneously.

[![esp32 wroom 32d](https://rbx.randseq.org/images/rbx/esp32-wroom-32d.jpg)](https://rbx.randseq.org/images/rbx/esp32-wroom-32d.jpg)

### Programming made easy

For the JavaScript interpreter,  [Duktape](https://duktape.org/)  was ported to the ESP32.  [Duktape](https://duktape.org/)  has very low resource and memory requirement, which makes it particularly suited for a microcontroller.

A browser-based IDE is served directly from the microcontroller, using  [Ace](https://ace.c9.io/)  as the code editor. Once the microcontroller is powered on, the user is able to connect via a web browser and start running JavaScript code immediately. Code can be saved to the flash storage on the ESP32 and retrieved later.

[![rbx startup 06](https://rbx.randseq.org/images/rbx/rbx-startup-06.png)](https://rbx.randseq.org/images/rbx/rbx-startup-06.png)

### Conclusion

In summary, RBX is relatively cheap to make, yet it is far more versatile than most of the pre-built electronic/robotic kits on the market. A young maker can get up to speed with the kit very quickly. As more experience is accumulated, he/she can delve into each of the "bricks" to see what makes them tick, or even make them from scratch.

On the software side, although we start with JavaScript, it is quite easy to switch to  [Arduino](https://www.arduino.cc/)  for more complex projects. At the root of RBX is an  [Arduino](https://www.arduino.cc/)  library, which is pretty straightforward to engage with directly for a more experienced  [Arduino](https://www.arduino.cc/)  programmer.

### Resources

 - [RBX website](https://rbx.randseq.org/)
 - [Sample projects](https://rbx.randseq.org/index.php/blinking-led)
