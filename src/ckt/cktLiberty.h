/****************************************************************************
  FileName     [ cktLiberty.h ]
  PackageName  [ ckt ]
  Synopsis     [ Liberty timing info. for ckt package ]
  Author       [ Hu-Hsi (Louis) Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ] 
****************************************************************************/

#ifndef CKT_LIBERTY_H
#define CKT_LIBERTY_H

#include <vector>
#include <cktEnum.h>

struct lu_table_template
{
   unsigned _dim   :  2;
   unsigned _size1 : 10;
   unsigned _size2 : 10;
   unsigned _size3 : 10;
   vector<VAR_TYPE> _type;
   vector<float>    _index1;
   vector<float>    _index2;
   vector<float>    _index3;
};

class CktGeneralInfo
{
  public:
     CktGeneralInfo()  {}
     ~CktGeneralInfo() {}
  private:
     float default_inout_pin_cap;
     float default_input_pin_cap;
     float default_output_pin_cap;
     float default_max_fanout;
     float default_max_transition;
     float default_fanout_load;
     float default_cell_leakage_power;
     float default_leakage_power_density;

     float slew_derate_from_library;
     float slew_lower_threshold_pct_rise;
     float slew_upper_threshold_pct_rise;
     float slew_lower_threshold_pct_fall;
     float slew_upper_threshold_pct_fall;
     float input_threshold_pct_fall;
     float output_threshold_pct_fall;
     float input_threshold_pct_rise;
     float output_threshold_pct_rise;

     vector<lu_table_template*> table_template_ary;

};

class CktTimingLut
{
  public:
     CktTimingLut()  {}
     ~CktTimingLut() {}
  private:
     TRAN_TYPE          _TranType;
     lu_table_template* _templateType;
     CktTimingLut*      _next;
     vector<float>      _value;
};

class CktOPinTime
{
  public:
     CktOPinTime()  {}
     ~CktOPinTime() {}
  private:
     unsigned      _timing_type  : 16;
     unsigned      _timing_sense : 16;
     CktLibInPin*  _related_pin;
     CktTimingLut* _timingTable;
};

struct CktLibInPin
{
   float capacitance;
   float fanout_load;
};

struct CktLibOutPin
{
   float max_transition;
   CktOPinTime _cktOPinTime;
};

class CktDelayInfo
{
  public:
     CktDelayInfo()  {}
     ~CktDelayInfo() {}
     
     static CktGeneralInfo _generalInfo;
  private:
     float area;
     CktLibInPinAry  _InPinTimeAry;
     CktLibOutPinAry _OutPinTimeAry;
};

#endif


