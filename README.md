# inoLevelSensor

## A non contact level sensor for waste water tanks

### Mission
This code acts as an interface between a given number of XKC-Y25-V level sensors and a gauge that accepts Voltage input. I personally run this on a Simarine Pico without any problems.

### Hardware needed
* Arduino (nano or mini pro is preferred due to lower power consumption)
* XKC-Y25-V Level sensors (as many sensors as you want the level indication to be precise) being the maximum the input pins available.

### Output
One of the digital pins will output a signal proportional to the amount of sensors detecting liquid. Resolution can be programmed.
This output signal is PWM (due to Arduino only being able to do it this way).

### Errors
If the sensors are not secuentially lit, the arduino throws an error indicating a full tank (which is just to prevent any overflow).
The status led flashes as to indicate the number of the sensor that is failing. (3 flashes = sensor nº 3)


### Troubleshooting
If the sensors do no light up correctly, try adjusting the sensitiveness through the screw located under the back cover of each sensor. The adjustment process is a bit finicky, but once it is correctly adjusted, everything works flawlessly.

&copy; **D.Sánchez 20019** Published under the **EUPL v1.2**
