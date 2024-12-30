# Maximale Spannung: 2,5V - 2,6V
# Maximaler Strom: 1200mA

from machine import Pin, PWM
from time import sleep

pwm_pin = Pin(5)  # GPIO 5 (D1) für PWM
pwm = PWM(pwm_pin)
pwm.freq(1000)

def set_motor_speed(speed: int):
    """
    Stellt die Geschwindigkeit des Motors ein.
    :param speed: Geschwindigkeit (0 bis 100).
    """
    if speed < 0 or speed > 100:
        raise ValueError("Die Geschwindigkeit muss zwischen 0 und 100 liegen.")
    duty = int(speed * 1023 / 100)  # Wandelt 0-100% in 0-1023 um
    pwm.duty(duty)

while True:
    for speed in range(60, 100):
        sleep(0.1)
        print(f"Speed: {speed}")
        set_motor_speed(speed)
    for speed in range(100, 59, -1):
        sleep(0.1)
        print(f"Speed: {speed}")
        set_motor_speed(speed)
