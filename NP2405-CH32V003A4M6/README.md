# CH32V003/NP2404,NP2405 Tiny House

"小さなお家" の各LEDをランダムに順次点灯し，一定時間後に順次消灯する制御ソフトウェア．

## Hardware

### Features

- MCU: CH32V003A4M6(SOP16)
- Board Size: 55mm x 32mm(NP2404), 53mm x 32mm(NP2405)
- Number of PWM outputs: 6
- Button press detection
- Battery: AAA

### Pin Assignment

| GPIO | Description |
| --- | --- |
| PA2 | ADC, Button press detection |
| PC1 | PWM, LED Control |
| PC4 | PWM, LED Control |
| PC6 | Output, DC/DC control |
| PC7 | PWM, LED Control |
| PD1 | SWIO |
| PD4 | PWM, LED Control |
| PD5 | PWM, LED Control |
| PD6 | PWM, LED Control |

### Schematics

![Schematics](schematics.png)

## Software

### Development Environment

- MounRiverStudio
