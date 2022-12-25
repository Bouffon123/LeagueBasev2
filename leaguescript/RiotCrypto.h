#pragma once

#include <Windows.h>

namespace Script {
namespace Native {

	/* It is necessary to specify alignment the packing to 4 bytes */
#pragma pack( push, 4 )
	/* Obfuscated value type */
	template<class Type = DWORD>
	class Obfuscation
	{
	public:
		unsigned char IsFilled;
		unsigned char LengthXor32;
		unsigned char LengthXor8;
		Type Key;
		unsigned char Index;
		Type Values[4];
	public:
		inline operator Type()
		{
			Type Result = sizeof(Type) == 1 ? this->Values[(this->Index + 1) & 3] : this->Values[this->Index];
			if (this->LengthXor32)
			{
				for (unsigned char i = 0; i < this->LengthXor32; i++)
				{
					reinterpret_cast<PDWORD>(&Result)[i] ^= ~(reinterpret_cast<PDWORD>(&this->Key)[i]);
				}
			}
			if (this->LengthXor8)
			{
				for (unsigned char i = sizeof(Type) - this->LengthXor8; i < sizeof(Type); i++)
				{
					reinterpret_cast<PBYTE>(&Result)[i] ^= ~(reinterpret_cast<PBYTE>(&this->Key)[i]);
				}
			}
			return Result;
		}
	};
#pragma pack( pop )

    template <typename T>
    __declspec(noinline) T DecryptField(void* ptr) {

        auto _this = (DWORD)ptr - 0x4;
        T result = *(T*)(_this + 4 * *(BYTE*)(_this + 0xC) + 0x10);

        auto large_size = *(BYTE*)(_this + 0x5);

        if (large_size) {

            auto key_it = (DWORD*)(_this + 0x8);
            auto i = 0;
            do
            {
                auto key_dir = *key_it++;
                *((DWORD*)&result + i++) ^= ~(key_dir);
                i++;
            } while (i < large_size);
        }

        return *reinterpret_cast<T*>(&result);
    }


    template <>
    inline bool DecryptField<bool>(void* ptr) {
        return !(*reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned>(ptr) + 0x1) % 2);
    }
}
}