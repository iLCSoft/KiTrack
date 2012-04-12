#include "KiTrack/Segment.h"

using namespace KiTrack;

Segment::Segment( std::vector <IHit*> hits){ 

   _hits = hits; 
   
   _state.push_back(0); 
   
   _children.clear(); 
   _parents.clear();

   _active = true;
}



Segment::Segment( IHit* hit){ 
   
   _hits.push_back( hit) ;
   _state.push_back(0); 
   _children.clear(); 
   _parents.clear();
   
   _active = true;
}



void Segment::resetState(){
   
   
   for ( unsigned i = 0; i <_state.size(); i++){
      
      _state[i] = 0;
      
   }
   
}



   

