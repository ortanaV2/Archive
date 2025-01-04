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

cycle_time = 0.025
start_speed = 50
end_speed = 100
while True:
    for speed in range(start_speed, end_speed):
        sleep(cycle_time)
        print(f"Speed: {speed}")
        set_motor_speed(speed)
        if speed == start_speed:
            for it in range(10):
                sleep(cycle_time)
                print(f"Speed: {speed}")
                set_motor_speed(speed)
    for speed in range(end_speed, start_speed-1, -1):
        if speed == end_speed:
            for it in range(10):
                sleep(cycle_time)
                print(f"Speed: {speed}")
                set_motor_speed(speed)
        else:
            sleep(cycle_time)
            print(f"Speed: {speed}")
            set_motor_speed(speed)

