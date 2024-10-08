#include "Criteria/Crit3_IPCircleDistTimesR.h"


#include <cmath>
#include <sstream>

#include "Criteria/SimpleCircle.h"

using namespace KiTrack;


Crit3_IPCircleDistTimesR::Crit3_IPCircleDistTimesR( float distToCircleMin , float distToCircleMax ){
   
   _distToCircleMax  = distToCircleMax;
   _distToCircleMin  = distToCircleMin;
   
   _name = "Crit3_IPCircleDistTimesR";
   _type = "3Hit";
   
   _saveValues = false;
   
}



bool Crit3_IPCircleDistTimesR::areCompatible( Segment* parent , Segment* child ) {
   

   
   
   if (( parent->getHits().size() == 2 )&&( child->getHits().size() == 2 )){ //a criterion for 2-segments


      IHit* a = child->getHits()[0];
      IHit* b = child->getHits()[1];
      IHit* c = parent-> getHits()[1];
      
      float ax = a->getX();
      float ay = a->getY();
     
      float bx = b->getX();
      float by = b->getY();
      
      float cx = c->getX();
      float cy = c->getY();
      
      
      try{
         
         SimpleCircle circle ( ax , ay , bx , by , cx , cy );
         
         double x = circle.getCenterX();
         double y = circle.getCenterY();
         double R = circle.getRadius();
         
         double circleDistToIPTimesR = fabs( R - sqrt (x*x+y*y) ) * R;
         
         if (_saveValues) _map_name_value["Crit3_IPCircleDistTimesR"] =  circleDistToIPTimesR;
               
         if ( circleDistToIPTimesR  > _distToCircleMax ) return false;
         if ( circleDistToIPTimesR  < _distToCircleMin ) return false;
         
      }
      catch ( const InvalidParameter& ){
         
         
         if (_saveValues) _map_name_value["Crit3_IPCircleDistTimesR"] =  0.;
         
      }
      
      
      
   }
   else{
      
      std::stringstream s;
      s << "Crit3_IPCircleDistTimesR::This criterion needs 2 segments with 2 hits each, passed was a "
       <<  parent->getHits().size() << " hit segment (parent) and a "
       <<  child->getHits().size() << " hit segment (child).";
      
      
      throw BadSegmentLength( s.str() );
      
   }
   
   
   return true;
   
   
}
