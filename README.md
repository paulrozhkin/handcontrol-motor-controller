<a href="#">
    <img src="https://raw.githubusercontent.com/paulrozhkin/handcontrol-documentation/master/img/logo.jpg" title="HandControl" alt="HandControl" width="300">
</a>

# Контроллер линейных приводов
Репозиторий содержит код управления линейными приводами [PQ12-P](https://www.actuonix.com/PQ12-Micro-Linear-Actuators-s/1825.htm) на STM32F103C8T6 для бионического протеза руки человека [HandControl](https://github.com/paulrozhkin/handcontrol-documentation). Управление приводами осуществляется с помощью H-мостов L293D. Контроллер обеспечивает регулируемое изменение положений пальцев протеза (приводов). Каждый палец имеет 180 степений свободы, которые могут быть установлены с точностью 5% на основании обработки обратной связи приводов. Установка положений палец осуществляется при приеме команд по SPI.

Схема контроллера доступна в репозитории [DCDriverShematic](https://github.com/paulrozhkin/DCDriverShematic)

## Linked Repositories
- [Документация](https://github.com/paulrozhkin/handcontrol-documentation)
- [Контроллер управления протезом на Raspberry Pi](https://github.com/paulrozhkin/arm_prosthesis_raspberry)
- [Приложение для конфигурирования протеза для ПК на WPF](https://github.com/paulrozhkin/HandControlApplication)
- [Приложение для управления протезом для Android](https://github.com/ForsaiR/HandControlAndroidAplication)
- [Схемотехника драйвера моторов (Altium Designer)](https://github.com/paulrozhkin/DCDriverShematic)
- [Схемотехника контроллера управления (Altium Designer)](https://github.com/paulrozhkin/ArmProsthesisShematic)
- [Сервер для одновременной работы с несколькими протезами по MQTT на Kotlin](https://github.com/paulrozhkin/hand-control-mqtt)
- [MQTT proxy для контроллера управления протеза для STM32F767ZITX](https://github.com/paulrozhkin/handcontrol-mqtt-proxy)

---

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
- Copyright 2020 © <a href="https://github.com/paulrozhkin" target="_blank">Paul Rozhkin</a>.
