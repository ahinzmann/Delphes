from BaseControlPlots import BaseControlPlots

# Requirements:
# event.Hs
# event.bHs
# event.MEt

class GenControlPlots(BaseControlPlots):
    """A class to create control plots for Hmet"""

    def __init__(self, dir=None, dataset=None, mode="plots"):
      # create output file if needed. If no file is given, it means it is delegated
      BaseControlPlots.__init__(self, dir=dir, purpose="Hmet", dataset=dataset, mode=mode)

    def beginJob(self):
      # declare histograms
      self.add("Nhiggs",";Higgs multiplicity;N",10,0,10)
      self.add("Nleptons",";Lepton multiplicity;N",10,0,10)
      self.add("Nbquarks",";b-quark multiplicity;N",10,0,10)
      self.add("Nlquarks",";l-quark multiplicity;N",10,0,10)
      self.add("HbbPt",";H Pt;N",100,0,1000)
      self.add("HbbEta",";H Eta;N",100,-5,5)
      self.add("HbbPhi",";H Phi;N",64,-3.2,3.2)
      self.add("HWWPt",";H Pt;N",100,0,1000)
      self.add("HWWEta",";H Eta;N",100,-5,5)
      self.add("HWWPhi",";H Phi;N",64,-3.2,3.2)
      self.add("b1Pt",";b_{1} Pt;N",100,0,500)
      self.add("b1Eta",";b_{1} Eta;N",100,-5,5)
      self.add("b1Phi",";b_{1} Phi;N",64,-3.2,3.2)
      self.add("b2Pt",";b_{2} Pt;N",100,0,500)
      self.add("b2Eta",";b_{2} Eta;N",100,-5,5)
      self.add("b2Phi",";b_{2} Phi;N",64,-3.2,3.2)

    def process(self, event):
      #get information
      result = { }
      nHiggs=0
      nLeptons=0
      nBquarks=0
      nLquarks=0
      result["HbbPt"] = []
      result["HbbEta"] = []
      result["HbbPhi"] = []
      result["HWWPt"] = []
      result["HWWEta"] = []
      result["HWWPhi"] = []
      result["b1Pt"] = []
      result["b1Eta"] = []
      result["b1Phi"] = []
      result["b2Pt"] = []
      result["b2Eta"] = []
      result["b2Phi"] = []
      for particle in event.particles:
          if abs(particle.PID) == 24 and particle.D1>=0 and particle.D1<len(event.particles) and event.particles[particle.D1]:
	     if abs(event.particles[particle.D1].PID) in [11,13,15]:
	       nLeptons+=1
	     if abs(event.particles[particle.D1].PID) in [1,2,3,4]:
	       nLquarks+=2
          if abs(particle.PID) == 25 and particle.D1>=0 and particle.D1<len(event.particles) and event.particles[particle.D1]:
	     if abs(event.particles[particle.D1].PID) in [5]:
	        nBquarks+=2
                result["HbbPt"].append( particle.PT )
	        result["HbbEta"].append( particle.Eta )
	        result["HbbPhi"].append( particle.Phi )
		if abs(event.particles[particle.D1].Eta)<abs(event.particles[particle.D2].Eta):
		    d1=particle.D1
		    d2=particle.D2
		else:
		    d1=particle.D2
		    d2=particle.D1
                result["b1Pt"].append( event.particles[d1].PT )
	        result["b1Eta"].append( event.particles[d1].Eta )
	        result["b1Phi"].append( event.particles[d1].Phi )
                result["b2Pt"].append( event.particles[d2].PT )
	        result["b2Eta"].append( event.particles[d2].Eta )
	        result["b2Phi"].append( event.particles[d2].Phi )
	     if abs(event.particles[particle.D1].PID) in [24]:
	        result["HWWPt"].append( particle.PT )
	        result["HWWEta"].append( particle.Eta )
	        result["HWWPhi"].append( particle.Phi )
	     nHiggs+=1
      result["Nhiggs"]  = nHiggs
      result["Nleptons"]  = nLeptons
      result["Nbquarks"]  = nBquarks
      result["Nlquarks"]  = nLquarks
      return result

if __name__=="__main__":
  import sys
  from DelphesAnalysis.BaseControlPlots import runTest
  runTest(sys.argv[1], GenControlPlots())

