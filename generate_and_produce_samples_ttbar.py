import os

import FWCore.ParameterSet.Config as cms

for i in range(2000):
  f=open("submit_ttbar_"+str(i)+".sh","w")
  f.write("""OUTFILES="*.txt"\n
SEOUTFILES="TT*.root"\n
HN_NAME=hinzmann\n
DBG=0\n
USER_SRM_HOME="srm://t3se01.psi.ch:8443/srm/managerv2?SFN=/pnfs/psi.ch/cms/trivcat/store/user/"\n
TOPWORKDIR=/scratch/`whoami`\n
JOBDIR=TTbar_new_"""+str(i)+"""_TP\n
WORKDIR=$TOPWORKDIR/$JOBDIR\n
RESULTDIR=$STARTDIR/$JOBDIR\n
source /afs/cern.ch/cms/cmsset_default.sh\n

cd /shome/hinzmann/Phase2UpgradeStudies/CMSSW_6_2_0_SLHC20/src/\n
eval `scram runtime -sh`\n
mkdir -p $WORKDIR\n
cd $WORKDIR\n
cmsRun /shome/hinzmann/Phase2UpgradeStudies/CMSSW_6_2_0_SLHC20/src/generate_ttbar.py

cd /shome/hinzmann/Phase2UpgradeStudies/CMSSW_5_3_24/src/Delphes\n
eval `scram runtime -sh`\n
export PYTHIA8=`pwd`/external/pythia8201\n
STARTDIR=`pwd`\n
cd $WORKDIR\n
/shome/hinzmann/Phase2UpgradeStudies/CMSSW_5_3_24/src/Delphes/DelphesCMSFWLite /shome/hinzmann/Phase2UpgradeStudies/CMSSW_5_3_24/src/Delphes/Cards/JetStudies_Phase_II_140PileUp_conf4_TP.tcl TTbar_new_"""+str(i)+"""_TP.root ttbar.root\n
source /shome/hinzmann/Phase2UpgradeStudies/CMSSW_5_3_24/src/Delphes/job_copy.sh\n

""")
  f.close()

  os.system("qsub -o /shome/hinzmann/jobs -e /shome/hinzmann/jobs -q all.q submit_ttbar_"+str(i)+".sh")
