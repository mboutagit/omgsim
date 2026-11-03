#include <SelectionManager.hh>

#include <TLeaf.h>
#include <string>

using namespace std;

SelectionManager::SelectionManager ()
{
  if (!fSelectionManager)
    return fSelectionManager=SelectionManager ();
  return fSelectionManager;
}

SelectionManager::LeafManager::LeafManager (TLeaf* leaf_)
{
  fLeafValueAddress=leaf_->GetValuePointer ();

  string leaf_typename=leaf_->GetTypeName ();

  if (leaf_typename == "Short_t"  ) fLeafType=eShort_t;
  if (leaf_typename == "UShort_t" ) fLeafType=eUShort_t;
  if (leaf_typename == "Int_t"    ) fLeafType=eInt_t;
  if (leaf_typename == "UInt_t"   ) fLeafType=eUInt_t;
  if (leaf_typename == "Long_t"   ) fLeafType=eLong_t;
  if (leaf_typename == "ULong_t"  ) fLeafType=eULong_t;
  if (leaf_typename == "Float_t"  ) fLeafType=eFloat_t;
  if (leaf_typename == "Double_t" ) fLeafType=eDouble_t;
  if (leaf_typename == "Bool_t"   ) fLeafType=eBool_t;
  if (leaf_typename == "Char_t"   ) fLeafType=eChar_t;
  if (leaf_typename == "Byte_t"   ) fLeafType=eByte_t;

}

auto SelectionManager::LeafManager::GetValue ()
{
  if (fLeafType == eShort_t  ) return (Short_t)  (*fLeafValueAddress);
  if (fLeafType == eUShort_t ) return (UShort_t) (*fLeafValueAddress);
  if (fLeafType == eInt_t    ) return (Int_t)    (*fLeafValueAddress);
  if (fLeafType == eUInt_t   ) return (UInt_t)   (*fLeafValueAddress);
  if (fLeafType == eLong_t   ) return (Long_t)   (*fLeafValueAddress);
  if (fLeafType == eULong_t  ) return (ULong_t)  (*fLeafValueAddress);
  if (fLeafType == eFloat_t  ) return (Float_t)  (*fLeafValueAddress);
  if (fLeafType == eDouble_t ) return (Double_t) (*fLeafValueAddress);
  if (fLeafType == eBool_t   ) return (Bool_t)   (*fLeafValueAddress);
  if (fLeafType == eChar_t   ) return (Char_t)   (*fLeafValueAddress);
  if (fLeafType == eByte_t   ) return (Byte_t)   (*fLeafValueAddress);
}











