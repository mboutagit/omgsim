//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep 28 11:13:28 2015 by ROOT version 5.34/19
// from TTree Hit/Hits in a TTree
// found on file: Water.root
//////////////////////////////////////////////////////////

#ifndef K40TreeReader_h
#define K40TreeReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class K40TreeReader {
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Float_t         Vertex_VertexX;
  Float_t         Vertex_VertexY;
  Float_t         Vertex_VertexZ;
  Float_t         Vertex_VertexT;
  Float_t         Vertex_VertexP;
  Float_t         Vertex_VertexDirX;
  Float_t         Vertex_VertexDirY;
  Float_t         Vertex_VertexDirZ;
  Float_t         Vertex_VertexEnergy;
  Float_t         Vertex_OMHitX;
  Float_t         Vertex_OMHitY;
  Float_t         Vertex_OMHitZ;
  Float_t         Vertex_OMHitT;
  Float_t         Vertex_OMHitP;
  Float_t         Vertex_PhotoX;
  Float_t         Vertex_PhotoY;
  Float_t         Vertex_PhotoZ;
  Float_t         Vertex_PhotoT;
  Float_t         Vertex_PhotoP;
  Short_t         Vertex_OMID;
  Short_t         Vertex_PMID;
  Float_t         Hit_TotalTime;
  Float_t         Hit_Energy;
  UChar_t         Hit_Limit;
  ULong64_t       HitNumber;

  // List of branches
  TBranch        *b_Vertex;   //!
  TBranch        *b_Hit;   //!
  TBranch        *b_EventNb;   //!

  std::string    fFileName;
  std::string    fOutputName = "default.root";

  K40TreeReader();
  virtual ~K40TreeReader();
  void SetSourceFileName (std::string);
  void SetOutPutFileName (std::string S) {fOutputName=S;}
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual unsigned Loop() = 0;
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef K40TreeReader_cxx
K40TreeReader::K40TreeReader() : fChain(0)
{
}

void K40TreeReader::SetSourceFileName (std::string filename)
{
  fFileName=filename;
  TTree* tree=0;
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename.data ());
  if (!f || !f->IsOpen()) {
    f = new TFile(filename.data ());
  }
  f->GetObject("Hit",tree);
  Init(tree);
}

K40TreeReader::~K40TreeReader()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t K40TreeReader::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t K40TreeReader::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void K40TreeReader::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("Vertex", &Vertex_VertexX, &b_Vertex);
  fChain->SetBranchAddress("Hit", &Hit_TotalTime, &b_Hit);
  fChain->SetBranchAddress("HitNumber", &HitNumber, &b_EventNb);
  Notify();
}

Bool_t K40TreeReader::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void K40TreeReader::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t K40TreeReader::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef K40TreeReader_cxx
