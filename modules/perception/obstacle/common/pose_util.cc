/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/perception/obstacle/common/pose_util.h"

namespace apollo {
namespace perception {

bool ReadPoseFile(const std::string& filename,
  Eigen::Matrix4d* pose,
  int* frame_id,
  double* time_stamp) {
  std::ifstream ifs(filename.c_str());
  if (!ifs.is_open()) {
    std::cerr << "Failed to open file " << filename << std::endl;
    return false;
  }
  char buffer[1024];
  ifs.getline(buffer, 1024);
  int id = 0;
  double time_samp = 0;
  double quat[4];
  double matrix3x3[9];
  double& pose03 = (*pose)(0, 3);
  double& pose13 = (*pose)(1, 3);
  double& pose23 = (*pose)(2, 3);
  sscanf(buffer, "%d %lf %lf %lf %lf %lf %lf %lf %lf", &id, &(time_samp),
    &pose03, &pose13, &pose23,
    &(quat[0]), &(quat[1]), &(quat[2]), &(quat[3]));
  QuaternionToRotationMatrix<double>(quat, matrix3x3);

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      (*pose)(i, j) = matrix3x3[i * 3 + j];
    }
  }

  (*frame_id) = id;
  (*time_stamp) = time_samp;
  return true;
}

}  // namespace perception
}  // namespace apollo
