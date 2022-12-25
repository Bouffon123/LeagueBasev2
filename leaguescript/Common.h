#pragma once

#include "RawCommon.h"
#include "RiotCrypto.h"
#include "Addresses.h"
#include "Structures.h"
#include "Context.h"

#pragma warning(push)
#pragma warning(disable:4251)

#define SCRIPT_VERSION "Version 12.13.453.3037 [PUBLIC]"

#define MAKE_RVA(address) (static_cast<DWORD>(g_ctx->mModuleBase) + static_cast<DWORD>(address))

#define MAKEGET(TYPE, NAME) public: TYPE NAME##();
#define MAKEGET_C(TYPE, NAME) \
  TYPE FOCUS_CLASS##::NAME##() { \
  return *reinterpret_cast< TYPE##* >( reinterpret_cast< unsigned >( this ) + static_cast< unsigned >( Script::Native::Structures::##FOCUS_CLASS##::##NAME ) ); \
  }

#define MAKEGET_ENC_C(TYPE, NAME) \
  TYPE FOCUS_CLASS##::NAME##() { \
  return Script::Native::DecryptField<##TYPE##>( reinterpret_cast< void* >( reinterpret_cast< unsigned >( this ) + static_cast< unsigned >( Script::Native::Structures::##FOCUS_CLASS##::##NAME ) ) ); \
  }

#define MAKEGET_REF(TYPE, NAME) public: TYPE##* NAME##();
#define MAKEGET_REF_C(TYPE, NAME) \
  TYPE##* FOCUS_CLASS##::NAME##() { \
  return reinterpret_cast< TYPE##* >( reinterpret_cast< unsigned >( this ) + static_cast< unsigned >( Script::Native::Structures::##FOCUS_CLASS##::##NAME ) ); \
  }

#define MAKEGET_STR( NAME) public: std::string NAME##();
#define MAKEGET_STR_C( NAME) \
  std::string FOCUS_CLASS##::NAME##() { \
  return ( reinterpret_cast< LolString##* >( reinterpret_cast< unsigned >( this ) + static_cast< unsigned >( Script::Native::Structures::##FOCUS_CLASS##::##NAME ) ) )->Get(); \
  }