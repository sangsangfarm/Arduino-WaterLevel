/**
 * @file WaterLevel.h
 * @brief Water level
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H

#include <Arduino.h>
#include <EEPROM.h>
#include <stdlib.h>
#include "Common.h"

#ifndef EEPROM_SIZE
#define EEPROM_SIZE 4096
#endif

// DEBUG
#ifndef WATER_LEVEL_DEBUG_PORT
#define WATER_LEVEL_DEBUG_PORT Serial
#endif

#ifdef WATER_LEVEL_DEBUG
#define print(...) WATER_LEVEL_DEBUG_PORT.print(__VA_ARGS__)
#define printf(...) WATER_LEVEL_DEBUG_PORT.printf(__VA_ARGS__)
#define println(...) WATER_LEVEL_DEBUG_PORT.println(__VA_ARGS__)

#else
#define print(...)
#define printf(...)
#define println(...)
#endif

#define INITIAL_WATER_LEVEL 99999
/**
 * @struct WATER_LEVEL_DATA
 * @brief Water level EEPROM data
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
struct WATER_LEVEL_DATA {
  /** Data storage state */
  bool is_saved;
  /** Minimum water level */
  unsigned int min_water_level;
  /** Maximum water level */
  unsigned int max_water_level;
};

class WaterLevel {
 private:
  int *_pins = NULL;
  size_t _length;
  int _eeprom_address = 0;
  WaterLevelState _state = NONE_WATER_LEVEL, _last_state = NONE_WATER_LEVEL;
  unsigned int _water_level = INITIAL_WATER_LEVEL,
               _last_water_level = INITIAL_WATER_LEVEL,
               _temp_water_level = INITIAL_WATER_LEVEL, _count = 0;

  WATER_LEVEL_DATA _data;

 public:
  WaterLevel();
  WaterLevel(int pins[], size_t length);
  ~WaterLevel();
  void setPins(int pins[], size_t length);
  void setEEPROMAddress(int eeprom_address);
  void loadData(void);
  void saveData(void);

  void check(void);

  unsigned int getWaterLevel(void);
  WaterLevelState getWaterLevelState(void);

  void setMinWaterLevel(unsigned int min_water_level);
  void setMaxWaterLevel(unsigned int max_water_level);
  unsigned int getMinWaterLevel(void);
  unsigned int getMaxWaterLevel(void);

  bool isWateLevelChanged(void);
  bool isWateLevelStateChanged(void);
};

#endif /** WATER_LEVEL_H **/