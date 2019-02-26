/* -*-c++-*--------------------------------------------------------------------
 * 2019 Bernd Pfrommer bernd.pfrommer@gmail.com
 */

#include "tagslam/camera_intrinsics2.h"
#include "tagslam/utils.h"
#include <ros/console.h>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

using std::string;

namespace tagslam {

  static void bombout(const std::string &param, const std::string &name) {
    ROS_ERROR_STREAM("cannot find " << param << " " + name);
    throw (std::runtime_error("cannot find " + param + " " + name));
  }
  static std::map<std::string, DistortionModel> distMap = {
    {"rad_tan", RADTAN}, {"radtan", RADTAN}, {"plumb_bob", RADTAN},
    {"equidistant", EQUIDISTANT}, {"equi", EQUIDISTANT},
    {"fisheye", EQUIDISTANT}};

  CameraIntrinsics2
  CameraIntrinsics2::parse(const std::string &prefix,
                          const ros::NodeHandle &nh) {
    CameraIntrinsics2 ci;
    if (!nh.getParam(prefix + "camera_model", ci.cameraModel_)) {
      bombout("camera_model", prefix); }
    std::string distModel;
    if (!nh.getParam(prefix + "distortion_model", distModel)) {
      bombout("distortion_model", prefix); }
    if (distMap.count(distModel) == 0) {
      ROS_ERROR_STREAM("unknown distortion model: " << distModel);
      throw (std::runtime_error("unknown distortion model: " + distModel));
    }
    ci.distortionModel_ = distMap[distModel];
    if (!nh.getParam(prefix + "distortion_coeffs", ci.distortionCoeffs_)) {
      bombout("distortion_coeffs", prefix); }
    if (!nh.getParam(prefix + "intrinsics",  ci.K_)) {
      bombout("intrinsics", prefix); }
    if (!nh.getParam(prefix + "resolution",  ci.resolution_)) {
      bombout("resolution", prefix); }
    return (ci);
  }
}
