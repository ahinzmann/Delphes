from BaseControlPlots import BaseControlPlots

# Requirements:
# event.jets
# event.bjets
# event.MEt

class JetControlPlots(BaseControlPlots):
    """A class to create control plots for jetmet"""

    def __init__(self, dir=None, dataset=None, mode="plots"):
      # create output file if needed. If no file is given, it means it is delegated
      BaseControlPlots.__init__(self, dir=dir, purpose="jetmet", dataset=dataset, mode=mode)

    def beginJob(self):
      # declare histograms
      self.add("Njets",";N jets (p_{T}>30 GeV);N",10,0,10)
      self.add("Jet1Pt",";Jet Pt;N",100,0,500)
      self.add("Jet1Eta",";Jet Eta;N",100,-5,5)
      self.add("Jet1Phi",";Jet Phi;N",64,-3.2,3.2)
      self.add("Jet2Pt",";Jet Pt;N",100,0,500)
      self.add("Jet2Eta",";Jet Eta;N",100,-5,5)
      self.add("Jet2Phi",";Jet Phi;N",64,-3.2,3.2)
      self.add("Jet3Pt",";Jet Pt;N",100,0,500)
      self.add("Jet3Eta",";Jet Eta;N",100,-5,5)
      self.add("Jet3Phi",";Jet Phi;N",64,-3.2,3.2)
      self.add("Jet4Pt",";;Jet Pt;N",100,0,500)
      self.add("Jet4Eta",";Jet Eta;N",100,-5,5)
      self.add("Jet4Phi",";Jet Phi;N",64,-3.2,3.2)
      self.add("Nbjets",";N b-jets (pT>30 GeV);N",10,0,10)
      self.add("BJet1Pt",";b-jet Pt;N",100,0,500)
      self.add("BJet1Eta",";b-jet Eta;N",100,-5,5)
      self.add("BJet1Phi",";b-jet Phi;N",64,-3.2,3.2)
      self.add("BJet2Pt",";b-jet Pt;N",100,0,500)
      self.add("BJet2Eta",";b-jet Eta;N",100,-5,5)
      self.add("BJet2Phi",";b-jet Phi;N",64,-3.2,3.2)
      self.add("BJet3Pt",";b-jet Pt;N",100,0,500)
      self.add("BJet3Eta",";b-jet Eta;N",100,-5,5)
      self.add("BJet3Phi",";b-jet Phi;N",64,-3.2,3.2)
      self.add("BJet4Pt",";b-jet Pt;N",100,0,500)
      self.add("BJet4Eta",";b-jet Eta;N",100,-5,5)
      self.add("BJet4Phi",";b-jet Phi;N",64,-3.2,3.2)
      self.add("MET",";MET;N",100,0,500)
      self.add("METphi",";MET phi;N",64,-3.2,3.2)

    def process(self, event):
      #get information
      result = { }
      result["Njets"]  = len([j for j in event.jets if j.PT>30])
      result["Jet1Pt"] = [ ]
      result["Jet1Eta"] = [ ]
      result["Jet1Phi"] = [ ]
      result["Jet2Pt"] = [ ]
      result["Jet2Eta"] = [ ]
      result["Jet2Phi"] = [ ]
      result["Jet3Pt"] = [ ]
      result["Jet3Eta"] = [ ]
      result["Jet3Phi"] = [ ]
      result["Jet4Pt"] = [ ]
      result["Jet4Eta"] = [ ]
      result["Jet4Phi"] = [ ]
      if event.jets.GetEntries()>0:
        result["Jet1Pt"].append(event.jets[0].PT)
        result["Jet1Eta"].append(event.jets[0].Eta)
        result["Jet1Phi"].append(event.jets[0].Phi)
      if event.jets.GetEntries()>1:
        result["Jet2Pt"].append(event.jets[1].PT)
        result["Jet2Eta"].append(event.jets[1].Eta)
        result["Jet2Phi"].append(event.jets[1].Phi)
      if event.jets.GetEntries()>2:
        result["Jet3Pt"].append(event.jets[2].PT)
        result["Jet3Eta"].append(event.jets[2].Eta)
        result["Jet3Phi"].append(event.jets[2].Phi)
      if event.jets.GetEntries()>3:
        result["Jet4Pt"].append(event.jets[3].PT)
        result["Jet4Eta"].append(event.jets[3].Eta)
        result["Jet4Phi"].append(event.jets[3].Phi)
      bjets=[]
      for jet in event.jets:
        if jet.BTag:
          bjets+=[jet]
      result["Nbjets"] = len([bj for bj in bjets if bj.PT>30])
      result["BJet1Pt"] = [ ]
      result["BJet1Eta"] = [ ]
      result["BJet1Phi"] = [ ]
      result["BJet2Pt"] = [ ]
      result["BJet2Eta"] = [ ]
      result["BJet2Phi"] = [ ]
      result["BJet3Pt"] = [ ]
      result["BJet3Eta"] = [ ]
      result["BJet3Phi"] = [ ]
      result["BJet4Pt"] = [ ]
      result["BJet4Eta"] = [ ]
      result["BJet4Phi"] = [ ]
      if len(bjets)>0:
        result["BJet1Pt"].append(bjets[0].PT)
        result["BJet1Eta"].append(bjets[0].Eta)
        result["BJet1Phi"].append(bjets[0].Phi)
      if len(bjets)>1:
        result["BJet2Pt"].append(bjets[1].PT)
        result["BJet2Eta"].append(bjets[1].Eta)
        result["BJet2Phi"].append(bjets[1].Phi)
      if len(bjets)>2:
        result["BJet3Pt"].append(bjets[2].PT)
        result["BJet3Eta"].append(bjets[2].Eta)
        result["BJet3Phi"].append(bjets[2].Phi)
      if len(bjets)>3:
        result["BJet4Pt"].append(bjets[3].PT)
        result["BJet4Eta"].append(bjets[3].Eta)
        result["BJet4Phi"].append(bjets[3].Phi)
      result["MET"] = event.met[0].MET
      result["METphi"] = event.met[0].Phi
      return result

if __name__=="__main__":
  import sys
  from DelphesAnalysis.BaseControlPlots import runTest
  runTest(sys.argv[1], JetControlPlots())

