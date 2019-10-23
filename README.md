# Arduino-WaterLevel

WaterLevel in Arudino.

## Support platform

- esp8226
- esp32

## Usage

### Initialize WaterLevel

```cpp
void setup() {
  int pins[3] = {1,2,3};
  size_t pin_num = 3;
  WaterLevel my_water_level = WaterLevel(pin, pin_num);
  water_level.setMaxWaterLevel(3);
  water_level.setMinWaterLevel(1);
}
```

Get water level

```cpp
void loop() {
  water_level.check();
  unsigned int current_water_level = water_level.getWaterLevel();
  WaterLevelState current_water_level_state = water_level.getWaterLevelState();
}
```
