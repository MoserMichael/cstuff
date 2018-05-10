---
layout: post
title: work with core dumps from remote site
---

{{ page.title }}
----------------

<p class="publish_date">
18 July 2011

</p>
One thing that happens to C/C** programs is that they crash at customer sites, usually a [Core dump](http://en.wikipedia.org/wiki/Core_dump) is left after the crash .

Now it happens that core dump is sent to you from a remote customer site, and joy and wonder, you can't get a meaningful stack trace out of it. (Usually that is the point to start loud chanting of the [Segmentation violation - Core dumped blues](http://www.netfunny.com/rhf/jokes/92q3/coredb.html)

Now
- One reason may be that the [stack is smashed](http://en.wikipedia.org/wiki/Stack_buffer_overflow) in this case you can't get any information, sorry. (well we [have a solution right here](/cstuff/all/projects/2011/06/19/stack-mirror.html) , but that's a different story ...)

- Another reason is that you have compiled the program with Optimization and frame pointer omission enabled, in this case you won't see all information either. Well one options is not to do that ( add -fno-frame-pointer-omission) . Well actually [Yossi Kreinin](http://www.yosefk.com/"), with a [very clever trick](http://www.yosefk.com/blog/getting-the-call-stack-without-a-frame-pointer.html)

- The third one (very likely) is that the crash occurs in a shared library (libc,pthread, oracle client, etc), and it can't be traced, because your system has a slightly different versions of this shared library as compared to what you have locally on your machine. Now this situation can be solved, so now it's scripting time (Perl to the rescue)

- Another scenario is when a system / third party shared library calls you back, In this case you are bound to see a different stack on your machine, of third party library is different from what you have. An example is C** stack unwinding; when an exception is caught, then GLIBC is calling your code, it is calling object destructors of stack based objects.

The script does the following:

- Get path of all dependent shared libraries (parse result of ldd <executable> )

- Copy shared libraries , the core file, the binary file and make an archive out of this
- Create a shell script that instructs GDB to use the copied shared libraries instead of what you got on your system. (here goes the trick)

Now [here is the script](https://github.com/MoserMichael/cstuff/blob/master/scripts/gather-libs.pl) that does it all.

### how to use it

    ./gather-libs.pl -c core.12345 -e exe_file -o out.tbz2

Creates the archive out.tbz2 that includes

- Core file core.12345

- Executable exe\_file and all its dependent shared libraries
- The script run.sh that invokes gdb with the copied shared libraries

Now once you have copied archive out.tbz2 over to your machine you can open it

    tar xvfj out.bz2

Now script run.sh will first check if the executable has any debug information; It happens that applications strip debug information before they ship to a customer, so it is better to check if that happened or not.

If executable has debug information then gdb is run and told to look at the copied shared libraries instead what is installed on your system.
To run with a different core file

    ./run.sh -c core.2345

To run with a different executable and core

    ,/run.sh -c core.2345 -e another-exe:

### How to tell gdb to look at different set of share libraries.

Gdb has the following command

> set solib-absolute-path <path>

It tells gdb to look for shared libraries from under <path>. Now the archive contains script run.sh ; which writes the following gdb script to a temporary file

    set solib-absolute-prefix .
    core-file <core file name>

Then it starts gdb with this generated script (-x option of gdb).
Now doing a script that writes another script is an exercise in magic (small magic alas).

### Limitations

As always, there are situation when this script does not work properly; For example if the application loads shared libraries dynamically with dlopen / dlsym, then those will not be copied into the archive.
(Please tell me that this does not happen often on Linux; Sorry).

### Interesting core dump trivia

Once upon a time we got a core file that was huge - about three gigabytes large. Can it be understood from the size of the core dump that the application has run out of memory?
Well the answer is a bit tricky,
Let's remember a few things about [Virtual memory](http://en.wikipedia.org/wiki/Virtual_memory) - there is the 'Virtual memory size' - for a given process these are all virtual address ranges summed up; it is the number of all reserved virtual addresses; these addresses may or may not be backed up with real memory.
Then there is the 'Resident set size' - the number of all physical memory pages that are in use by a process multiplied by page size; (a memory page Is a fixed size unit that backs up a range of virtual memory / is the real memory behind a range of virtual addresses; a page is typically 4096 or 8192 bytes long).
The question is now, can the 'Resident set size' be extracted from the core dump?
Now a core dump is just an [ELF format binary file](http://en.wikipedia.org/wiki/Executable_and_Linkable_Format) with a special section called "note" - information about the core dump, registers at the time of the crash, etc.
The core dump is created by the Linux kernel; so let's search for the function that is writing the core dump, the [Linux cross reference project](http://lxr.linux.no/+trees) can be used to search the kernel sources, searches here are faster than grep !

- First the ELF binary format is described in header [include/linux/elf.h](http://lxr.linux.no/#linux+v3.0.4/include/linux/elf.h) ;

- "structure elf32\_note": http://lxr.linux.no/\#linux+v3.0.4/include/linux/elf.h\#L402 is defined here, it describes the note header that describes the core file. So we should search for platform independent [elf\_note](http://lxr.linux.no/#linux+v3.0.4/include/linux/elf.h#L422)
- Now function [elf\_core\_dump](http://lxr.linux.no/#linux+v3.0.4/fs/binfmt_elf.c#L1875) seems to be writing the core dump.

The function first writes the executable header and note sections (lines 1903 - 1978) ; then an elf section header is written for each virtual memory address range (1981- 2003) then at line 2019 it starts to get interesting , it writes the actual memory pages into the note ext section.
Line 2019 loops on all virtual memory ranges; for each range the nested loop at line 2026 loops over all pages that make up a memory range. So line 2030 gets the content of a memory page, if the page is available and backed up by physical memory then it is written into the file (2034), if not then line 2039 just extends the file by the size of one range and puts zeroes into it.

So know we know that the core file is equal to the size of the virtual memory. In principle one could search for consecutive zero ranges, but then it is impossible to tell if a range of zeros is a range of virtual addresses that is not backed by physical memory, or a page of physical memory that is all zero.

The loop that writes the memory ranges.

    2019        for (vma = first_vma(current, gate_vma); vma != NULL;
    2020                        vma = next_vma(vma, gate_vma)) {
    2021                unsigned long addr;
    2022                unsigned long end;
    2023
    2024                end = vma->vm_start + vma_dump_size(vma, cprm->mm_flags);
    2025
    2026                for (addr = vma->vm_start; addr < end; addr += PAGE_SIZE) {
    2027                        struct page *page;
    2028                        int stop;
    2029
    2030                        page = get_dump_page(addr);
    2031                        if (page) {
    2032                                void *kaddr = kmap(page);
    2033                                stop = ((size += PAGE_SIZE) > cprm->limit) ||
    2034                                        !dump_write(cprm->file, kaddr,
    2035                                                    PAGE_SIZE);
    2036                                kunmap(page);
    2037                                page_cache_release(page);
    2038                        } else
    2039                                stop = !dump_seek(cprm->file, PAGE_SIZE);
    2040                        if (stop)
    2041                                goto end_coredump;
    2042                }
    2043        }

The big function

    1868/*
    1869 * Actual dumper
    1870 *
    1871 * This is a two-pass process; first we find the offsets of the bits,
    1872 * and then they are actually written out.  If we run out of core limit
    1873 * we just truncate.
    1874 */
    1875static int elf_core_dump(struct coredump_params *cprm)
    1876{
    1877        int has_dumped = 0;
    1878        mm_segment_t fs;
    1879        int segs;
    1880        size_t size = 0;
    1881        struct vm_area_struct *vma, *gate_vma;
    1882        struct elfhdr *elf = NULL;
    1883        loff_t offset = 0, dataoff, foffset;
    1884        struct elf_note_info info;
    1885        struct elf_phdr *phdr4note = NULL;
    1886        struct elf_shdr *shdr4extnum = NULL;
    1887        Elf_Half e_phnum;
    1888        elf_addr_t e_shoff;
    1889
    1890        /*
    1891         * We no longer stop all VM operations.
    1892         * 
    1893         * This is because those proceses that could possibly change map_count
    1894         * or the mmap / vma pages are now blocked in do_exit on current
    1895         * finishing this core dump.
    1896         *
    1897         * Only ptrace can touch these memory addresses, but it doesn't change
    1898         * the map_count or the pages allocated. So no possibility of crashing
    1899         * exists while dumping the mm->vm_next areas to the core file.
    1900         */
    1901  
    1902        /* alloc memory for large data structures: too large to be on stack */
    1903        elf = kmalloc(sizeof(*elf), GFP_KERNEL);
    1904        if (!elf)
    1905                goto out;
    1906        /*
    1907         * The number of segs are recored into ELF header as 16bit value.
    1908         * Please check DEFAULT_MAX_MAP_COUNT definition when you modify here.
    1909         */
    1910        segs = current->mm->map_count;
    1911        segs += elf_core_extra_phdrs();
    1912
    1913        gate_vma = get_gate_vma(current->mm);
    1914        if (gate_vma != NULL)
    1915                segs++;
    1916
    1917        /* for notes section */
    1918        segs++;
    1919
    1920        /* If segs > PN_XNUM(0xffff), then e_phnum overflows. To avoid
    1921         * this, kernel supports extended numbering. Have a look at
    1922         * include/linux/elf.h for further information. */
    1923        e_phnum = segs > PN_XNUM ? PN_XNUM : segs;
    1924
    1925        /*
    1926         * Collect all the non-memory information about the process for the
    1927         * notes.  This also sets up the file header.
    1928         */
    1929        if (!fill_note_info(elf, e_phnum, &info, cprm->signr, cprm->regs))
    1930                goto cleanup;
    1931
    1932        has_dumped = 1;
    1933        current->flags |= PF_DUMPCORE;
    1934  
    1935        fs = get_fs();
    1936        set_fs(KERNEL_DS);
    1937
    1938        offset += sizeof(*elf);                         /* Elf header */
    1939        offset += segs * sizeof(struct elf_phdr);       /* Program headers */
    1940        foffset = offset;
    1941
    1942        /* Write notes phdr entry */
    1943        {
    1944                size_t sz = get_note_info_size(&info);
    1945
    1946                sz += elf_coredump_extra_notes_size();
    1947
    1948                phdr4note = kmalloc(sizeof(*phdr4note), GFP_KERNEL);
    1949                if (!phdr4note)
    1950                        goto end_coredump;
    1951
    1952                fill_elf_note_phdr(phdr4note, sz, offset);
    1953                offset += sz;
    1954        }
    1955
    1956        dataoff = offset = roundup(offset, ELF_EXEC_PAGESIZE);
    1957
    1958        offset += elf_core_vma_data_size(gate_vma, cprm->mm_flags);
    1959        offset += elf_core_extra_data_size();
    1960        e_shoff = offset;
    1961
    1962        if (e_phnum == PN_XNUM) {
    1963                shdr4extnum = kmalloc(sizeof(*shdr4extnum), GFP_KERNEL);
    1964                if (!shdr4extnum)
    1965                        goto end_coredump;
    1966                fill_extnum_info(elf, shdr4extnum, e_shoff, segs);
    1967        }
    1968
    1969        offset = dataoff;
    1970
    1971        size += sizeof(*elf);
    1972        if (size > cprm->limit || !dump_write(cprm->file, elf, sizeof(*elf)))
    1973                goto end_coredump;
    1974
    1975        size += sizeof(*phdr4note);
    1976        if (size > cprm->limit
    1977            || !dump_write(cprm->file, phdr4note, sizeof(*phdr4note)))
    1978                goto end_coredump;
    1979
    1980        /* Write program headers for segments dump */
    1981        for (vma = first_vma(current, gate_vma); vma != NULL;
    1982                        vma = next_vma(vma, gate_vma)) {
    1983                struct elf_phdr phdr;
    1984
    1985                phdr.p_type = PT_LOAD;
    1986                phdr.p_offset = offset;
    1987                phdr.p_vaddr = vma->vm_start;
    1988                phdr.p_paddr = 0;
    1989                phdr.p_filesz = vma_dump_size(vma, cprm->mm_flags);
    1990                phdr.p_memsz = vma->vm_end - vma->vm_start;
    1991                offset += phdr.p_filesz;
    1992                phdr.p_flags = vma->vm_flags & VM_READ ? PF_R : 0;
    1993                if (vma->vm_flags & VM_WRITE)
    1994                        phdr.p_flags |= PF_W;
    1995                if (vma->vm_flags & VM_EXEC)
    1996                        phdr.p_flags |= PF_X;
    1997                phdr.p_align = ELF_EXEC_PAGESIZE;
    1998
    1999                size += sizeof(phdr);
    2000                if (size > cprm->limit
    2001                    || !dump_write(cprm->file, &phdr, sizeof(phdr)))
    2002                        goto end_coredump;
    2003        }
    2004
    2005        if (!elf_core_write_extra_phdrs(cprm->file, offset, &size, cprm->limit))
    2006                goto end_coredump;
    2007
    2008        /* write out the notes section */
    2009        if (!write_note_info(&info, cprm->file, &foffset))
    2010                goto end_coredump;
    2011
    2012        if (elf_coredump_extra_notes_write(cprm->file, &foffset))
    2013                goto end_coredump;
    2014
    2015        /* Align to page */
    2016        if (!dump_seek(cprm->file, dataoff - foffset))
    2017                goto end_coredump;
    2018
    2019        for (vma = first_vma(current, gate_vma); vma != NULL;
    2020                        vma = next_vma(vma, gate_vma)) {
    2021                unsigned long addr;
    2022                unsigned long end;
    2023
    2024                end = vma->vm_start + vma_dump_size(vma, cprm->mm_flags);
    2025
    2026                for (addr = vma->vm_start; addr < end; addr += PAGE_SIZE) {
    2027                        struct page *page;
    2028                        int stop;
    2029
    2030                        page = get_dump_page(addr);
    2031                        if (page) {
    2032                                void *kaddr = kmap(page);
    2033                                stop = ((size += PAGE_SIZE) > cprm->limit) ||
    2034                                        !dump_write(cprm->file, kaddr,
    2035                                                    PAGE_SIZE);
    2036                                kunmap(page);
    2037                                page_cache_release(page);
    2038                        } else
    2039                                stop = !dump_seek(cprm->file, PAGE_SIZE);
    2040                        if (stop)
    2041                                goto end_coredump;
    2042                }
    2043        }
    2044
    2045        if (!elf_core_write_extra_data(cprm->file, &size, cprm->limit))
    2046                goto end_coredump;
    2047
    2048        if (e_phnum == PN_XNUM) {
    2049                size += sizeof(*shdr4extnum);
    2050                if (size > cprm->limit
    2051                    || !dump_write(cprm->file, shdr4extnum,
    2052                                   sizeof(*shdr4extnum)))
    2053                        goto end_coredump;
    2054        }
    2055
    2056end_coredump:
    2057        set_fs(fs);
    2058
    2059cleanup:
    2060        free_note_info(&info);
    2061        kfree(shdr4extnum);
    2062        kfree(phdr4note);
    2063        kfree(elf);
    2064out:
    2065        return has_dumped;
    2066}
    2067
