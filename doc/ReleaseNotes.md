# v01-10-01

* 2024-08-22 tmadlener ([PR#4](https://github.com/iLCSoft/KiTrack/pull/4))
  - Move CI to github actions and remove travis-ci config
  - Fix c++ warnings and enable `-Werror` for CI

# v01-10

* 2019-08-09 Frank Gaede ([PR#2](https://github.com/iLCSoft/KiTrack/pull/2))
  - make compatible w/ c++17
        - remove all throw() declarations
        - replace std::random_shuffle w/ std::shuffle<..., std::mt19937>

# v01-09

* 2018-01-31 Frank Gaede ([PR#1](https://github.com/iLCSoft/KiTrack/pull/1))
  - fix all compiler warnings (gcc54-ub1604)
       - only "member initialization" from effc++

# v01-08


# v01-07
F. Gaede
* made compatible with c++11
* removed -ansi -pedantic -Wno-long-long

# v01-06
* Crit3_PT_MV: adding criterio for minimum PT to mini-vector tracking (Y.Voutsinas)

* v01-05
* Use the KiTrack core tools for standalone tracking at VXD based on mini* vectors
* Importing a first set of criteria for VXD tracking
* Extending IHit interface to accomodate mini* vectors

# v01-04
* SegmentBuilder: added output of failed criterion
* SubsetHopfieldNN: set the activation threshold down to 0.5

# v01-03
* Added validity check of parameters in the HopfielNeuralNet

# v01-02
* Changed order of hits in segments to start with innermost layer at [0]
* Added somed info methods for better debugging
* Prevent different criteria (all with self calculated angles) from rounding errors
* Automaton: Added a counter of the connections between the segments
* Changed order of hits in segments to start with innermost layer at [0]
* Added somed info methods for better debugging
* Prevent different criteria (all with self calculated angles) from rounding errors
 
# v01-01
* Erased track bound subset classes ITrackSubset, TrackSubsetHopfieldNN and TrackSubsetSimple as their functionality is now covered by the template classes Subset, SubsetSimple and SubsetHopfieldNN.
* Added documentation of the Hopfield Neural Network (for classes HopfielNeuralNet and SubsetHopfieldNN)
* Made setter and getter functions for parameters in SubsetHopfieldNN
* Added new classes for SubsetFinding: 
 * Subset, SubsetSimple, SubsetHopfieldNN
 * They are based on the classes: ITrackSubset, TrackSubsetSimple and TrackSubsetHopfieldNN
 * The difference is they are templated, thus the basic algorithms can be used with any class.
 * So if one wants for example to use the Hopfield Network Subset with an own track class or with
 * something that is not even a track, this can now be done.
* Added doxygen mainpage
* Added new criteria, that are modified versions of the existing ones, but are taking into account the dependence on pt
* Commented out some old criteria in Criteria.cc, so they don't belong to the standard criteria any more
 

# v01-00
* first version: splitted the software from the ForwardTracking package at version v01-02
* Moved SimpleCircle from KiTrack to Criteria (as it is used by them)
* Commented out drawSegments() in Automaton, because it is dependent on the framework (functionality should be restored in a different way)