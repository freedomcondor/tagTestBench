#include "LEDprojection.h"

std::vector<CVector2> LEDprojection(const CRotationMatrix3& c_camera, 
                      const CRotationMatrix3& c_rotation, 
                      const CVector3& c_location, 
                      const Real f_ledHalfDis)
{
   std::array<CVector3, 4> arrLed3DLocations{
      CVector3( f_ledHalfDis, 0.0f, 0.0f),
      CVector3( 0.0f, f_ledHalfDis, 0.0f),
      CVector3( -f_ledHalfDis, 0.0f, 0.0f),
      CVector3( 0.0f, -f_ledHalfDis, 0.0f),
   };

   std::vector<CVector2> arrLed2D;

   for (CVector3 cLed3DLocation : arrLed3DLocations) {
      cLed3DLocation = c_rotation * cLed3DLocation;
      cLed3DLocation += c_location;
      CVector3 cLed3DProjection = c_camera * cLed3DLocation;
      arrLed2D.push_back(CVector2(cLed3DProjection.GetX()/cLed3DProjection.GetZ(), 
                                  cLed3DProjection.GetY()/cLed3DProjection.GetZ()));
   }

   return arrLed2D;
}
