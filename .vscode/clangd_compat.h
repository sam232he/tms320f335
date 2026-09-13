/*
 * Editor-only stubs so clangd can parse TI C2000 sources.
 * Not used by the CCS / cl2000 build.
 */
#ifndef CLANGD_COMPAT_H
#define CLANGD_COMPAT_H

#ifndef __TMS320C28XX__
#define interrupt
#define cregister
#define far
#define asm(x)
#endif

#endif /* CLANGD_COMPAT_H */
