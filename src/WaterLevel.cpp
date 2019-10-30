/**
 * @file WaterLevel.cpp
 * @brief Water level
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#include <WaterLevel.h>

/**
 * @fn WaterLevel::WaterLevel()
 * @brief WaterLevel constructor
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevel::WaterLevel() {}

/**
 * @fn WaterLevel::WaterLevel()
 * @brief Water level constructor
 * @param pins Water level GPIO pins to be set up
 * @param length The number of water level GPIO pins
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevel::WaterLevel(int pins[], size_t length) { setPins(pins, length); }

/**
 * @fn WaterLevel::~WaterLevel()
 * @brief WaterLevel destructor
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevel::~WaterLevel() {
  if (_pins != NULL) {
    free(_pins);
    _pins = NULL;
  }
}

/**
 * @fn void WaterLevel::setPins(int pins[], size_t length)
 * @brief Set water level GPIO pins
 * @param pins Water level GPIO pins to be set up
 * @param length The number of water level GPIO pins
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::setPins(int pins[], size_t length) {
  _length = length;
  _pins = (int *)malloc(sizeof(int) * _length);
  for (int i = 0; i < _length; i++) {
    _pins[i] = pins[i];
    pinMode(_pins[i], INPUT_PULLUP);
  }
}

/**
 * @fn void WaterLevel::setEEPROMAddress(int eeprom_address)
 * @brief Set EEPROM address
 * @param eeprom_address EEPROM address to be set up
 * @return void
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::setEEPROMAddress(int eeprom_address) {
  _eeprom_address = eeprom_address;
}

/**
 * @fn void WaterLevel::loadData(void)
 * @brief Load EEPROM data
 * @return void
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::loadData(void) {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(_eeprom_address, _data);
  if (!_data.is_saved) {
    _data.min_water_level = 9999;
    _data.max_water_level = 0;
  }
  EEPROM.end();
  printf("[WaterLevel] min water level : %d max water level: %d\n",
         _data.min_water_level, _data.max_water_level);
}

/**
 * @fn void WaterLevel::saveData(void)
 * @brief Save EEPROM data
 * @return void
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::saveData(void) {
  EEPROM.begin(EEPROM_SIZE);
  printf("[WaterLevel] EEPROM address: %d\n", _eeprom_address);
  _data.is_saved = true;
  EEPROM.put(_eeprom_address, _data);
  EEPROM.commit();
  EEPROM.end();
}

/**
 * @fn void WaterLevel::check(void)
 * @brief Check water level
 * @return void
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::check(void) {
  _last_state = _state;
  _last_water_level = _water_level;
  _water_level = 0;
  int count = 20;
  unsigned int _water_levels[100], _temp_water_level;

  memset(_water_levels, 0, sizeof(_water_levels));

  for (int i = 0; i < count; i++) {
    _temp_water_level = 0;
    for (int idx = 0; idx < _length; idx++) {
      if (digitalRead(_pins[idx]) == HIGH) {
        _temp_water_level = idx + 1;
      }
      delay(20);
    }
    _water_levels[_temp_water_level]++;
  }
  for (int i = 0; i < count; i++) {
    if (_water_levels[i] > _water_levels[_water_level]) {
      _water_level = i;
    }
  }
  // Prevent water level sensor errors due to value error
  // (ex. It suddenly turns to the lowest water level.)
  int gap = _last_water_level - _water_level;
  if (_last_water_level != INITIAL_WATER_LEVEL && gap > 1) {
    if (_temp_water_level != _water_level) {
      _count = 1;
      _temp_water_level = _water_level;
    } else if (_temp_water_level == _water_level) {
      _count++;
    }
    if (_count < 10) {
      _water_level = _last_water_level;
    }
  } else {
    _temp_water_level = INITIAL_WATER_LEVEL;
    _count = 0;
  }

  if (_water_level < _data.min_water_level) {
    _state = LACK_WATER_LEVEL;
  } else if (_data.min_water_level <= _water_level &&
             _water_level < _data.max_water_level) {
    _state = GOOD_WATER_LEVEL;
  } else if (_data.max_water_level <= _water_level) {
    _state = FLOOD_WATER_LEVEL;
  } else {
    _state = ERROR_WATER_LEVEL;
  }
  printf("[WaterLevel] water level : %d state : %d\n", _water_level, _state);
}

/**
 * @fn unsigned int WaterLevel::getWaterLevel(void)
 * @brief Get water level
 * @return Water level
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
unsigned int WaterLevel::getWaterLevel(void) { return _water_level; }

/**
 * @fn WaterLevelState WaterLevel::getWaterLevelState(void)
 * @brief Get water level state
 * @return Water level state
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevelState WaterLevel::getWaterLevelState(void) { return _state; }

/**
 * @fn void WaterLevel::setMinWaterLevel(unsigned int min_water_level);
 * @brief Set minimum water level
 * @param min_water_level Minimum water level to be set up
 * @return void
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::setMinWaterLevel(unsigned int min_water_level) {
  _data.min_water_level = min_water_level;
}

/**
 * @fn void WaterLevel::setMaxWaterLevel(int max_water_level)
 * @brief Set maximum water level
 * @param min_water_level Maximum water level to be set up
 * @return void
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevel::setMaxWaterLevel(unsigned int max_water_level) {
  _data.max_water_level = max_water_level;
}
/**
 * @fn unsigned int WaterLevel::getMinWaterLevel(void)
 * @brief Get minimum water level
 * @return Minimum water level
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
unsigned int WaterLevel::getMinWaterLevel(void) {
  return _data.min_water_level;
}
/**
 * @fn unsigned int WaterLevel::getMaxWaterLevel(void)
 * @brief Get maxnimum water level
 * @return Maxnimum water level
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
unsigned int WaterLevel::getMaxWaterLevel(void) {
  return _data.max_water_level;
}
/**
 * @fn bool WaterLevel::isWateLevelChanged(void)
 * @brief Check if water level has changed or not
 * @return The change of water level
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevel::isWateLevelChanged(void) {
  return _last_water_level != _water_level;
}

/**
 * @fn bool WaterLevel::isWateLevelStateChanged(void)
 * @brief Check if water level state has changed or not
 * @return The change of water level state
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevel::isWateLevelStateChanged(void) { return _last_state != _state; }
