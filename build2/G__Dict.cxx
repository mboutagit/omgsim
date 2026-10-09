// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetector.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorConstruction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigi.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigitizerModule.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEvent.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEventAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHardwareManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHeader.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHit.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BLactelDetector.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BStackingAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BSteppingAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BVolumesManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/Utils.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcher.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherConf.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherInterface.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/DAQManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/Waveform.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace lactel {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *lactel_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("lactel", 0 /*version*/, "KM3DetectorConstruction.hh", 47,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &lactel_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *lactel_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *CLHEPcLcLHep3Vector_Dictionary();
   static void CLHEPcLcLHep3Vector_TClassManip(TClass*);
   static void *new_CLHEPcLcLHep3Vector(void *p = nullptr);
   static void *newArray_CLHEPcLcLHep3Vector(Long_t size, void *p);
   static void delete_CLHEPcLcLHep3Vector(void *p);
   static void deleteArray_CLHEPcLcLHep3Vector(void *p);
   static void destruct_CLHEPcLcLHep3Vector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CLHEP::Hep3Vector*)
   {
      ::CLHEP::Hep3Vector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CLHEP::Hep3Vector));
      static ::ROOT::TGenericClassInfo 
         instance("CLHEP::Hep3Vector", "CLHEP/Vector/ThreeVector.h", 41,
                  typeid(::CLHEP::Hep3Vector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CLHEPcLcLHep3Vector_Dictionary, isa_proxy, 4,
                  sizeof(::CLHEP::Hep3Vector) );
      instance.SetNew(&new_CLHEPcLcLHep3Vector);
      instance.SetNewArray(&newArray_CLHEPcLcLHep3Vector);
      instance.SetDelete(&delete_CLHEPcLcLHep3Vector);
      instance.SetDeleteArray(&deleteArray_CLHEPcLcLHep3Vector);
      instance.SetDestructor(&destruct_CLHEPcLcLHep3Vector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CLHEP::Hep3Vector*)
   {
      return GenerateInitInstanceLocal(static_cast<::CLHEP::Hep3Vector*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::CLHEP::Hep3Vector*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CLHEPcLcLHep3Vector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::CLHEP::Hep3Vector*>(nullptr))->GetClass();
      CLHEPcLcLHep3Vector_TClassManip(theClass);
   return theClass;
   }

   static void CLHEPcLcLHep3Vector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_Waveform(void *p = nullptr);
   static void *newArray_Waveform(Long_t size, void *p);
   static void delete_Waveform(void *p);
   static void deleteArray_Waveform(void *p);
   static void destruct_Waveform(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Waveform*)
   {
      ::Waveform *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Waveform >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Waveform", ::Waveform::Class_Version(), "tools/include/Waveform.hh", 30,
                  typeid(::Waveform), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Waveform::Dictionary, isa_proxy, 4,
                  sizeof(::Waveform) );
      instance.SetNew(&new_Waveform);
      instance.SetNewArray(&newArray_Waveform);
      instance.SetDelete(&delete_Waveform);
      instance.SetDeleteArray(&deleteArray_Waveform);
      instance.SetDestructor(&destruct_Waveform);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Waveform*)
   {
      return GenerateInitInstanceLocal(static_cast<::Waveform*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Waveform*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GeometryObject(void *p = nullptr);
   static void *newArray_GeometryObject(Long_t size, void *p);
   static void delete_GeometryObject(void *p);
   static void deleteArray_GeometryObject(void *p);
   static void destruct_GeometryObject(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GeometryObject*)
   {
      ::GeometryObject *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GeometryObject >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("GeometryObject", ::GeometryObject::Class_Version(), "GeometryObject.hh", 9,
                  typeid(::GeometryObject), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::GeometryObject::Dictionary, isa_proxy, 4,
                  sizeof(::GeometryObject) );
      instance.SetNew(&new_GeometryObject);
      instance.SetNewArray(&newArray_GeometryObject);
      instance.SetDelete(&delete_GeometryObject);
      instance.SetDeleteArray(&deleteArray_GeometryObject);
      instance.SetDestructor(&destruct_GeometryObject);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GeometryObject*)
   {
      return GenerateInitInstanceLocal(static_cast<::GeometryObject*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::GeometryObject*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *GeometryGridlElactelcLcLElementaryCellgR_Dictionary();
   static void GeometryGridlElactelcLcLElementaryCellgR_TClassManip(TClass*);
   static void *new_GeometryGridlElactelcLcLElementaryCellgR(void *p = nullptr);
   static void *newArray_GeometryGridlElactelcLcLElementaryCellgR(Long_t size, void *p);
   static void delete_GeometryGridlElactelcLcLElementaryCellgR(void *p);
   static void deleteArray_GeometryGridlElactelcLcLElementaryCellgR(void *p);
   static void destruct_GeometryGridlElactelcLcLElementaryCellgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GeometryGrid<lactel::ElementaryCell>*)
   {
      ::GeometryGrid<lactel::ElementaryCell> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GeometryGrid<lactel::ElementaryCell> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("GeometryGrid<lactel::ElementaryCell>", ::GeometryGrid<lactel::ElementaryCell>::Class_Version(), "GeometryGrid.hh", 13,
                  typeid(::GeometryGrid<lactel::ElementaryCell>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &GeometryGridlElactelcLcLElementaryCellgR_Dictionary, isa_proxy, 4,
                  sizeof(::GeometryGrid<lactel::ElementaryCell>) );
      instance.SetNew(&new_GeometryGridlElactelcLcLElementaryCellgR);
      instance.SetNewArray(&newArray_GeometryGridlElactelcLcLElementaryCellgR);
      instance.SetDelete(&delete_GeometryGridlElactelcLcLElementaryCellgR);
      instance.SetDeleteArray(&deleteArray_GeometryGridlElactelcLcLElementaryCellgR);
      instance.SetDestructor(&destruct_GeometryGridlElactelcLcLElementaryCellgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GeometryGrid<lactel::ElementaryCell>*)
   {
      return GenerateInitInstanceLocal(static_cast<::GeometryGrid<lactel::ElementaryCell>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::GeometryGrid<lactel::ElementaryCell>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GeometryGridlElactelcLcLElementaryCellgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::GeometryGrid<lactel::ElementaryCell>*>(nullptr))->GetClass();
      GeometryGridlElactelcLcLElementaryCellgR_TClassManip(theClass);
   return theClass;
   }

   static void GeometryGridlElactelcLcLElementaryCellgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *GeometryGridlElactelcLcLModulegR_Dictionary();
   static void GeometryGridlElactelcLcLModulegR_TClassManip(TClass*);
   static void *new_GeometryGridlElactelcLcLModulegR(void *p = nullptr);
   static void *newArray_GeometryGridlElactelcLcLModulegR(Long_t size, void *p);
   static void delete_GeometryGridlElactelcLcLModulegR(void *p);
   static void deleteArray_GeometryGridlElactelcLcLModulegR(void *p);
   static void destruct_GeometryGridlElactelcLcLModulegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GeometryGrid<lactel::Module>*)
   {
      ::GeometryGrid<lactel::Module> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GeometryGrid<lactel::Module> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("GeometryGrid<lactel::Module>", ::GeometryGrid<lactel::Module>::Class_Version(), "GeometryGrid.hh", 13,
                  typeid(::GeometryGrid<lactel::Module>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &GeometryGridlElactelcLcLModulegR_Dictionary, isa_proxy, 4,
                  sizeof(::GeometryGrid<lactel::Module>) );
      instance.SetNew(&new_GeometryGridlElactelcLcLModulegR);
      instance.SetNewArray(&newArray_GeometryGridlElactelcLcLModulegR);
      instance.SetDelete(&delete_GeometryGridlElactelcLcLModulegR);
      instance.SetDeleteArray(&deleteArray_GeometryGridlElactelcLcLModulegR);
      instance.SetDestructor(&destruct_GeometryGridlElactelcLcLModulegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GeometryGrid<lactel::Module>*)
   {
      return GenerateInitInstanceLocal(static_cast<::GeometryGrid<lactel::Module>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::GeometryGrid<lactel::Module>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GeometryGridlElactelcLcLModulegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::GeometryGrid<lactel::Module>*>(nullptr))->GetClass();
      GeometryGridlElactelcLcLModulegR_TClassManip(theClass);
   return theClass;
   }

   static void GeometryGridlElactelcLcLModulegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *GeometryGridlElactelcLcLBLactelDetectorgR_Dictionary();
   static void GeometryGridlElactelcLcLBLactelDetectorgR_TClassManip(TClass*);
   static void *new_GeometryGridlElactelcLcLBLactelDetectorgR(void *p = nullptr);
   static void *newArray_GeometryGridlElactelcLcLBLactelDetectorgR(Long_t size, void *p);
   static void delete_GeometryGridlElactelcLcLBLactelDetectorgR(void *p);
   static void deleteArray_GeometryGridlElactelcLcLBLactelDetectorgR(void *p);
   static void destruct_GeometryGridlElactelcLcLBLactelDetectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GeometryGrid<lactel::BLactelDetector>*)
   {
      ::GeometryGrid<lactel::BLactelDetector> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GeometryGrid<lactel::BLactelDetector> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("GeometryGrid<lactel::BLactelDetector>", ::GeometryGrid<lactel::BLactelDetector>::Class_Version(), "GeometryGrid.hh", 13,
                  typeid(::GeometryGrid<lactel::BLactelDetector>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &GeometryGridlElactelcLcLBLactelDetectorgR_Dictionary, isa_proxy, 4,
                  sizeof(::GeometryGrid<lactel::BLactelDetector>) );
      instance.SetNew(&new_GeometryGridlElactelcLcLBLactelDetectorgR);
      instance.SetNewArray(&newArray_GeometryGridlElactelcLcLBLactelDetectorgR);
      instance.SetDelete(&delete_GeometryGridlElactelcLcLBLactelDetectorgR);
      instance.SetDeleteArray(&deleteArray_GeometryGridlElactelcLcLBLactelDetectorgR);
      instance.SetDestructor(&destruct_GeometryGridlElactelcLcLBLactelDetectorgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GeometryGrid<lactel::BLactelDetector>*)
   {
      return GenerateInitInstanceLocal(static_cast<::GeometryGrid<lactel::BLactelDetector>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::GeometryGrid<lactel::BLactelDetector>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GeometryGridlElactelcLcLBLactelDetectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::GeometryGrid<lactel::BLactelDetector>*>(nullptr))->GetClass();
      GeometryGridlElactelcLcLBLactelDetectorgR_TClassManip(theClass);
   return theClass;
   }

   static void GeometryGridlElactelcLcLBLactelDetectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_BDetector(void *p);
   static void deleteArray_BDetector(void *p);
   static void destruct_BDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BDetector*)
   {
      ::BDetector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BDetector >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("BDetector", ::BDetector::Class_Version(), "BDetector.hh", 11,
                  typeid(::BDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BDetector::Dictionary, isa_proxy, 4,
                  sizeof(::BDetector) );
      instance.SetDelete(&delete_BDetector);
      instance.SetDeleteArray(&deleteArray_BDetector);
      instance.SetDestructor(&destruct_BDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BDetector*)
   {
      return GenerateInitInstanceLocal(static_cast<::BDetector*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::BDetector*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_lactelcLcLElementaryCell(void *p = nullptr);
   static void *newArray_lactelcLcLElementaryCell(Long_t size, void *p);
   static void delete_lactelcLcLElementaryCell(void *p);
   static void deleteArray_lactelcLcLElementaryCell(void *p);
   static void destruct_lactelcLcLElementaryCell(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::ElementaryCell*)
   {
      ::lactel::ElementaryCell *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::ElementaryCell >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::ElementaryCell", ::lactel::ElementaryCell::Class_Version(), "BLactelDetector.hh", 43,
                  typeid(::lactel::ElementaryCell), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::lactel::ElementaryCell::Dictionary, isa_proxy, 4,
                  sizeof(::lactel::ElementaryCell) );
      instance.SetNew(&new_lactelcLcLElementaryCell);
      instance.SetNewArray(&newArray_lactelcLcLElementaryCell);
      instance.SetDelete(&delete_lactelcLcLElementaryCell);
      instance.SetDeleteArray(&deleteArray_lactelcLcLElementaryCell);
      instance.SetDestructor(&destruct_lactelcLcLElementaryCell);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::ElementaryCell*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::ElementaryCell*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::ElementaryCell*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_lactelcLcLModule(void *p = nullptr);
   static void *newArray_lactelcLcLModule(Long_t size, void *p);
   static void delete_lactelcLcLModule(void *p);
   static void deleteArray_lactelcLcLModule(void *p);
   static void destruct_lactelcLcLModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::Module*)
   {
      ::lactel::Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::Module", ::lactel::Module::Class_Version(), "BLactelDetector.hh", 92,
                  typeid(::lactel::Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::lactel::Module::Dictionary, isa_proxy, 4,
                  sizeof(::lactel::Module) );
      instance.SetNew(&new_lactelcLcLModule);
      instance.SetNewArray(&newArray_lactelcLcLModule);
      instance.SetDelete(&delete_lactelcLcLModule);
      instance.SetDeleteArray(&deleteArray_lactelcLcLModule);
      instance.SetDestructor(&destruct_lactelcLcLModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_lactelcLcLBLactelDetector(void *p = nullptr);
   static void *newArray_lactelcLcLBLactelDetector(Long_t size, void *p);
   static void delete_lactelcLcLBLactelDetector(void *p);
   static void deleteArray_lactelcLcLBLactelDetector(void *p);
   static void destruct_lactelcLcLBLactelDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BLactelDetector*)
   {
      ::lactel::BLactelDetector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::BLactelDetector >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BLactelDetector", ::lactel::BLactelDetector::Class_Version(), "BLactelDetector.hh", 145,
                  typeid(::lactel::BLactelDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::lactel::BLactelDetector::Dictionary, isa_proxy, 4,
                  sizeof(::lactel::BLactelDetector) );
      instance.SetNew(&new_lactelcLcLBLactelDetector);
      instance.SetNewArray(&newArray_lactelcLcLBLactelDetector);
      instance.SetDelete(&delete_lactelcLcLBLactelDetector);
      instance.SetDeleteArray(&deleteArray_lactelcLcLBLactelDetector);
      instance.SetDestructor(&destruct_lactelcLcLBLactelDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::BLactelDetector*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::BLactelDetector*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::BLactelDetector*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BAnalysisManager*)
   {
      ::lactel::BAnalysisManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::BAnalysisManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BAnalysisManager", ::lactel::BAnalysisManager::Class_Version(), "BAnalysisManager.hh", 25,
                  typeid(::lactel::BAnalysisManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::lactel::BAnalysisManager::Dictionary, isa_proxy, 4,
                  sizeof(::lactel::BAnalysisManager) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::BAnalysisManager*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::BAnalysisManager*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p = nullptr);
   static void *newArray_lactelcLcLBAnalysisManagercLcLMeasureParameters(Long_t size, void *p);
   static void delete_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p);
   static void deleteArray_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p);
   static void destruct_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BAnalysisManager::MeasureParameters*)
   {
      ::lactel::BAnalysisManager::MeasureParameters *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::BAnalysisManager::MeasureParameters >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BAnalysisManager::MeasureParameters", ::lactel::BAnalysisManager::MeasureParameters::Class_Version(), "BAnalysisManager.hh", 57,
                  typeid(::lactel::BAnalysisManager::MeasureParameters), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::lactel::BAnalysisManager::MeasureParameters::Dictionary, isa_proxy, 4,
                  sizeof(::lactel::BAnalysisManager::MeasureParameters) );
      instance.SetNew(&new_lactelcLcLBAnalysisManagercLcLMeasureParameters);
      instance.SetNewArray(&newArray_lactelcLcLBAnalysisManagercLcLMeasureParameters);
      instance.SetDelete(&delete_lactelcLcLBAnalysisManagercLcLMeasureParameters);
      instance.SetDeleteArray(&deleteArray_lactelcLcLBAnalysisManagercLcLMeasureParameters);
      instance.SetDestructor(&destruct_lactelcLcLBAnalysisManagercLcLMeasureParameters);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::BAnalysisManager::MeasureParameters*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::BAnalysisManager::MeasureParameters*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager::MeasureParameters*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_lactelcLcLBAnalysisManagercLcLMeasure(void *p = nullptr);
   static void *newArray_lactelcLcLBAnalysisManagercLcLMeasure(Long_t size, void *p);
   static void delete_lactelcLcLBAnalysisManagercLcLMeasure(void *p);
   static void deleteArray_lactelcLcLBAnalysisManagercLcLMeasure(void *p);
   static void destruct_lactelcLcLBAnalysisManagercLcLMeasure(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BAnalysisManager::Measure*)
   {
      ::lactel::BAnalysisManager::Measure *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::BAnalysisManager::Measure >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BAnalysisManager::Measure", ::lactel::BAnalysisManager::Measure::Class_Version(), "BAnalysisManager.hh", 82,
                  typeid(::lactel::BAnalysisManager::Measure), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::lactel::BAnalysisManager::Measure::Dictionary, isa_proxy, 4,
                  sizeof(::lactel::BAnalysisManager::Measure) );
      instance.SetNew(&new_lactelcLcLBAnalysisManagercLcLMeasure);
      instance.SetNewArray(&newArray_lactelcLcLBAnalysisManagercLcLMeasure);
      instance.SetDelete(&delete_lactelcLcLBAnalysisManagercLcLMeasure);
      instance.SetDeleteArray(&deleteArray_lactelcLcLBAnalysisManagercLcLMeasure);
      instance.SetDestructor(&destruct_lactelcLcLBAnalysisManagercLcLMeasure);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::BAnalysisManager::Measure*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::BAnalysisManager::Measure*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager::Measure*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *lactelcLcLBAnalysisManagercLcLDAQEvent_Dictionary();
   static void lactelcLcLBAnalysisManagercLcLDAQEvent_TClassManip(TClass*);
   static void *new_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p = nullptr);
   static void *newArray_lactelcLcLBAnalysisManagercLcLDAQEvent(Long_t size, void *p);
   static void delete_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p);
   static void deleteArray_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p);
   static void destruct_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BAnalysisManager::DAQEvent*)
   {
      ::lactel::BAnalysisManager::DAQEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::lactel::BAnalysisManager::DAQEvent));
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BAnalysisManager::DAQEvent", "BAnalysisManager.hh", 104,
                  typeid(::lactel::BAnalysisManager::DAQEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &lactelcLcLBAnalysisManagercLcLDAQEvent_Dictionary, isa_proxy, 4,
                  sizeof(::lactel::BAnalysisManager::DAQEvent) );
      instance.SetNew(&new_lactelcLcLBAnalysisManagercLcLDAQEvent);
      instance.SetNewArray(&newArray_lactelcLcLBAnalysisManagercLcLDAQEvent);
      instance.SetDelete(&delete_lactelcLcLBAnalysisManagercLcLDAQEvent);
      instance.SetDeleteArray(&deleteArray_lactelcLcLBAnalysisManagercLcLDAQEvent);
      instance.SetDestructor(&destruct_lactelcLcLBAnalysisManagercLcLDAQEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::BAnalysisManager::DAQEvent*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::BAnalysisManager::DAQEvent*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager::DAQEvent*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *lactelcLcLBAnalysisManagercLcLDAQEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager::DAQEvent*>(nullptr))->GetClass();
      lactelcLcLBAnalysisManagercLcLDAQEvent_TClassManip(theClass);
   return theClass;
   }

   static void lactelcLcLBAnalysisManagercLcLDAQEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *lactelcLcLBAnalysisManagercLcLHit_Dictionary();
   static void lactelcLcLBAnalysisManagercLcLHit_TClassManip(TClass*);
   static void *new_lactelcLcLBAnalysisManagercLcLHit(void *p = nullptr);
   static void *newArray_lactelcLcLBAnalysisManagercLcLHit(Long_t size, void *p);
   static void delete_lactelcLcLBAnalysisManagercLcLHit(void *p);
   static void deleteArray_lactelcLcLBAnalysisManagercLcLHit(void *p);
   static void destruct_lactelcLcLBAnalysisManagercLcLHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BAnalysisManager::Hit*)
   {
      ::lactel::BAnalysisManager::Hit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::lactel::BAnalysisManager::Hit));
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BAnalysisManager::Hit", "BAnalysisManager.hh", 196,
                  typeid(::lactel::BAnalysisManager::Hit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &lactelcLcLBAnalysisManagercLcLHit_Dictionary, isa_proxy, 4,
                  sizeof(::lactel::BAnalysisManager::Hit) );
      instance.SetNew(&new_lactelcLcLBAnalysisManagercLcLHit);
      instance.SetNewArray(&newArray_lactelcLcLBAnalysisManagercLcLHit);
      instance.SetDelete(&delete_lactelcLcLBAnalysisManagercLcLHit);
      instance.SetDeleteArray(&deleteArray_lactelcLcLBAnalysisManagercLcLHit);
      instance.SetDestructor(&destruct_lactelcLcLBAnalysisManagercLcLHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lactel::BAnalysisManager::Hit*)
   {
      return GenerateInitInstanceLocal(static_cast<::lactel::BAnalysisManager::Hit*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager::Hit*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *lactelcLcLBAnalysisManagercLcLHit_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::lactel::BAnalysisManager::Hit*>(nullptr))->GetClass();
      lactelcLcLBAnalysisManagercLcLHit_TClassManip(theClass);
   return theClass;
   }

   static void lactelcLcLBAnalysisManagercLcLHit_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Waveform::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Waveform::Class_Name()
{
   return "Waveform";
}

//______________________________________________________________________________
const char *Waveform::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Waveform*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Waveform::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Waveform*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Waveform::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Waveform*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Waveform::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Waveform*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GeometryObject::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *GeometryObject::Class_Name()
{
   return "GeometryObject";
}

//______________________________________________________________________________
const char *GeometryObject::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryObject*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int GeometryObject::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryObject*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GeometryObject::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryObject*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GeometryObject::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryObject*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr GeometryGrid<lactel::ElementaryCell>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *GeometryGrid<lactel::ElementaryCell>::Class_Name()
{
   return "GeometryGrid<lactel::ElementaryCell>";
}

//______________________________________________________________________________
template <> const char *GeometryGrid<lactel::ElementaryCell>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::ElementaryCell>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int GeometryGrid<lactel::ElementaryCell>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::ElementaryCell>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *GeometryGrid<lactel::ElementaryCell>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::ElementaryCell>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *GeometryGrid<lactel::ElementaryCell>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::ElementaryCell>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr GeometryGrid<lactel::Module>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *GeometryGrid<lactel::Module>::Class_Name()
{
   return "GeometryGrid<lactel::Module>";
}

//______________________________________________________________________________
template <> const char *GeometryGrid<lactel::Module>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::Module>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int GeometryGrid<lactel::Module>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::Module>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *GeometryGrid<lactel::Module>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::Module>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *GeometryGrid<lactel::Module>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::Module>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr GeometryGrid<lactel::BLactelDetector>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *GeometryGrid<lactel::BLactelDetector>::Class_Name()
{
   return "GeometryGrid<lactel::BLactelDetector>";
}

//______________________________________________________________________________
template <> const char *GeometryGrid<lactel::BLactelDetector>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::BLactelDetector>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int GeometryGrid<lactel::BLactelDetector>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::BLactelDetector>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *GeometryGrid<lactel::BLactelDetector>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::BLactelDetector>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *GeometryGrid<lactel::BLactelDetector>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GeometryGrid<lactel::BLactelDetector>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr BDetector::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BDetector::Class_Name()
{
   return "BDetector";
}

//______________________________________________________________________________
const char *BDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BDetector*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BDetector*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BDetector*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BDetector*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace lactel {
//______________________________________________________________________________
atomic_TClass_ptr ElementaryCell::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ElementaryCell::Class_Name()
{
   return "lactel::ElementaryCell";
}

//______________________________________________________________________________
const char *ElementaryCell::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::ElementaryCell*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ElementaryCell::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::ElementaryCell*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ElementaryCell::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::ElementaryCell*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ElementaryCell::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::ElementaryCell*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace lactel
namespace lactel {
//______________________________________________________________________________
atomic_TClass_ptr Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Module::Class_Name()
{
   return "lactel::Module";
}

//______________________________________________________________________________
const char *Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace lactel
namespace lactel {
//______________________________________________________________________________
atomic_TClass_ptr BLactelDetector::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BLactelDetector::Class_Name()
{
   return "lactel::BLactelDetector";
}

//______________________________________________________________________________
const char *BLactelDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BLactelDetector*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BLactelDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BLactelDetector*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BLactelDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BLactelDetector*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BLactelDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BLactelDetector*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace lactel
namespace lactel {
//______________________________________________________________________________
atomic_TClass_ptr BAnalysisManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BAnalysisManager::Class_Name()
{
   return "lactel::BAnalysisManager";
}

//______________________________________________________________________________
const char *BAnalysisManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BAnalysisManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BAnalysisManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BAnalysisManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace lactel
namespace lactel {
//______________________________________________________________________________
atomic_TClass_ptr BAnalysisManager::MeasureParameters::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BAnalysisManager::MeasureParameters::Class_Name()
{
   return "lactel::BAnalysisManager::MeasureParameters";
}

//______________________________________________________________________________
const char *BAnalysisManager::MeasureParameters::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::MeasureParameters*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BAnalysisManager::MeasureParameters::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::MeasureParameters*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BAnalysisManager::MeasureParameters::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::MeasureParameters*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BAnalysisManager::MeasureParameters::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::MeasureParameters*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace lactel
namespace lactel {
//______________________________________________________________________________
atomic_TClass_ptr BAnalysisManager::Measure::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BAnalysisManager::Measure::Class_Name()
{
   return "lactel::BAnalysisManager::Measure";
}

//______________________________________________________________________________
const char *BAnalysisManager::Measure::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::Measure*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BAnalysisManager::Measure::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::Measure*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BAnalysisManager::Measure::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::Measure*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BAnalysisManager::Measure::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::lactel::BAnalysisManager::Measure*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace lactel
namespace ROOT {
   // Wrappers around operator new
   static void *new_CLHEPcLcLHep3Vector(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::CLHEP::Hep3Vector : new ::CLHEP::Hep3Vector;
   }
   static void *newArray_CLHEPcLcLHep3Vector(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::CLHEP::Hep3Vector[nElements] : new ::CLHEP::Hep3Vector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CLHEPcLcLHep3Vector(void *p) {
      delete (static_cast<::CLHEP::Hep3Vector*>(p));
   }
   static void deleteArray_CLHEPcLcLHep3Vector(void *p) {
      delete [] (static_cast<::CLHEP::Hep3Vector*>(p));
   }
   static void destruct_CLHEPcLcLHep3Vector(void *p) {
      typedef ::CLHEP::Hep3Vector current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::CLHEP::Hep3Vector

//______________________________________________________________________________
void Waveform::Streamer(TBuffer &R__b)
{
   // Stream an object of class Waveform.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Waveform::Class(),this);
   } else {
      R__b.WriteClassBuffer(Waveform::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Waveform(void *p) {
      return  p ? new(p) ::Waveform : new ::Waveform;
   }
   static void *newArray_Waveform(Long_t nElements, void *p) {
      return p ? new(p) ::Waveform[nElements] : new ::Waveform[nElements];
   }
   // Wrapper around operator delete
   static void delete_Waveform(void *p) {
      delete (static_cast<::Waveform*>(p));
   }
   static void deleteArray_Waveform(void *p) {
      delete [] (static_cast<::Waveform*>(p));
   }
   static void destruct_Waveform(void *p) {
      typedef ::Waveform current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Waveform

//______________________________________________________________________________
void GeometryObject::Streamer(TBuffer &R__b)
{
   // Stream an object of class GeometryObject.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(GeometryObject::Class(),this);
   } else {
      R__b.WriteClassBuffer(GeometryObject::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GeometryObject(void *p) {
      return  p ? new(p) ::GeometryObject : new ::GeometryObject;
   }
   static void *newArray_GeometryObject(Long_t nElements, void *p) {
      return p ? new(p) ::GeometryObject[nElements] : new ::GeometryObject[nElements];
   }
   // Wrapper around operator delete
   static void delete_GeometryObject(void *p) {
      delete (static_cast<::GeometryObject*>(p));
   }
   static void deleteArray_GeometryObject(void *p) {
      delete [] (static_cast<::GeometryObject*>(p));
   }
   static void destruct_GeometryObject(void *p) {
      typedef ::GeometryObject current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::GeometryObject

//______________________________________________________________________________
template <> void GeometryGrid<lactel::ElementaryCell>::Streamer(TBuffer &R__b)
{
   // Stream an object of class GeometryGrid<lactel::ElementaryCell>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(GeometryGrid<lactel::ElementaryCell>::Class(),this);
   } else {
      R__b.WriteClassBuffer(GeometryGrid<lactel::ElementaryCell>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GeometryGridlElactelcLcLElementaryCellgR(void *p) {
      return  p ? new(p) ::GeometryGrid<lactel::ElementaryCell> : new ::GeometryGrid<lactel::ElementaryCell>;
   }
   static void *newArray_GeometryGridlElactelcLcLElementaryCellgR(Long_t nElements, void *p) {
      return p ? new(p) ::GeometryGrid<lactel::ElementaryCell>[nElements] : new ::GeometryGrid<lactel::ElementaryCell>[nElements];
   }
   // Wrapper around operator delete
   static void delete_GeometryGridlElactelcLcLElementaryCellgR(void *p) {
      delete (static_cast<::GeometryGrid<lactel::ElementaryCell>*>(p));
   }
   static void deleteArray_GeometryGridlElactelcLcLElementaryCellgR(void *p) {
      delete [] (static_cast<::GeometryGrid<lactel::ElementaryCell>*>(p));
   }
   static void destruct_GeometryGridlElactelcLcLElementaryCellgR(void *p) {
      typedef ::GeometryGrid<lactel::ElementaryCell> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::GeometryGrid<lactel::ElementaryCell>

//______________________________________________________________________________
template <> void GeometryGrid<lactel::Module>::Streamer(TBuffer &R__b)
{
   // Stream an object of class GeometryGrid<lactel::Module>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(GeometryGrid<lactel::Module>::Class(),this);
   } else {
      R__b.WriteClassBuffer(GeometryGrid<lactel::Module>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GeometryGridlElactelcLcLModulegR(void *p) {
      return  p ? new(p) ::GeometryGrid<lactel::Module> : new ::GeometryGrid<lactel::Module>;
   }
   static void *newArray_GeometryGridlElactelcLcLModulegR(Long_t nElements, void *p) {
      return p ? new(p) ::GeometryGrid<lactel::Module>[nElements] : new ::GeometryGrid<lactel::Module>[nElements];
   }
   // Wrapper around operator delete
   static void delete_GeometryGridlElactelcLcLModulegR(void *p) {
      delete (static_cast<::GeometryGrid<lactel::Module>*>(p));
   }
   static void deleteArray_GeometryGridlElactelcLcLModulegR(void *p) {
      delete [] (static_cast<::GeometryGrid<lactel::Module>*>(p));
   }
   static void destruct_GeometryGridlElactelcLcLModulegR(void *p) {
      typedef ::GeometryGrid<lactel::Module> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::GeometryGrid<lactel::Module>

//______________________________________________________________________________
template <> void GeometryGrid<lactel::BLactelDetector>::Streamer(TBuffer &R__b)
{
   // Stream an object of class GeometryGrid<lactel::BLactelDetector>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(GeometryGrid<lactel::BLactelDetector>::Class(),this);
   } else {
      R__b.WriteClassBuffer(GeometryGrid<lactel::BLactelDetector>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GeometryGridlElactelcLcLBLactelDetectorgR(void *p) {
      return  p ? new(p) ::GeometryGrid<lactel::BLactelDetector> : new ::GeometryGrid<lactel::BLactelDetector>;
   }
   static void *newArray_GeometryGridlElactelcLcLBLactelDetectorgR(Long_t nElements, void *p) {
      return p ? new(p) ::GeometryGrid<lactel::BLactelDetector>[nElements] : new ::GeometryGrid<lactel::BLactelDetector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_GeometryGridlElactelcLcLBLactelDetectorgR(void *p) {
      delete (static_cast<::GeometryGrid<lactel::BLactelDetector>*>(p));
   }
   static void deleteArray_GeometryGridlElactelcLcLBLactelDetectorgR(void *p) {
      delete [] (static_cast<::GeometryGrid<lactel::BLactelDetector>*>(p));
   }
   static void destruct_GeometryGridlElactelcLcLBLactelDetectorgR(void *p) {
      typedef ::GeometryGrid<lactel::BLactelDetector> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::GeometryGrid<lactel::BLactelDetector>

//______________________________________________________________________________
void BDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class BDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(BDetector::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_BDetector(void *p) {
      delete (static_cast<::BDetector*>(p));
   }
   static void deleteArray_BDetector(void *p) {
      delete [] (static_cast<::BDetector*>(p));
   }
   static void destruct_BDetector(void *p) {
      typedef ::BDetector current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::BDetector

namespace lactel {
//______________________________________________________________________________
void ElementaryCell::Streamer(TBuffer &R__b)
{
   // Stream an object of class lactel::ElementaryCell.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(lactel::ElementaryCell::Class(),this);
   } else {
      R__b.WriteClassBuffer(lactel::ElementaryCell::Class(),this);
   }
}

} // namespace lactel
namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLElementaryCell(void *p) {
      return  p ? new(p) ::lactel::ElementaryCell : new ::lactel::ElementaryCell;
   }
   static void *newArray_lactelcLcLElementaryCell(Long_t nElements, void *p) {
      return p ? new(p) ::lactel::ElementaryCell[nElements] : new ::lactel::ElementaryCell[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLElementaryCell(void *p) {
      delete (static_cast<::lactel::ElementaryCell*>(p));
   }
   static void deleteArray_lactelcLcLElementaryCell(void *p) {
      delete [] (static_cast<::lactel::ElementaryCell*>(p));
   }
   static void destruct_lactelcLcLElementaryCell(void *p) {
      typedef ::lactel::ElementaryCell current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::ElementaryCell

namespace lactel {
//______________________________________________________________________________
void Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class lactel::Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(lactel::Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(lactel::Module::Class(),this);
   }
}

} // namespace lactel
namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLModule(void *p) {
      return  p ? new(p) ::lactel::Module : new ::lactel::Module;
   }
   static void *newArray_lactelcLcLModule(Long_t nElements, void *p) {
      return p ? new(p) ::lactel::Module[nElements] : new ::lactel::Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLModule(void *p) {
      delete (static_cast<::lactel::Module*>(p));
   }
   static void deleteArray_lactelcLcLModule(void *p) {
      delete [] (static_cast<::lactel::Module*>(p));
   }
   static void destruct_lactelcLcLModule(void *p) {
      typedef ::lactel::Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::Module

namespace lactel {
//______________________________________________________________________________
void BLactelDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class lactel::BLactelDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(lactel::BLactelDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(lactel::BLactelDetector::Class(),this);
   }
}

} // namespace lactel
namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLBLactelDetector(void *p) {
      return  p ? new(p) ::lactel::BLactelDetector : new ::lactel::BLactelDetector;
   }
   static void *newArray_lactelcLcLBLactelDetector(Long_t nElements, void *p) {
      return p ? new(p) ::lactel::BLactelDetector[nElements] : new ::lactel::BLactelDetector[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLBLactelDetector(void *p) {
      delete (static_cast<::lactel::BLactelDetector*>(p));
   }
   static void deleteArray_lactelcLcLBLactelDetector(void *p) {
      delete [] (static_cast<::lactel::BLactelDetector*>(p));
   }
   static void destruct_lactelcLcLBLactelDetector(void *p) {
      typedef ::lactel::BLactelDetector current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::BLactelDetector

namespace lactel {
//______________________________________________________________________________
void BAnalysisManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class lactel::BAnalysisManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(lactel::BAnalysisManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(lactel::BAnalysisManager::Class(),this);
   }
}

} // namespace lactel
namespace ROOT {
} // end of namespace ROOT for class ::lactel::BAnalysisManager

namespace lactel {
//______________________________________________________________________________
void BAnalysisManager::MeasureParameters::Streamer(TBuffer &R__b)
{
   // Stream an object of class lactel::BAnalysisManager::MeasureParameters.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(lactel::BAnalysisManager::MeasureParameters::Class(),this);
   } else {
      R__b.WriteClassBuffer(lactel::BAnalysisManager::MeasureParameters::Class(),this);
   }
}

} // namespace lactel
namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::MeasureParameters : new ::lactel::BAnalysisManager::MeasureParameters;
   }
   static void *newArray_lactelcLcLBAnalysisManagercLcLMeasureParameters(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::MeasureParameters[nElements] : new ::lactel::BAnalysisManager::MeasureParameters[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p) {
      delete (static_cast<::lactel::BAnalysisManager::MeasureParameters*>(p));
   }
   static void deleteArray_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p) {
      delete [] (static_cast<::lactel::BAnalysisManager::MeasureParameters*>(p));
   }
   static void destruct_lactelcLcLBAnalysisManagercLcLMeasureParameters(void *p) {
      typedef ::lactel::BAnalysisManager::MeasureParameters current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::BAnalysisManager::MeasureParameters

namespace lactel {
//______________________________________________________________________________
void BAnalysisManager::Measure::Streamer(TBuffer &R__b)
{
   // Stream an object of class lactel::BAnalysisManager::Measure.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(lactel::BAnalysisManager::Measure::Class(),this);
   } else {
      R__b.WriteClassBuffer(lactel::BAnalysisManager::Measure::Class(),this);
   }
}

} // namespace lactel
namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLBAnalysisManagercLcLMeasure(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::Measure : new ::lactel::BAnalysisManager::Measure;
   }
   static void *newArray_lactelcLcLBAnalysisManagercLcLMeasure(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::Measure[nElements] : new ::lactel::BAnalysisManager::Measure[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLBAnalysisManagercLcLMeasure(void *p) {
      delete (static_cast<::lactel::BAnalysisManager::Measure*>(p));
   }
   static void deleteArray_lactelcLcLBAnalysisManagercLcLMeasure(void *p) {
      delete [] (static_cast<::lactel::BAnalysisManager::Measure*>(p));
   }
   static void destruct_lactelcLcLBAnalysisManagercLcLMeasure(void *p) {
      typedef ::lactel::BAnalysisManager::Measure current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::BAnalysisManager::Measure

namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::DAQEvent : new ::lactel::BAnalysisManager::DAQEvent;
   }
   static void *newArray_lactelcLcLBAnalysisManagercLcLDAQEvent(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::DAQEvent[nElements] : new ::lactel::BAnalysisManager::DAQEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p) {
      delete (static_cast<::lactel::BAnalysisManager::DAQEvent*>(p));
   }
   static void deleteArray_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p) {
      delete [] (static_cast<::lactel::BAnalysisManager::DAQEvent*>(p));
   }
   static void destruct_lactelcLcLBAnalysisManagercLcLDAQEvent(void *p) {
      typedef ::lactel::BAnalysisManager::DAQEvent current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::BAnalysisManager::DAQEvent

namespace ROOT {
   // Wrappers around operator new
   static void *new_lactelcLcLBAnalysisManagercLcLHit(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::Hit : new ::lactel::BAnalysisManager::Hit;
   }
   static void *newArray_lactelcLcLBAnalysisManagercLcLHit(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::lactel::BAnalysisManager::Hit[nElements] : new ::lactel::BAnalysisManager::Hit[nElements];
   }
   // Wrapper around operator delete
   static void delete_lactelcLcLBAnalysisManagercLcLHit(void *p) {
      delete (static_cast<::lactel::BAnalysisManager::Hit*>(p));
   }
   static void deleteArray_lactelcLcLBAnalysisManagercLcLHit(void *p) {
      delete [] (static_cast<::lactel::BAnalysisManager::Hit*>(p));
   }
   static void destruct_lactelcLcLBAnalysisManagercLcLHit(void *p) {
      typedef ::lactel::BAnalysisManager::Hit current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::lactel::BAnalysisManager::Hit

namespace ROOT {
   static TClass *vectorlElactelcLcLModulegR_Dictionary();
   static void vectorlElactelcLcLModulegR_TClassManip(TClass*);
   static void *new_vectorlElactelcLcLModulegR(void *p = nullptr);
   static void *newArray_vectorlElactelcLcLModulegR(Long_t size, void *p);
   static void delete_vectorlElactelcLcLModulegR(void *p);
   static void deleteArray_vectorlElactelcLcLModulegR(void *p);
   static void destruct_vectorlElactelcLcLModulegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<lactel::Module>*)
   {
      vector<lactel::Module> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<lactel::Module>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<lactel::Module>", -2, "vector", 428,
                  typeid(vector<lactel::Module>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlElactelcLcLModulegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<lactel::Module>) );
      instance.SetNew(&new_vectorlElactelcLcLModulegR);
      instance.SetNewArray(&newArray_vectorlElactelcLcLModulegR);
      instance.SetDelete(&delete_vectorlElactelcLcLModulegR);
      instance.SetDeleteArray(&deleteArray_vectorlElactelcLcLModulegR);
      instance.SetDestructor(&destruct_vectorlElactelcLcLModulegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<lactel::Module> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<lactel::Module>","std::vector<lactel::Module, std::allocator<lactel::Module> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<lactel::Module>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElactelcLcLModulegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<lactel::Module>*>(nullptr))->GetClass();
      vectorlElactelcLcLModulegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElactelcLcLModulegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElactelcLcLModulegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::Module> : new vector<lactel::Module>;
   }
   static void *newArray_vectorlElactelcLcLModulegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::Module>[nElements] : new vector<lactel::Module>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElactelcLcLModulegR(void *p) {
      delete (static_cast<vector<lactel::Module>*>(p));
   }
   static void deleteArray_vectorlElactelcLcLModulegR(void *p) {
      delete [] (static_cast<vector<lactel::Module>*>(p));
   }
   static void destruct_vectorlElactelcLcLModulegR(void *p) {
      typedef vector<lactel::Module> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<lactel::Module>

namespace ROOT {
   static TClass *vectorlElactelcLcLElementaryCellgR_Dictionary();
   static void vectorlElactelcLcLElementaryCellgR_TClassManip(TClass*);
   static void *new_vectorlElactelcLcLElementaryCellgR(void *p = nullptr);
   static void *newArray_vectorlElactelcLcLElementaryCellgR(Long_t size, void *p);
   static void delete_vectorlElactelcLcLElementaryCellgR(void *p);
   static void deleteArray_vectorlElactelcLcLElementaryCellgR(void *p);
   static void destruct_vectorlElactelcLcLElementaryCellgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<lactel::ElementaryCell>*)
   {
      vector<lactel::ElementaryCell> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<lactel::ElementaryCell>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<lactel::ElementaryCell>", -2, "vector", 428,
                  typeid(vector<lactel::ElementaryCell>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlElactelcLcLElementaryCellgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<lactel::ElementaryCell>) );
      instance.SetNew(&new_vectorlElactelcLcLElementaryCellgR);
      instance.SetNewArray(&newArray_vectorlElactelcLcLElementaryCellgR);
      instance.SetDelete(&delete_vectorlElactelcLcLElementaryCellgR);
      instance.SetDeleteArray(&deleteArray_vectorlElactelcLcLElementaryCellgR);
      instance.SetDestructor(&destruct_vectorlElactelcLcLElementaryCellgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<lactel::ElementaryCell> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<lactel::ElementaryCell>","std::vector<lactel::ElementaryCell, std::allocator<lactel::ElementaryCell> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<lactel::ElementaryCell>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElactelcLcLElementaryCellgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<lactel::ElementaryCell>*>(nullptr))->GetClass();
      vectorlElactelcLcLElementaryCellgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElactelcLcLElementaryCellgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElactelcLcLElementaryCellgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::ElementaryCell> : new vector<lactel::ElementaryCell>;
   }
   static void *newArray_vectorlElactelcLcLElementaryCellgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::ElementaryCell>[nElements] : new vector<lactel::ElementaryCell>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElactelcLcLElementaryCellgR(void *p) {
      delete (static_cast<vector<lactel::ElementaryCell>*>(p));
   }
   static void deleteArray_vectorlElactelcLcLElementaryCellgR(void *p) {
      delete [] (static_cast<vector<lactel::ElementaryCell>*>(p));
   }
   static void destruct_vectorlElactelcLcLElementaryCellgR(void *p) {
      typedef vector<lactel::ElementaryCell> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<lactel::ElementaryCell>

namespace ROOT {
   static TClass *vectorlElactelcLcLBLactelDetectorgR_Dictionary();
   static void vectorlElactelcLcLBLactelDetectorgR_TClassManip(TClass*);
   static void *new_vectorlElactelcLcLBLactelDetectorgR(void *p = nullptr);
   static void *newArray_vectorlElactelcLcLBLactelDetectorgR(Long_t size, void *p);
   static void delete_vectorlElactelcLcLBLactelDetectorgR(void *p);
   static void deleteArray_vectorlElactelcLcLBLactelDetectorgR(void *p);
   static void destruct_vectorlElactelcLcLBLactelDetectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<lactel::BLactelDetector>*)
   {
      vector<lactel::BLactelDetector> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<lactel::BLactelDetector>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<lactel::BLactelDetector>", -2, "vector", 428,
                  typeid(vector<lactel::BLactelDetector>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlElactelcLcLBLactelDetectorgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<lactel::BLactelDetector>) );
      instance.SetNew(&new_vectorlElactelcLcLBLactelDetectorgR);
      instance.SetNewArray(&newArray_vectorlElactelcLcLBLactelDetectorgR);
      instance.SetDelete(&delete_vectorlElactelcLcLBLactelDetectorgR);
      instance.SetDeleteArray(&deleteArray_vectorlElactelcLcLBLactelDetectorgR);
      instance.SetDestructor(&destruct_vectorlElactelcLcLBLactelDetectorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<lactel::BLactelDetector> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<lactel::BLactelDetector>","std::vector<lactel::BLactelDetector, std::allocator<lactel::BLactelDetector> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<lactel::BLactelDetector>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElactelcLcLBLactelDetectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<lactel::BLactelDetector>*>(nullptr))->GetClass();
      vectorlElactelcLcLBLactelDetectorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElactelcLcLBLactelDetectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElactelcLcLBLactelDetectorgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::BLactelDetector> : new vector<lactel::BLactelDetector>;
   }
   static void *newArray_vectorlElactelcLcLBLactelDetectorgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::BLactelDetector>[nElements] : new vector<lactel::BLactelDetector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElactelcLcLBLactelDetectorgR(void *p) {
      delete (static_cast<vector<lactel::BLactelDetector>*>(p));
   }
   static void deleteArray_vectorlElactelcLcLBLactelDetectorgR(void *p) {
      delete [] (static_cast<vector<lactel::BLactelDetector>*>(p));
   }
   static void destruct_vectorlElactelcLcLBLactelDetectorgR(void *p) {
      typedef vector<lactel::BLactelDetector> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<lactel::BLactelDetector>

namespace ROOT {
   static TClass *vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR_Dictionary();
   static void vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR_TClassManip(TClass*);
   static void *new_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p = nullptr);
   static void *newArray_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(Long_t size, void *p);
   static void delete_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p);
   static void deleteArray_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p);
   static void destruct_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<lactel::BAnalysisManager::DAQEvent>*)
   {
      vector<lactel::BAnalysisManager::DAQEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<lactel::BAnalysisManager::DAQEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<lactel::BAnalysisManager::DAQEvent>", -2, "vector", 428,
                  typeid(vector<lactel::BAnalysisManager::DAQEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<lactel::BAnalysisManager::DAQEvent>) );
      instance.SetNew(&new_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR);
      instance.SetNewArray(&newArray_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR);
      instance.SetDelete(&delete_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR);
      instance.SetDestructor(&destruct_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<lactel::BAnalysisManager::DAQEvent> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<lactel::BAnalysisManager::DAQEvent>","std::vector<lactel::BAnalysisManager::DAQEvent, std::allocator<lactel::BAnalysisManager::DAQEvent> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<lactel::BAnalysisManager::DAQEvent>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<lactel::BAnalysisManager::DAQEvent>*>(nullptr))->GetClass();
      vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::BAnalysisManager::DAQEvent> : new vector<lactel::BAnalysisManager::DAQEvent>;
   }
   static void *newArray_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<lactel::BAnalysisManager::DAQEvent>[nElements] : new vector<lactel::BAnalysisManager::DAQEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p) {
      delete (static_cast<vector<lactel::BAnalysisManager::DAQEvent>*>(p));
   }
   static void deleteArray_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p) {
      delete [] (static_cast<vector<lactel::BAnalysisManager::DAQEvent>*>(p));
   }
   static void destruct_vectorlElactelcLcLBAnalysisManagercLcLDAQEventgR(void *p) {
      typedef vector<lactel::BAnalysisManager::DAQEvent> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<lactel::BAnalysisManager::DAQEvent>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 428,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<float>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<float>*>(nullptr))->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete (static_cast<vector<float>*>(p));
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] (static_cast<vector<float>*>(p));
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace {
  void TriggerDictionaryInitialization_libDict_Impl() {
    static const char* headers[] = {
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetector.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorConstruction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorMessenger.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigi.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigitizerModule.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEvent.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEventAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHardwareManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHeader.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHit.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BLactelDetector.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorMessenger.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BStackingAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BSteppingAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BVolumesManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/Utils.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcher.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherConf.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherInterface.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherMessenger.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/DAQManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/Waveform.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/bouta-mohammed/root/root_install/include",
"/home/bouta-mohammed/geant4/geant4.10.01.p03/geant4_install/include/Geant4",
"/usr/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/AA/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/BPostAnalysis/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/K40/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/MottaSchonertSimulation/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/WProp/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/common/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/lactel-dataformat/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tests/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/Geometry/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/utils/include",
"/usr/include/hdf5/serial",
"/home/bouta-mohammed/work/cppm/lactel/trunk",
"/home/bouta-mohammed/root/root_install/include/",
"/home/bouta-mohammed/work/cppm/lactel/trunk/build2/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace CLHEP{class __attribute__((annotate("$clingAutoload$CLHEP/Vector/ThreeVector.h")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  Hep3Vector;}
class __attribute__((annotate("$clingAutoload$Waveform.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  Waveform;
class __attribute__((annotate("$clingAutoload$GeometryObject.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  GeometryObject;
namespace lactel{class __attribute__((annotate("$clingAutoload$BLactelDetector.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  ElementaryCell;}
template <typename T> class __attribute__((annotate("$clingAutoload$GeometryGrid.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  GeometryGrid;

namespace lactel{class __attribute__((annotate("$clingAutoload$BLactelDetector.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  Module;}
namespace lactel{class __attribute__((annotate("$clingAutoload$BLactelDetector.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  BLactelDetector;}
class __attribute__((annotate("$clingAutoload$BDetector.hh")))  __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  BDetector;
namespace lactel{class __attribute__((annotate("$clingAutoload$/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh")))  BAnalysisManager;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libDict dictionary payload"

#ifndef G4_STORE_TRAJECTORY
  #define G4_STORE_TRAJECTORY 1
#endif
#ifndef G4VERBOSE
  #define G4VERBOSE 1
#endif
#ifndef G4UI_USE
  #define G4UI_USE 1
#endif
#ifndef G4VIS_USE
  #define G4VIS_USE 1
#endif
#ifndef G4UI_USE_TCSH
  #define G4UI_USE_TCSH 1
#endif
#ifndef G4INTY_USE_QT
  #define G4INTY_USE_QT 1
#endif
#ifndef G4UI_USE_QT
  #define G4UI_USE_QT 1
#endif
#ifndef G4VIS_USE_OPENGLQT
  #define G4VIS_USE_OPENGLQT 1
#endif
#ifndef G4VIS_USE_OPENGL
  #define G4VIS_USE_OPENGL 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetector.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorConstruction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigi.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigitizerModule.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEvent.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEventAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHardwareManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHeader.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHit.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BLactelDetector.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BStackingAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BSteppingAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BVolumesManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/Utils.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcher.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherConf.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherInterface.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/BWaveCatcherMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/DAQManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include/Waveform.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"BDetector", payloadCode, "@",
"CLHEP::Hep3Vector", payloadCode, "@",
"GeometryGrid<lactel::BLactelDetector>", payloadCode, "@",
"GeometryGrid<lactel::ElementaryCell>", payloadCode, "@",
"GeometryGrid<lactel::Module>", payloadCode, "@",
"GeometryObject", payloadCode, "@",
"Waveform", payloadCode, "@",
"lactel::BAnalysisManager", payloadCode, "@",
"lactel::BAnalysisManager::DAQEvent", payloadCode, "@",
"lactel::BAnalysisManager::Hit", payloadCode, "@",
"lactel::BAnalysisManager::Measure", payloadCode, "@",
"lactel::BAnalysisManager::MeasureParameters", payloadCode, "@",
"lactel::BLactelDetector", payloadCode, "@",
"lactel::ElementaryCell", payloadCode, "@",
"lactel::Module", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDict() {
  TriggerDictionaryInitialization_libDict_Impl();
}
