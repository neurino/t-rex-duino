# T-Rex-duino

Clone of T-rex game from Chrome browser rewritten for Arduino.

Forked from [github.com/AlexIII/t-rex-duino] to support SSD1306 0.96"
OLED display using SPI and re-organised as a [PlatformIO][] project.

A few components needed:

- Arduino Pro Mini (easy to change target in `platformio.ini`)
- SPI OLED display 128x64 SSD1306 (or SSD1309)
- A button (or two, if you want to duck!)

## Instructions

1. Assemble
2. [Install PlatformIO core][]
3. `$ pio run -t upload`
4. Play!
5. Repeat from step 4

## License

MIT License  
© 2020 github.com/AlexIII  
© 2026 github.com/neurino

[github.com/AlexIII/t-rex-duino]: https://github.com/AlexIII/t-rex-duino
[PlatformIO]: https://platformio.org/
[Install PlatformIO core]: https://docs.platformio.org/en/latest/core/installation/index.html
