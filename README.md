## paste-smasher
paste-smasher is an injectable dll that is targeted towards dumping p2c's that manually map their dll and that use WriteProcessMemory and VirtualAllocEx in their injectors.

# features:
1. hooks syscall for NtWriteVirtualMemory
2. hooks VirtualAllocEx
3. console logging
4. file logging
5. dumping memory buffer to file as it's written

# how do i use this?
1. compile in release/debug in x86/x64 depending on your target process.
2. edit the settings of the dumper to your likings inside of hooks/hooks.h.
3. inject into your target loader with your favorite injector can be manually mapped or injected with any injector.
4. a "dumps" directory will be created next to the dll if you have dumping enabled, and a log file will be created next to the dumper's dll if you have file logging enabled as well.

# example log output:
```cpp
// example output from log file
[paste-smasher] setup hooks
[paste-smasher] virtual_alloc_ex: [in_addr: 00000000, ret_addr: 4E090000, alloc_size: 18792448, type: 12288, prot: 64, idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4E090000, buffer: 07130040, size: 1024, idx: 1, alloc_idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4E091000, buffer: 07130440, size: 454144, idx: 2, alloc_idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4E100000, buffer: 0719F240, size: 123904, idx: 3, alloc_idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4E11F000, buffer: 071BD640, size: 5120, idx: 4, alloc_idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4F270000, buffer: 071BEA40, size: 10240, idx: 5, alloc_idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4F273000, buffer: 071C1240, size: 512, idx: 6, alloc_idx: 1]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 4F274000, buffer: 071C1440, size: 30720, idx: 7, alloc_idx: 1]
[paste-smasher] virtual_alloc_ex: [in_addr: 00000000, ret_addr: 1DF60000, alloc_size: 4096, type: 12288, prot: 64, idx: 2]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 1DF60000, buffer: 01CFF2CC, size: 24, idx: 8, alloc_idx: 2]
[paste-smasher] nt_write_virtual_memory: [handle: 00000670, addr: 1DF60018, buffer: 0020F060, size: 256, idx: 9, alloc_idx: 2]
```

# credits:
TsudaKageyu - [minhook](https://github.com/TsudaKageyu/minhook)
, JustasMasiulis - [xorstr](https://github.com/JustasMasiulis/xorstr)
