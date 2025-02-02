﻿/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#pragma once

#include <cassert>
#include <vector>
#include <string>
#include <string_view>
#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Psapi.h>
#elif defined(__linux__)
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <dlfcn.h>
#endif

namespace hook
{
	extern ptrdiff_t baseAddressDifference;
	extern uintptr_t baseAddress;
	extern uintptr_t endAddress;

	inline void find_addr_set()
	{
#if defined(_WIN32) || defined(_WIN64)
		MODULEINFO moduleInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo));
		auto ms_BaseAddr = reinterpret_cast<DWORD64>(moduleInfo.lpBaseOfDll);
		auto ms_EndAddr = ms_BaseAddr + moduleInfo.SizeOfImage;

		baseAddress = ms_BaseAddr;
		endAddress = ms_EndAddr;
#elif defined(__linux__)
		char buf[4096];
		ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
		if (len != -1)
		{
			buf[len] = '\0';
			auto ms_BaseAddr = reinterpret_cast<uintptr_t>(dlopen(buf, RTLD_LAZY));
			auto ms_EndAddr = ms_BaseAddr + len;

			baseAddress = ms_BaseAddr;
			endAddress = ms_EndAddr;
		}
		else
		{
			auto ms_BaseAddr = reinterpret_cast<uintptr_t>(dlopen(nullptr, RTLD_LAZY));
			auto ms_EndAddr = ms_BaseAddr + 0x1000;

			baseAddress = ms_BaseAddr;
			endAddress = ms_EndAddr;
		}
#endif
	}

	// sets the base address difference based on an obtained pointer
	inline void set_base(uintptr_t address)
	{
#ifdef _M_IX86
		uintptr_t addressDiff = (address - 0x400000);
#elif defined(_M_AMD64)
		uintptr_t addressDiff = (address - 0x140000000);
#endif

		// pointer-style cast to ensure unsigned overflow ends up copied directly into a signed value
		baseAddressDifference = *(ptrdiff_t*)&addressDiff;
	}

	// sets the base to the process main base
	void set_base();

	inline uintptr_t getRVA(uintptr_t rva)
	{
		set_base();
#ifdef _M_IX86
		return static_cast<uintptr_t>(baseAddressDifference + 0x400000 + rva);
#elif defined(_M_AMD64)
		return static_cast<uintptr_t>(baseAddressDifference + 0x140000000 + rva);
#endif
	}

	class pattern_match
	{
	private:
		void* m_pointer;

	public:
		inline pattern_match(void* pointer)
			: m_pointer(pointer)
		{
		}

		template<typename T>
		T* get(ptrdiff_t offset = 0) const
		{
			char* ptr = reinterpret_cast<char*>(m_pointer);
			return reinterpret_cast<T*>(ptr + offset);
		}
	};

	class pattern
	{
	private:
		std::basic_string<uint8_t> m_bytes;
		std::basic_string<uint8_t> m_mask;

#if PATTERNS_USE_HINTS
		uint64_t m_hash;
#endif

		std::vector<pattern_match> m_matches;

		bool m_matched = false;

		uintptr_t m_rangeStart;
		uintptr_t m_rangeEnd;

	private:
		void Initialize(std::string_view pattern);

		bool ConsiderHint(uintptr_t offset);

		void EnsureMatches(uint32_t maxCount);

		inline pattern_match _get_internal(size_t index) const
		{
			return m_matches[index];
		}

		inline pattern(uintptr_t module)
			: pattern(module, 0)
		{
		}

		inline pattern(uintptr_t begin, uintptr_t end)
			: m_rangeStart(begin), m_rangeEnd(end)
		{
		}

	public:
		pattern(std::string_view pattern)
			: hook::pattern(getRVA(0))
		{
			Initialize(std::move(pattern));
		}

		inline pattern(void* module, std::string_view pattern)
			: hook::pattern(reinterpret_cast<uintptr_t>(module))
		{
			Initialize(std::move(pattern));
		}

		inline pattern(uintptr_t begin, uintptr_t end, std::string_view pattern)
			: m_rangeStart(begin), m_rangeEnd(end)
		{
			Initialize(std::move(pattern));
		}

		inline pattern&& count(uint32_t expected)
		{
			EnsureMatches(expected);
			assert(m_matches.size() == expected);
			return std::forward<pattern>(*this);
		}

		inline pattern&& count_hint(uint32_t expected)
		{
			EnsureMatches(expected);
			return std::forward<pattern>(*this);
		}

		inline pattern&& clear()
		{
			m_matches.clear();
			m_matched = false;
			return std::forward<pattern>(*this);
		}

		inline size_t size()
		{
			EnsureMatches(UINT32_MAX);
			return m_matches.size();
		}

		inline bool empty()
		{
			return size() == 0;
		}

		inline pattern_match get(size_t index)
		{
			EnsureMatches(UINT32_MAX);
			return _get_internal(index);
		}

		inline pattern_match get_one()
		{
			return std::forward<pattern>(*this).count(1)._get_internal(0);
		}

		template<typename T = void>
		inline auto get_first(ptrdiff_t offset = 0)
		{
			return get_one().get<T>(offset);
		}

		template <typename Pred>
		inline Pred for_each_result(Pred&& pred)
		{
			EnsureMatches(UINT32_MAX);
			for (auto it : m_matches)
			{
				std::forward<Pred>(pred)(it);
			}
			return std::forward<Pred>(pred);
		}

	public:
#if PATTERNS_USE_HINTS && PATTERNS_CAN_SERIALIZE_HINTS
		// define a hint
		static void hint(uint64_t hash, uintptr_t address);
#endif
	};

	inline pattern make_module_pattern(void* module, std::string_view bytes)
	{
		return pattern(module, std::move(bytes));
	}

	inline pattern make_range_pattern(uintptr_t begin, uintptr_t end, std::string_view bytes)
	{
		return pattern(begin, end, std::move(bytes));
	}

	template<typename T = void>
	inline auto get_pattern(std::string_view pattern_string, ptrdiff_t offset = 0)
	{
		return pattern(std::move(pattern_string)).get_first<T>(offset);
	}

	inline uintptr_t find_pattern(const std::string& pattern) {
		auto copy = pattern;
		for (size_t pos = copy.find("??"); pos != std::string::npos; pos = copy.find("??", pos + 1))
		{
			copy.replace(pos, 2, "?");
		}

		auto thePattern = baseAddress == 0 && endAddress == 0
			? hook::pattern(copy)
			: hook::pattern(baseAddress, endAddress, copy);
		if (!thePattern.size())
		{
			return 0;
		}

		return uintptr_t(thePattern.get_first());
	}
}