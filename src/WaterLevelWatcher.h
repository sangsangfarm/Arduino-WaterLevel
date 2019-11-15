/**
 * @file WaterLevelWatcher.h
 * @brief Water level watcher
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#ifndef WATER_LEVEL_WATCHER_H
#define WATER_LEVEL_WATCHER_H

#include <ArduinoJson.h>
#include <EEPROM.h>
#include <time.h>
#include "Common.h"
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <HTTPClient.h>
#endif

#ifndef EEPROM_SIZE
#define EEPROM_SIZE 4096
#endif

#define JSON_LEN 256

// DEBUG
#ifndef WATER_LEVEL_WATCHER_DEBUG_PORT
#define WATER_LEVEL_WATCHER_DEBUG_PORT Serial
#endif

#ifdef WATER_LEVEL_WATCHER_DEBUG
#define print(...) WATER_LEVEL_WATCHER_DEBUG_PORT.print(__VA_ARGS__)
#define printf(...) WATER_LEVEL_WATCHER_DEBUG_PORT.printf(__VA_ARGS__)
#define println(...) WATER_LEVEL_WATCHER_DEBUG_PORT.println(__VA_ARGS__)

#else
#define print(...)
#define printf(...)
#define println(...)
#endif
/**
 * @struct WATER_LEVEL_WATCHER_DATA
 * @brief Water level watcher's EEPROM data
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
struct WATER_LEVEL_WATCHER_DATA {
  /** Data storage state */
  bool is_saved;
  /** Water level watcher's power state */
  bool power;
  /** Water level watcher's URL */
  char url[128];
  /** Device name controlled by water level watcher */
  char device_name[20];
  /** Outlet number controlled by water level watcher */
  int outlet;
  /** The last time of watering by water level watcher */
  time_t last_watering_time;
  /** The time of water flowing by water level watcher */
  long water_flow_time;
  /** The interval time of watering by water level watcher */
  long watering_interval_time;
};

/**
 * @brief Water level watcher
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
class WaterLevelWatcher {
 private:
  WATER_LEVEL_WATCHER_DATA _water_level_watcher_data;
  WaterLevelState _state;
  bool _water_level_change_flag = false;

 public:
  WaterLevelWatcher(void);
  void watch(void);
  bool isWaterLevelChanged(void);

  bool getPower(void);
  void setPower(bool power);

  char *getUrl(void);
  void setUrl(char *url);

  char *getDeviceName(void);
  void setDeviceName(char *device_name);

  int getOutlet(void);
  void setOutlet(int outlet);

  time_t getLastWateringTime(void);
  void setLastWateringTime(time_t last_watering_time);

  long getWaterFlowTime(void);
  void setWaterFlowTime(long water_flow_time);

  long getWateringIntervalTime(void);
  void setWateringIntervalTime(long watering_interval_time);

  bool isTimeOver(void);
  bool isTimeToWatering(void);

  WaterLevelState getState(void);
};

#endif /** WATER_LEVEL_WATCHER_H **/