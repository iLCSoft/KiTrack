#ifndef Segment_h
#define Segment_h

#include <vector>
#include <list>
#include <string>

#include "KiTrack/IHit.h"

namespace KiTrack{

   
   /** A Segment is something like a track or a part of a track: it consists of hits linked together.
    *
    * It is used by the class Automaton, which is a cellular automaton used for tracking.
    * 
    */   
   class Segment {
   
   
   public:
         
         Segment( std::vector <IHit*> hits);
         Segment( IHit* hit);
         
         
         void deleteParent ( Segment* delParent ){ _parents.remove( delParent );};
         void deleteChild ( Segment* delChild ){ _children.remove( delChild );};
         
         
         std::list <Segment*> getChildren() { return _children;};
         std::list <Segment*> getParents()  { return _parents;};
         
         std::vector <IHit*> getHits()const {return _hits;};
         
         void addChild( Segment* child ){ _children.push_back(child); };
         void addParent( Segment* parent ){ _parents.push_back(parent); };
         
         unsigned getLayer()const { return _layer; };
         void setLayer( unsigned layer ) { _layer = layer; }; 
         
         std::vector<int>& getState() { return _state; }; 
         
         void raiseState() { if (_state.size() > 0) _state[0]++; };
         int getInnerState()const { return _state[0];}; 
         int getOuterState()const { return _state.back();}; 
         void resetState();
         
         void setSkippedLayers( unsigned skippedLayers ){ _state.resize( skippedLayers + 1 );}
         
         bool isActive() const { return _active;}
         void setActive( bool active ){ _active = active; }
         
         /** @return infos about the segment */
         std::string getInfo();
     
   private:
         
         std::list <Segment*> _children; 
         std::list <Segment*> _parents;
         
         
   
         std::vector <IHit*> _hits;
         
         std::vector<int> _state;
         
         unsigned _layer;
         bool _active;
      
   };


} //end of KiTrack Namespace

#endif

