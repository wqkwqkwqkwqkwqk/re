#ifndef __REPLACE_INIT_PLACE__
#define __REPLACE_INIT_PLACE__

#include <Eigen/SparseCore>
#include <opendb/db.h>

namespace replace {

class PlacerBase;
class InitialPlaceVars {
public:
  int maxInitialPlaceIter;
  int minDiffLength;
  int solverIterMax;
  float netWeightScale;
  int verbose;
  InitialPlaceVars();
  void reset();
};

typedef Eigen::SparseMatrix<float, Eigen::RowMajor> SMatrix;

class InitialPlace {
  public:
    InitialPlace();
    InitialPlace(PlacerBase* placerBase);
    ~InitialPlace();
    
    void reset();
    void setInitialPlaceVars(InitialPlaceVars initialPlaceVars);
    void doBicgstabPlace();

  private:
    InitialPlaceVars initialPlaceVars_;
    PlacerBase* pb_;

    // Solve two SparseMatrix equations here;
    //
    // find xcgX_ s.t. satisfies matX_ * xcgX_ = xcgB_ 
    // find ycgX_ s.t. satisfies matY_ * ycgX_ = ycgB_
    //
    // xcgX_ : current/target instances' center X coordinates. 1-col vector.
    // ycgX_ : current/target instances' center Y coordinates. 1-col vector.
    //
    // xcgB_ : contains fixed instances' forces toward X coordi. 1-col vector.
    // ycgB_ : contains fixed instances' forces toward Y coordi. 1-col vector.
    //
    // matX_ : SparseMatrix that contains connectivity forces on X // B2B model is used
    // matY_ : SparseMatrix that contains connectivity forces on Y // B2B model is used
    // 
    // Used the interative BiCGSTAB solver to solve matrix eqs.
    
    Eigen::VectorXf xcgX_, xcgB_;
    Eigen::VectorXf ycgX_, ycgB_;
    SMatrix matX_, matY_;

    void placeInstsCenter();
    void setPlaceInstExtId();
    void updatePinInfo();
    void createSparseMatrix();
    void updateCoordi();
};

}
#endif
