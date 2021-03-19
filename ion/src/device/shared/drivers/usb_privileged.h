#ifndef ION_DEVICE_SHARED_USB_PRIVILEGED_H
#define ION_DEVICE_SHARED_USB_PRIVILEGED_H

namespace Ion {
namespace Device {
namespace USB {

void init();
void shutdown();
void initGPIO();
void shutdownGPIO();
void initOTG();
void shutdownOTG();

}
}
}

#endif
