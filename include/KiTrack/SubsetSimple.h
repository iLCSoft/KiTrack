#ifndef SubsetSimple_h
#define SubsetSimple_h

#include <algorithm>

#include "marlin/VerbosityLevels.h"

#include "KiTrack/Subset.h"



namespace KiTrack {
   
   
   template <class T, class QI >
   struct SorterQI
   {
      SorterQI( QI getQI ): _getQI( getQI ){}
      
      bool operator()( T a, T b ){ return ( _getQI( a ) > _getQI( b ) ); }
      
      QI _getQI;
   };
   
 
   /** A class to get the best subset by taking the elements with the highest quality
    * indicator and throwing away all incompatible ones. 
    * Of the remaining elements repeat the procedure. 
    * 
    */
   template <class T >
   class SubsetSimple : public Subset<T> {
      
   public:
      
      
      template< class GetQI, class AreCompatible >
      void calculateBestSet( AreCompatible areCompatible, GetQI getQI );
      
      
   };
   
   


   
   
   template<class T> template<class GetQI, class AreCompatible> 
   void SubsetSimple<T>::calculateBestSet( AreCompatible areCompatible, GetQI getQI ){
      
      
      unsigned nAccepted=0;
      unsigned nRejected=0;
      
      
      
      std::vector< T > elements = this->_elements;
      
      // sort the vector from big QI to small QI
      SorterQI< T, GetQI > sorterQI( getQI );
      sort( elements.begin(), elements.end() , sorterQI );
      
      streamlog_out( DEBUG8 ) << "The elements and their QIs sorted:\n";
      for( unsigned i=0; i < elements.size(); i++ ){
         
         double qi = getQI( elements[i] );
         streamlog_out( DEBUG8 ) << elements[i] << "\t" << qi << "\n";
         
      }
      
      /*The idea here is: the first track is (now that we sorted) the one with the highest
       * QI. Check all other tracks if they are complatible with it.
       * If one of them is incompatible throw it away. Once done with that, store the
       * first track as accepted (and delete it from the vector) and do it once more for
       * the new highest QI track.
       * Repeat until all tracks are stored.
       */   
      while( elements.size() > 0 ){
         
         
         // check all elements with smaller QI if they are compatible
         for( unsigned i=1; i < elements.size(); i++){
            
            
            if( areCompatible( elements[0] , elements[i] ) == false ){ // the track is incompatible
               
               // reject it
               nRejected++;
               this->_rejectedElements.push_back( elements[i] );
               
               streamlog_out( DEBUG7 ) << "reject " << elements[i] << "\n";
               // and delete it from the elements we are looking at
               elements.erase( elements.begin() + i );
               i--;
               
            }
            
         }
         
         // store the first element
         streamlog_out( DEBUG7 ) << "accept " << elements[0] << "\n";
         nAccepted++;
         this->_acceptedElements.push_back( elements[0] );
         
         // delete it from the current tracks
         elements.erase( elements.begin() );
         
      }
      
      
      
      streamlog_out( DEBUG4 ) << "SubsetSimple accepted " << nAccepted 
      << " elements and rejected " << nRejected << " elements of all in all " 
      << nAccepted + nRejected << " elements.\n";
      
      
   }




} // end namespace

#endif
