#ifndef SubsetHopfieldNN_h
#define SubsetHopfieldNN_h

#include <CLHEP/Random/RandFlat.h>

#include "KiTrack/Subset.h"
#include "KiTrack/HopfieldNeuralNet.h"



namespace KiTrack {
 
   /** A class to get the best subset with help of a Hopfield Neural Network
    * 
    */
   template< class T >
   class SubsetHopfieldNN : public Subset<T>{
      
   public:
      
      template< class GetQI, class AreCompatible >
      void calculateBestSet( AreCompatible areCompatible, GetQI getQI );
      
      
   
   };
   
   
   template< class T > template< class GetQI, class AreCompatible >
   void SubsetHopfieldNN<T>::calculateBestSet( AreCompatible areCompatible, GetQI getQI ){
      
      
      unsigned nAccepted=0;
      unsigned nRejected=0;
      unsigned nCompWithAll=0;
      unsigned nIncompatible=0;
      
      
      std::vector< T > elements = this->_elements; //this pointer is needed here, because of the template!
      
      unsigned nElements = elements.size();
      
      // the information for the Hopfield Neural Network:
      
      std::vector < std::vector <bool> > G; // a matrix telling, if two neurons (elements) are compatible
      G.resize( nElements );
      for (unsigned i=0; i<nElements; i++) G[i].resize( elements.size() );
      
      std::vector < double > QI ; // the quality indicators of the neurons (elements)
      QI.resize( nElements );
      
      std::vector < double > states; // the initial state to start from.
      states.resize( nElements );
      
      
      double omega = 0.75;
      
      double initStateMin = 0.;
      double initStateMax = 0.1;
      
      
      /**********************************************************************************************/
      /*                1. Find out which elements are compatible and get the QIs                     */
      /**********************************************************************************************/
      
      
      for ( unsigned i=0; i < nElements ; i++){ //over all elements
         
         
         T elementA = elements[i]; //the track we want to look at.
         
         // Get the quality
         QI[i] = getQI( elementA );
         
         streamlog_out(DEBUG3) << "QI of element " << i << " = " << QI[i] << "\n";
         
         
         // Set an initial state
         states[i] = CLHEP::RandFlat::shoot ( initStateMin , initStateMax ); //random ( uniformly ) values from initStateMin to initStateMax
         
         
         // Fill the states in the G matrix. (whether two elements are compatible or not
         for ( unsigned j=i+1; j < nElements ; j++ ){ // over all elements that come after the current one (TODO: explain, why not previous ones too)
            
            T elementB = elements[j]; // the track we check if it is in conflict with trackA
            
            if ( areCompatible( elementA , elementB ) ){ 
               
               G[i][j] = 0;
               G[j][i] = 0;
               
            }
            else{
               
               G[i][j] = 1;
               G[j][i] = 1;            
               
            }
            
         }
         
      }
      
      // output of the G matrix:
      if( !G.empty() ){
         
         streamlog_out(DEBUG7) << "G:\n";
         
         
         for ( unsigned i=0; i < G.size(); i++ ){
            
            
            for ( unsigned j=0; j < G[i].size(); j++ ){
               
               streamlog_out(DEBUG7) << G[i][j] << "  ";
               
            }
            
            streamlog_out(DEBUG7) << "\n";
            
         }
        
      }
      
      
      /**********************************************************************************************/
      /*                2. Save elements, that are compatible with all others                         */
      /**********************************************************************************************/
      
      for( unsigned i=0; i < elements.size(); i++ ){
         
         
         bool isCompatibleWithAll = true;
         
         //check if this track is compatible with all others
         for( unsigned j=0; j < elements.size(); j++){
            
            //G[i][j] == 0 i and j are compatible, if G[i][j] == 1 i and j are incompatible
            if( (i != j) && G[i][j] ){
               
               isCompatibleWithAll = false;
               break;
               
            }
            
         }
         
         
         if ( isCompatibleWithAll ){ //if it is compatible with all others, we don't need the Hopfield Neural Net, we can just save it
            
            
            //add the track to the good ones
            this->_acceptedElements.push_back( elements[i] );
            nCompWithAll++;
            
            
            //And now erase it from the ones we will still check:
            elements.erase( elements.begin() + i );
            states.erase( states.begin() + i );
            QI.erase( QI.begin() + i );
            
            for( unsigned j=0; j<G.size(); j++ ) G[j].erase( G[j].begin() + i );
                     G.erase( G.begin() + i );
            
            i--;
            
         }
         else{
            
            nIncompatible++;
            
         }
         
      }
      
      
      streamlog_out( DEBUG3 ) << nCompWithAll << " elements are compatible with all others, " << nIncompatible
      << " elements are interfering and will be checked for the best subset\n";
      
      
      
      /**********************************************************************************************/
      /*                3. Let the Neural Network perform to find the best subset                   */
      /**********************************************************************************************/  
      
      if( !elements.empty() ){
         
         HopfieldNeuralNet net( G , QI , states , omega);
         
         net.setT (2.1);
         net.setTInf(0.1);
         net.setLimitForStable(0.01);
         
         unsigned nIterations=1;
         
         streamlog_out(DEBUG6) << "states: ( ";
         for ( unsigned int i=0; i< states.size(); i++) streamlog_out(DEBUG6) << states[i] << " "; 
         streamlog_out(DEBUG6) << ")\n";
         
         while ( !net.doIteration() ){ // while the Neural Net is not (yet) stable
            
            nIterations++;
            
            std::vector <double> newStates = net.getStates();
            
            streamlog_out(DEBUG6) << "states: ( ";      
            
            for ( unsigned int i=0; i< newStates.size(); i++) streamlog_out(DEBUG6) << newStates[i] << " "; 
            
            streamlog_out(DEBUG6) << ")\n";
            
         }
         
         
         
         streamlog_out( DEBUG3 ) << "Hopfield Neural Network is stable after " << nIterations << " iterations.\n";
         
         
         
         
         /**********************************************************************************************/
         /*                4. Now just sort the elements into accepted and rejected ones                 */
         /**********************************************************************************************/  
         
         
         states = net.getStates();
         
         double activationMin = 0.75; // the minimal value of state to be accepted
         
         
         for ( unsigned i=0; i < states.size(); i++ ){
            
            
            if ( states[i] >= activationMin ){
               
               this->_acceptedElements.push_back( elements[i] );
               nAccepted++;
               
            }
            else{
               
               this->_rejectedElements.push_back( elements[i] );
               nRejected++;
               
            }
            
         }
         
      }
      
      
      streamlog_out( DEBUG3 ) << "Hopfield Neural Network accepted " << nAccepted 
      << " elements and rejected " << nRejected << " elements of all in all " 
      << nAccepted + nRejected << "incomaptible elements.\n";
      
      streamlog_out( DEBUG3 )   << "So in sum " << nAccepted + nCompWithAll
      << " elements survived and " << nRejected << " elements got rejected.\n";
      
      
   }
   
   
}


#endif

