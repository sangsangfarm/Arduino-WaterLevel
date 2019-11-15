/**
 * @file WaterLevelWatcher.cpp
 * @brief Water level watcher
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#include <WaterLevelWatcher.h>

/**
 * @fn WaterLevelWatcher::WaterLevelWatcher(void)
 * @brief WaterLevelWatcher constructor
 * @return WaterLevelWatcher object
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevelWatcher::WaterLevelWatcher(void) {}

/**
 * @fn void WaterLevelWatcher::watch(void)
 * @brief Get water level state from url
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::watch(void) {
  printf("[WaterLevelWatcher] Power state : %s\n",
         _water_level_watcher_data.power ? "ON" : "OFF");
  if (!_water_level_watcher_data.power ||
      strncmp(_water_level_watcher_data.url, "", sizeof("")) == 0 ||
      _water_level_watcher_data.outlet == -1) {
    _water_level_change_flag = false;
    return;
  }
  WaterLevelState state = _state;
  HTTPClient http;
  String url = String(_water_level_watcher_data.url);
  http.begin(url);
  http.setConnectTimeout(1000);
  println(url);
  http.addHeader("Content-Type", "application/json");
  int http_code = http.GET();
  String result = http.getString();
  println(result);
  printf("[WaterLevelWatcher] Http Code : %d\n", http_code);
  http.end();
  DynamicJsonDocument root(JSON_LEN);
  DeserializationError error = deserializeJson(root, result);
  if (200 <= http_code && http_code < 300) {
    if (error) {
      printf("[WaterLevelWatcher] Parse %s failed.\r\n", result.c_str());
      println(error.c_str());
      state = ERROR_WATER_LEVEL;
    }
    if (!root["SensorState"].isNull()) {
      state = (WaterLevelState)(int)root["SensorState"];
    }
  } else {
    state = ERROR_WATER_LEVEL;
  }

  root.clear();
  if (_state != state) {
    _water_level_change_flag = true;
    _state = state;
  } else {
    _water_level_change_flag = false;
  }
}

/**
 * @fn bool WaterLevelWatcher::getPower(void)
 * @brief Get water level watcher's power state
 * @return Water level watcher's power state
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevelWatcher::getPower(void) {
  return _water_level_watcher_data.power;
}

/**
 * @fn void WaterLevelWatcher::setPower(bool power)
 * @brief Set water level watcher's power state
 * @param power Power state to be set up
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setPower(bool power) {
  _water_level_watcher_data.power = power;
}
/**
 * @fn bool WaterLevelWatcher::isWaterLevelChanged(void)
 * @brief Check water level change
 * @return Change in water level
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevelWatcher::isWaterLevelChanged(void) {
  return _water_level_change_flag;
}

/**
 * @fn char *WaterLevelWatcher::getUrl(void)
 * @brief Get water level watcher's URL
 * @return URL
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
char *WaterLevelWatcher::getUrl(void) { return _water_level_watcher_data.url; }

/**
 * @fn void WaterLevelWatcher::setUrl(char *url)
 * @brief Set water level watcher's URL
 * @param url URL to be set up
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setUrl(char *url) {
  memset(_water_level_watcher_data.url, 0,
         sizeof(_water_level_watcher_data.url));
  memcpy(_water_level_watcher_data.url, url, strlen(url) + 1);
}

/**
 * @fn char *WaterLevelWatcher::getDeviceName(void)
 * @brief Get device name controlled by water level watcher
 * @return Device name
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
char *WaterLevelWatcher::getDeviceName(void) {
  return _water_level_watcher_data.device_name;
}

/**
 * @fn void WaterLevelWatcher::setOutlet(int outlet)
 * @brief Set device name controlled by water level watcher
 * @param device_name Device name to be set up
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setDeviceName(char *device_name) {
  memset(_water_level_watcher_data.device_name, 0,
         sizeof(_water_level_watcher_data.device_name));
  memcpy(_water_level_watcher_data.device_name, device_name,
         strlen(device_name) + 1);
}

/**
 * @fn int WaterLevelWatcher::getOutlet(void)
 * @brief Get outlet number controlled by water level watcher
 * @return Outlet number
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
int WaterLevelWatcher::getOutlet(void) {
  return _water_level_watcher_data.outlet;
}

/**
 * @fn void WaterLevelWatcher::setOutlet(int outlet)
 * @brief Set outlet number controlled by water level watcher
 * @param outlet Outlet number to be set up
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setOutlet(int outlet) {
  _water_level_watcher_data.outlet = outlet;
}

/**
 * @fn WaterLevelState WaterLevelWatcher::getState(void)
 * @brief Get water level state
 * @return Water level state
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevelState WaterLevelWatcher::getState(void) {
  printf("[WaterLevelWatcher] state: %d\n", _state);
  return _state;
}

/**
 * @fn void WaterLevelWatcher::setLastWateringTime(time_t last_watering_time)
 * @brief Set the last watering time by water level watcher
 * @param last_watering_time The last time of watering to be set up
 * @return void
 * @date 2019-11-14
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setLastWateringTime(time_t last_watering_time) {
  _water_level_watcher_data.last_watering_time = last_watering_time;
}

/**
 * @fn time_t WaterLevelWatcher::getLastWateringTime(void)
 * @brief Get the last watering time by water level watcher
 * @return The last time of watering
 * @date 2019-11-14
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
time_t WaterLevelWatcher::getLastWateringTime(void) {
  return _water_level_watcher_data.last_watering_time;
}

/**
 * @fn void WaterLevelWatcher::setWaterFlowTime(long water_flow_time)
 * @brief Set the water flow time by water level watcher
 * @param water_flow_time The water flow time to be set up
 * @return void
 * @date 2019-11-14
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setWaterFlowTime(long water_flow_time) {
  _water_level_watcher_data.water_flow_time = water_flow_time;
}

/**
 * @fn long WaterLevelWatcher::getWaterFlowTime(void)
 * @brief Get the water flow time by water level watcher
 * @return The water flow time
 * @date 2019-11-14
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
long WaterLevelWatcher::getWaterFlowTime(void) {
  return _water_level_watcher_data.water_flow_time;
}

/**
 * @fn void WaterLevelWatcher::setWateringIntervalTime(long
 * watering_interval_time)
 * @brief Set the interval time of watering by water level watcher
 * @param watering_interval_time The interval time of watering to be set up
 * @return void
 * @date 2019-11-14
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setWateringIntervalTime(long watering_interval_time) {
  _water_level_watcher_data.watering_interval_time = watering_interval_time;
}

/**
 * @fn long WaterLevelWatcher::getWateringIntervalTime(void)
 * @brief Get the interval time of watering by water level watcher
 * @return The interval time of watering
 * @date 2019-11-14
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
long WaterLevelWatcher::getWateringIntervalTime(void) {
  return _water_level_watcher_data.watering_interval_time;
}

/**
 * @fn bool WaterLevelWatcher::isTimeOver(void)
 * @brief Check watering timeout
 * @return timeout or not
 * @date 2019-11-15
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevelWatcher::isTimeOver(void) {
  time_t now = time(nullptr);
  return now - _water_level_watcher_data.last_watering_time >
         _water_level_watcher_data.water_flow_time;
}

/**
 * @fn bool WaterLevelWatcher::isTimeToWatering(void)
 * @brief Check the time to water.
 * @return the time to water or not
 * @date 2019-11-15
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevelWatcher::isTimeToWatering(void) {
  time_t now = time(nullptr);
  return now - _water_level_watcher_data.last_watering_time >
         _water_level_watcher_data.watering_interval_time;
}