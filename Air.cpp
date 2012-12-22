#include "Air.h"
#include <usb.h>

struct usb_dev_handle *devh;

void release_usb_device(int dummy) {
    int ret;
    ret = usb_release_interface(devh, 0);
    usb_close(devh);
    exit(1);
}

struct usb_device* find_device(int vendor, int product) {
    struct usb_bus *bus;
    printf("as");
    for (bus = usb_get_busses(); bus; bus = bus->next) {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next) {
            if (dev->descriptor.idVendor == vendor
                && dev->descriptor.idProduct == product)
                return dev;
        }
    }
    return NULL;
}

int main () 
{
  int ret, vendor, product;
  struct usb_device *dev;
  char buf[1000];
  printf("hello");

  usb_init();
  usb_set_debug(255);
  usb_find_busses();
  usb_find_devices();

  vendor  = 0x03eb;
  product = 0x2013;
  dev = find_device(vendor, product);
  printf("device");  
  if (dev == NULL){
    printf("\nCouldn't find device 03eb:2013!\n\n");
    exit(1);
  }
  printf("HERE");
  assert(dev);

  devh = usb_open(dev);
  assert(devh);
  
  signal(SIGTERM, release_usb_device);

  ret = usb_get_driver_np(devh, 0, buf, sizeof(buf));
  if (ret == 0) {
      printf("interface 0 already claimed by driver \"%s\", attempting to detach it\n", buf);
      ret = usb_detach_kernel_driver_np(devh, 0);
      printf("usb_detach_kernel_driver_np returned %d\n", ret);
  }
  printf("woohoo\n"); 

  ret = usb_claim_interface(devh, 0);
  if (ret != 0) {
      printf("claim failed with error %d\n", ret);
        exit(1);
  }
  ret = usb_set_altinterface(devh, 0);

  assert(ret >= 0);


  ret = usb_get_descriptor(devh, 0x0000001, 0x0000000, buf, 0x0000012);
  printf("desc1 %d\n", ret);
  usleep(3*1000);
  ret = usb_get_descriptor(devh, 0x0000002, 0x0000000, buf, 0x0000009);
  printf("desc2 %d\n", ret);
  usleep(2*1000);
  ret = usb_get_descriptor(devh, 0x0000002, 0x0000000, buf, 0x0000029);
  printf("desc3 %d\n", ret);
  usleep(2*1000);
  ret = usb_release_interface(devh, 0);
  printf("release %d\n", ret);
  ret = usb_set_configuration(devh, 0x0000001);
  printf("config %d\n", ret);
  ret = usb_claim_interface(devh, 0);
  printf("claim %d\n", ret);
  ret = usb_set_altinterface(devh, 0);
  printf("%d, %d\n", USB_TYPE_CLASS + USB_RECIP_INTERFACE, buf);
  usleep(34*1000);
  ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 0x000000a, 0x0000000, 0x0000000, buf, 0x0000000, 1000);
  printf("control %d\n", ret);
  usleep(11*1000);
  ret = usb_get_descriptor(devh, 0x0000022, 0x0000000, buf, 0x0000075);
  printf("get desc %d\n", ret);
  usleep(4*1000);
  ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
  printf("read1 %d\n", ret);
  ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
  printf("read2 %d\n", ret);
  ret = usb_release_interface(devh, 0);
  //usb_reset(devh);
  usb_close(devh);
/*
  int c0 = 104;
  int c1 = 37;
  unsigned short iresult;
  while(0==0){
    usleep(2500*1000);
    memcpy(buf, "\x40\x68\x2a\x54\x52\x0a\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40", 0x0000010);
    buf[1] = (char) c0;
    c0++;
    if (c0 == 256) c0 = 103;
    ret = usb_interrupt_write(devh, 0x00000002, buf, 0x0000010, 1000);
    usleep(94*1000);
    ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
    // Seems to yield a realistic value...
    memcpy(&iresult,buf+2,2);
    iresult /= 2;
		
    usleep(14*1000);
    ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
    
    usleep(3*1000);
    memcpy(buf, "\x40\x30\x30\x30\x37\x46\x4c\x41\x47\x47\x45\x54\x3f\x0a\x40\x40", 0x0000010);
    buf[4] = (char) c1;
    c1++;
    if (c1 == 40) c1++;
    else if (c1 == 47) c1=30;
    ret = usb_interrupt_write(devh, 0x00000002, buf, 0x0000010, 1000);
    
    usleep(23*1000);
    ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
    usleep(14*1000);
    ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
    usleep(8*1000);
    ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000010, 1000);
  }*/

	return 0; 
}
