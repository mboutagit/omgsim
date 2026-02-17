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
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorConstruction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigi.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigitizerModule.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEvent.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEventAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHardwareManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHit.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BSteppingAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/Utils.hh"

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

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lactel::BAnalysisManager*)
   {
      ::lactel::BAnalysisManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::lactel::BAnalysisManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("lactel::BAnalysisManager", ::lactel::BAnalysisManager::Class_Version(), "BAnalysisManager.hh", 24,
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
         instance("lactel::BAnalysisManager::MeasureParameters", ::lactel::BAnalysisManager::MeasureParameters::Class_Version(), "BAnalysisManager.hh", 56,
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
         instance("lactel::BAnalysisManager::Measure", ::lactel::BAnalysisManager::Measure::Class_Version(), "BAnalysisManager.hh", 81,
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

namespace {
  void TriggerDictionaryInitialization_libDict_Impl() {
    static const char* headers[] = {
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BAnalysisManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorConstruction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorMessenger.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigi.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigitizerModule.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEvent.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEventAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHardwareManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHit.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorMessenger.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunManager.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BSteppingAction.hh",
"/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/Utils.hh",
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
"/home/bouta-mohammed/work/cppm/lactel/trunk/tools/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk/utils/include",
"/home/bouta-mohammed/work/cppm/lactel/trunk",
"/home/bouta-mohammed/root/root_install/include/",
"/home/bouta-mohammed/work/cppm/lactel/trunk/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
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
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorConstruction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDetectorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigi.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BDigitizerModule.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEvent.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BEventAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHardwareManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BHit.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BPrimaryGeneratorMessenger.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BRunManager.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/BSteppingAction.hh"
#include "/home/bouta-mohammed/work/cppm/lactel/trunk/LACTELSim/include/Utils.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"lactel::BAnalysisManager", payloadCode, "@",
"lactel::BAnalysisManager::Measure", payloadCode, "@",
"lactel::BAnalysisManager::MeasureParameters", payloadCode, "@",
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
