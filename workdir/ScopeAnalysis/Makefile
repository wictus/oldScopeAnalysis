OS = $(shell uname)
CC = g++ 
LD = $(CC) 
DEBUG_FLAG = -ggdb -g
#######################
COPTS = -std=c++11 -Wall `root-config --cflags` $(DEBUG_FLAG)
LDOPTS = -Wall `root-config --glibs` $(DEBUG_FLAG) -lboost_program_options
#######################
SRC_MODULE = SignalAnalysis/SDARecoOffsetsCalc.cpp SignalAnalysis/SDARecoTimeAtThr.cpp SignalAnalysis/SDAMakePhysSignals.cpp HitAnalysis/SDAMatchHits.cpp HitAnalysis/SDAHitPositions.cpp LORAnalysis/SDAMatchLORs.cpp LORAnalysis/SDATOFs.cpp SignalAnalysis/SDARecoChargeCalc.cpp JPetRecoSignalTools/JPetRecoSignalTools.cpp SignalAnalysis/drawOffset.cpp SignalAnalysis/drawAllOffsets.cpp SignalAnalysis/drawAllCharges.cpp HitAnalysis/SDAHitFindEnergyConstant.cpp OutputModules/bossOutput.cpp JPetHitTools/FindConstant.cpp HitAnalysis/SDAHitCalculateEnergy.cpp HitAnalysis/SDAHitCutOnEnergy.cpp LORAnalysis/SDALORCalculateTimeAtThr.cpp OutputModules/thresholdScan.cpp LORAnalysis/SDAEstimateTimeCalib.cpp LORAnalysis/SDAEstimateTOFCalib.cpp LORAnalysis/SDALORCutOnEnergy.cpp LORAnalysis/SDALORCalculateEnergy.cpp LORAnalysis/SDADoTimeCalib.cpp LORAnalysis/SDADoTOFCalib.cpp OutputModules/twoDRecoOutput.cpp LORAnalysis/SDACalculatePositions.cpp LORAnalysis/SDACalculateTimes.cpp OutputModules/bpOutput.cpp LORAnalysis/SDALORCalculateWalkCorrection.cpp LORAnalysis/SDALORApplyWalkCorrectionToTOF.cpp OutputModules/TOTVSQ.cpp LORAnalysis/SDALORApplyWalkCorrectionToDeltaT.cpp LORAnalysis/SDALORCalculateEnergyFromCalibration.cpp SignalAnalysis/SDARecoAmplitudeCalc.cpp LORAnalysis/SDALORFindAmplitudeConstant.cpp OutputModules/SDALORExctractAmplitudes.cpp LORAnalysis/SDALORCutOnTOT.cpp LORAnalysis/SDALORCalculateTOT.cpp SignalAnalysis/drawAllAmps.cpp SignalAnalysis/SDARecoRisingTimeCalc.cpp SignalAnalysis/SDARecoFallingTimeCalc.cpp SignalAnalysis/SDARecoEnergyResolutionFit.cpp SignalAnalysis/SDARecoAmplitudeCut.cpp SignalAnalysis/SDARecoCutAroundPhotopeak.cpp OutputModules/makeMeanSignal.cpp
HDR_MODULE = $(SRC_MODULE:.cpp=.h)
OBJ_MODULE = $(SRC_MODULE:.cpp=.o)
SRC_MAIN = main.cpp
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)
#######################
EXECUTABLE = $(SRC_MAIN:.cpp=.x)
LOG = JPet.log
#######################
SRC_DICT = moduleDict.cpp
HDR_DICT = $(SRC_DICT:.cpp=.h)
OBJ_DICT = $(SRC_DICT:.cpp=.o)
#######################
SOURCES = $(SRC_MAIN) $(SRC_MODULE) $(SRC_DICT) 
OBJECTS = $(OBJ_MAIN) $(OBJ_MODULE) $(OBJ_DICT)
SHAREDLIB_DIR = ../..
SHAREDLIBS = $(SHAREDLIB_DIR)/libJPetFramework.so #$(SHAREDLIB_DIR)/framework/JPetTreeHeader/libJPetTreeHeader.so 
SHAREDLIBS += $(SHAREDLIB_DIR)/framework/DBHandler/libdbhandler.so
ifeq ($(OS), Darwin)
	LDOPTS += -rpath @loader_path/$(SHAREDLIB_DIR)
endif
#######################
%.o: %.cpp
	$(CC) $^ -o $@ -c $(COPTS)
#######################
# ifndef ROOTSYS
# $(error ROOTSYS is not set)
# endif
#######################
all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS) $(SHAREDLIBS)
	$(LD) -o $@ $^ $(LDOPTS)
# $(OBJ_MODULE): $(SRC_MODULE)
# $(OBJ_MAIN): $(SRC_MAIN)
$(SHAREDLIBS): 
	cd ../..; $(MAKE) modules sharedlib;
$(SRC_DICT):
	rootcint -f $@ -c -P -I$(ROOTSYS) $(HDR_MODULE) LinkDef.h

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(SRC_DICT) $(HDR_DICT) $(LOG) 

cleandata:
	rm -f *.tslot.*.root *.phys.*.root *.sig.root
