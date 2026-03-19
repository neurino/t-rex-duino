# T-Rex-duino

Clone of T-rex game from Chrome browser rewritten for Arduino.

![demo][]

Forked from [github.com/AlexIII/t-rex-duino] to support SPI SSD1306
0.96" OLED display and re-organised as a [PlatformIO][] project.

A few components needed:

- Arduino Pro Mini
  - If you want to use Uno, Nano etc. change board in `platformio.ini`
- SPI OLED display 128x64 SSD1306 (or SSD1309)
- Two buttons: one to jump, one to duck
  - Keep both pressed on power-up to reset hi score

## Instructions

1. Assemble
2. [Install PlatformIO core][]
3. Upload with `$ pio run -t upload`
4. Play!
5. Repeat from step 4

## License

[MIT License][]  
© 2020 github.com/AlexIII  
© 2026 github.com/neurino

[demo]: img/t-rex-demo-3.gif
[github.com/AlexIII/t-rex-duino]: https://github.com/AlexIII/t-rex-duino
[PlatformIO]: https://platformio.org/
[Install PlatformIO core]: https://docs.platformio.org/en/latest/core/installation/index.html
[MIT License]: LICENSE
