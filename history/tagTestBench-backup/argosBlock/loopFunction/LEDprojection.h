#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/apriltag_pose.h>

#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/vector2.h>

using namespace argos;

std::vector<CVector2> LEDprojection(const CRotationMatrix3& c_camera, 
                      const CRotationMatrix3& c_rotation, 
                      const CVector3& c_location, 
                      const Real f_ledHalfDis);
