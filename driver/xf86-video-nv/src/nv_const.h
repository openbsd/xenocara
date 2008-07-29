#ifndef __NV_CONST_H__
#define __NV_CONST_H__

#define NV_VERSION 4000
#define NV_NAME "NV"
#define NV_DRIVER_NAME "nv"

/* libpciaccess gratuitous API changes */
#if XSERVER_LIBPCIACCESS
 #define MEMBASE(p,n)     (p)->regions[n].base_addr
 #define VENDOR_ID(p)     (p)->vendor_id
 #define DEVICE_ID(p)     (p)->device_id
 #define SUBVENDOR_ID(p)  (p)->subvendor_id
 #define SUBDEVICE_ID(p)  (p)->subdevice_id
 #define CHIP_REVISION(p) (p)->revision
#else
 #define MEMBASE(p,n)     (p)->memBase[n]
 #define VENDOR_ID(p)     (p)->vendor
 #define DEVICE_ID(p)     (p)->chipType
 #define SUBVENDOR_ID(p)  (p)->subsysVendor
 #define SUBDEVICE_ID(p)  (p)->subsysCard
 #define CHIP_REVISION(p) (p)->chipRev
#endif

#endif /* __NV_CONST_H__ */
