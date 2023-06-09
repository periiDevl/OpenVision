//
// Copyright (C) Microsoft Corporation
// All rights reserved.
//

#include "pch.h"

extern __abi_Module* __abi_module;

namespace Platform {
	namespace Details {

		//Forward declarations from vccorlib120.dll
		void STDMETHODCALLTYPE RunServer(_In_ Microsoft::WRL::Details::ModuleBase**, __abi_Module** abiModule, _In_opt_z_ const ::default::char16*);
		__declspec(no_refcount) ::Platform::Object^ STDMETHODCALLTYPE RegisterFactories(
			_In_ Microsoft::WRL::Details::ModuleBase**,
			__abi_Module** abiModule,
			_In_opt_ ::Platform::Module::UnregisterCallback callback);

	} // namespace Details

	// vccorlib.h does not include sal.h for annotations
	#pragma warning(suppress: 28301) // C28301: No annotations for first declaration of 'RunServer'
	void STDMETHODCALLTYPE Module::RunServer(_In_opt_z_ const ::default::char16* serverName)
	{
		Platform::Details::RunServer(&Microsoft::WRL::Details::ModuleBase::module_, &__abi_module, serverName);
	}

	// vccorlib.h does not include sal.h for annotations
	#pragma warning(suppress: 28301) // C28301: No annotations for first declaration of 'RegisterFactories'
	__declspec(no_refcount) ::Platform::Object^ STDMETHODCALLTYPE Module::RegisterFactories(_In_opt_ ::Platform::Module::UnregisterCallback callback)
	{
		return Platform::Details::RegisterFactories(&Microsoft::WRL::Details::ModuleBase::module_, &__abi_module, callback);
	}

} // namespace Platform
