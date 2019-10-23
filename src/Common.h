
#ifndef WATER_LEVEL_COMMON_H
#define WATER_LEVEL_COMMON_H

/**
 * @brief 수위 상태
 * @date 2019-10-10
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
enum WaterLevelState {
  NONE_WATER_LEVEL,
  GOOD_WATER_LEVEL,
  FLOOD_WATER_LEVEL,
  LACK_WATER_LEVEL,
  ERROR_WATER_LEVEL
};

#endif /** WATER_LEVEL_COMMON_H */