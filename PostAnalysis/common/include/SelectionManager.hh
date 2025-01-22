#ifndef SELECTIONMANAGER__HH
#define SELECTIONMANAGER__HH

class SelectionManager
{
private:
  SelectionManager ();

private:
  static SelectionManager* fInstance=NULL;

public:
  static SelectionManager* GetIt ();


protected:
  class LeafManager
  {
    enum eLeafTypes {
      eShort_t  ,
      eUShort_t ,
      eInt_t    ,
      eUInt_t   ,
      eLong_t   ,
      eULong_t  ,
      eFloat_t  ,
      eDouble_t ,
      eBool_t   ,
      eChar_t   ,
      eByte_t
    };
  public:
    LeafManager (TLeaf*);

    auto GetValue const ();

  private:
    eLeafTypes* fLeafType;
    void* fLeafValueAddress;
  };


  map <std::string, LeafManager> fLeafManagerDictionary;


};

#endif
