/****************************************************************************
*
* This is a part of the TOTEM offline software.
* Authors: 
*   Jan Kašpar (jan.kaspar@gmail.com)
*
****************************************************************************/

#ifndef EventFilter_TotemRawToDigi_RawToDigiConverter
#define EventFilter_TotemRawToDigi_RawToDigiConverter

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "EventFilter/TotemRawToDigi/interface/VFATFrameCollection.h"

#include "CondFormats/TotemReadoutObjects/interface/TotemDAQMapping.h"
#include "CondFormats/TotemReadoutObjects/interface/TotemAnalysisMask.h"

#include "DataFormats/TotemDigi/interface/TotemRPDigi.h"
#include "DataFormats/TotemDigi/interface/TotemVFATStatus.h"

//----------------------------------------------------------------------------------------------------

/// \brief Collection of code to convert TOTEM raw data into digi.
class RawToDigiConverter
{
  public:
    RawToDigiConverter(const edm::ParameterSet &conf);

    /// Creates RP digi.
    void Run(const VFATFrameCollection &coll, const TotemDAQMapping &mapping, const TotemAnalysisMask &mask,
      edm::DetSetVector<TotemRPDigi> &digi, edm::DetSetVector<TotemVFATStatus> &status);

    /// Print error summaries.
    void PrintSummaries();

  private:
    struct Record
    {
      const TotemVFATInfo *info;
      const VFATFrame *frame;
      TotemVFATStatus status;
    };

    unsigned char verbosity;
    
    unsigned int printErrorSummary;
    unsigned int printUnknownFrameSummary;

    enum TestFlag { tfNoTest, tfWarn, tfErr };

    /// flags for which tests to run
    unsigned int testFootprint;
    unsigned int testCRC;
    unsigned int testID;
    unsigned int testECRaw;
    unsigned int testECDAQ;
    unsigned int testECMostFrequent;
    unsigned int testBCMostFrequent;

    /// the minimal required number of frames to determine the most frequent counter value
    unsigned int EC_min, BC_min;

    /// the minimal required (relative) occupancy of the most frequent counter value to be accepted
    double EC_fraction, BC_fraction;

    /// error summaries
    std::map<TotemFramePosition, std::map<TotemVFATStatus, unsigned int> > errorSummary;
    std::map<TotemFramePosition, unsigned int> unknownSummary;

    /// Common processing for all VFAT based sub-systems.
    void RunCommon(const VFATFrameCollection &input, const TotemDAQMapping &mapping,
      std::map<TotemFramePosition, Record> &records);
};

#endif
