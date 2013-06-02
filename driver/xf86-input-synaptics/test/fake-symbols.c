#include "fake-symbols.h"

_X_EXPORT int
xf86ReadSerial(int fd, void *buf, int count)
{
    return 0;
}

_X_EXPORT int
xf86WriteSerial(int fd, const void *buf, int count)
{
    return 0;
}

_X_EXPORT int
xf86CloseSerial(int fd)
{
    return 0;
}

_X_EXPORT int
xf86WaitForInput(int fd, int timeout)
{
    return 0;
}

_X_EXPORT int
xf86OpenSerial(XF86OptionPtr options)
{
    return 0;
}

_X_EXPORT int
xf86SetSerialSpeed(int fd, int speed)
{
    return 0;
}

_X_EXPORT XF86OptionPtr
xf86ReplaceIntOption(XF86OptionPtr optlist, const char *name, const int val)
{
    return NULL;
}

_X_EXPORT char *
xf86SetStrOption(XF86OptionPtr optlist, const char *name, const char *deflt)
{
    return NULL;
}

_X_EXPORT int
xf86SetBoolOption(XF86OptionPtr optlist, const char *name, int deflt)
{
    return 0;
}

_X_EXPORT XF86OptionPtr
xf86AddNewOption(XF86OptionPtr head, const char *name, const char *val)
{
    return NULL;
}

_X_EXPORT const char *
xf86FindOptionValue(XF86OptionPtr options, const char *name)
{
    return NULL;
}

_X_EXPORT char *
xf86OptionName(XF86OptionPtr opt)
{
    return NULL;
}

_X_EXPORT char *
xf86OptionValue(XF86OptionPtr opt)
{
    return NULL;
}

_X_EXPORT int
xf86NameCmp(const char *s1, const char *s2)
{
    return 0;
}

_X_EXPORT void
xf86AddEnabledDevice(InputInfoPtr pInfo)
{
    return;
}

_X_EXPORT void
xf86RemoveEnabledDevice(InputInfoPtr pInfo)
{
    return;
}

_X_EXPORT Atom
XIGetKnownProperty(char *name)
{
    return None;
}

_X_EXPORT void
xf86AddInputDriver(InputDriverPtr driver, pointer module, int flags)
{
    return;
}

_X_EXPORT int
xf86ScaleAxis(int Cx, int to_max, int to_min, int from_max, int from_min)
{
    int X;
    int64_t to_width = to_max - to_min;
    int64_t from_width = from_max - from_min;

    if (from_width) {
        X = (int) (((to_width * (Cx - from_min)) / from_width) + to_min);
    }
    else {
        X = 0;
        /*ErrorF ("Divide by Zero in xf86ScaleAxis\n"); */
    }

    if (X > to_max)
        X = to_max;
    if (X < to_min)
        X = to_min;

    return X;
}

_X_EXPORT void
DeleteInputDeviceRequest(DeviceIntPtr pDev)
{
    return;
}

_X_EXPORT void
FreeInputAttributes(InputAttributes * attrs)
{
    return;
}

_X_EXPORT void
xf86PostButtonEvent(DeviceIntPtr device,
                    int is_absolute,
                    int button,
                    int is_down, int first_valuator, int num_valuators, ...)
{
    return;
}

_X_EXPORT int
Xasprintf(char **ret, const char *format, ...)
{
    return 0;
}

_X_EXPORT int
XISetDevicePropertyDeletable(DeviceIntPtr dev, Atom property, Bool deletable)
{
    return 0;
}

_X_EXPORT InputInfoPtr
xf86FirstLocalDevice(void)
{
    return NULL;
}

_X_EXPORT void
xf86DeleteInput(InputInfoPtr pInp, int flags)
{
    return;
}

_X_EXPORT XF86OptionPtr
xf86OptionListDuplicate(XF86OptionPtr options)
{
    return NULL;
}

_X_EXPORT Bool
InitButtonClassDeviceStruct(DeviceIntPtr dev, int numButtons, Atom *labels,
                            CARD8 *map)
{
    return FALSE;
}

_X_EXPORT void
InitValuatorAxisStruct(DeviceIntPtr dev, int axnum, Atom label, int minval,
                       int maxval, int resolution, int min_res, int max_res,
                       int mode)
{
    return;
}

_X_EXPORT void
xf86PostKeyboardEvent(DeviceIntPtr device, unsigned int key_code, int is_down)
{
    return;
}

_X_EXPORT int
xf86SetIntOption(XF86OptionPtr optlist, const char *name, int deflt)
{
    return 0;
}

_X_EXPORT void
xf86PostButtonEventP(DeviceIntPtr device,
                     int is_absolute,
                     int button,
                     int is_down, int first_valuator, int num_valuators,
                     const int *valuators)
{
    return;
}

_X_EXPORT Bool
InitPtrFeedbackClassDeviceStruct(DeviceIntPtr dev, PtrCtrlProcPtr controlProc)
{
    return FALSE;
}

_X_EXPORT int
XIChangeDeviceProperty(DeviceIntPtr dev, Atom property, Atom type,
                       int format, int mode, unsigned long len,
                       XF86OptionPtr value, Bool sendevent)
{
    return 0;
}

_X_EXPORT CARD32
GetTimeInMillis(void)
{
    return 0;
}

_X_EXPORT int
NewInputDeviceRequest(InputOption *options,
                      InputAttributes * attrs,
                      DeviceIntPtr *pdev)
{
    return 0;
}

_X_EXPORT Bool
InitLedFeedbackClassDeviceStruct(DeviceIntPtr dev, LedCtrlProcPtr controlProc)
{
    return FALSE;
}

_X_EXPORT InputAttributes *
DuplicateInputAttributes(InputAttributes * attrs)
{
    return NULL;
}

_X_EXPORT int
ValidAtom(Atom atom)
{
    return None;
}

_X_EXPORT Bool
InitKeyboardDeviceStruct(DeviceIntPtr dev, XkbRMLVOSet * rmlvo,
                         BellProcPtr bell_func, KbdCtrlProcPtr ctrl_func)
{
    return FALSE;
}

_X_EXPORT long
XIRegisterPropertyHandler(DeviceIntPtr dev,
                          int (*SetProperty) (DeviceIntPtr dev,
                                              Atom property,
                                              XIPropertyValuePtr prop,
                                              BOOL checkonly),
                          int (*GetProperty) (DeviceIntPtr dev,
                                              Atom property),
                          int (*DeleteProperty) (DeviceIntPtr dev,
                                                 Atom property))
{
    return 0;
}

_X_EXPORT int
InitProximityClassDeviceStruct(DeviceIntPtr dev)
{
    return 0;
}

_X_EXPORT void
xf86Msg(MessageType type, const char *format, ...)
{
    return;
}

_X_EXPORT void
xf86MsgVerb(MessageType type, int verb, const char *format, ...)
{
    return;
}

_X_EXPORT void
xf86IDrvMsg(InputInfoPtr dev, MessageType type, const char *format, ...)
{
    return;
}

_X_EXPORT void
LogMessageVerbSigSafe(MessageType type, int verb, const char *format, ...)
{
    return;
}

_X_EXPORT void
xf86PostMotionEventP(DeviceIntPtr device,
                     int is_absolute, int first_valuator, int num_valuators,
                     const int *valuators)
{
    return;
}

_X_EXPORT Bool
InitValuatorClassDeviceStruct(DeviceIntPtr dev, int numAxes, Atom *labels,
                              int numMotionEvents, int mode)
{
    return FALSE;
}

_X_EXPORT XF86OptionPtr
xf86ReplaceStrOption(XF86OptionPtr optlist, const char *name, const char *val)
{
    return NULL;
}

_X_EXPORT XF86OptionPtr
xf86NextOption(XF86OptionPtr list)
{
    return NULL;
}

_X_EXPORT int
XIGetDeviceProperty(DeviceIntPtr dev, Atom property, XIPropertyValuePtr *value)
{
    return 0;
}

_X_EXPORT Atom
MakeAtom(const char *string, unsigned len, Bool makeit)
{
    return None;
}

_X_EXPORT int
GetMotionHistorySize(void)
{
    return 0;
}

_X_EXPORT void
xf86PostProximityEventP(DeviceIntPtr device,
                        int is_in, int first_valuator, int num_valuators,
                        const int *valuators)
{
    return;
}

_X_EXPORT Bool
InitFocusClassDeviceStruct(DeviceIntPtr dev)
{
    return FALSE;
}

void
xf86ProcessCommonOptions(InputInfoPtr pInfo, XF86OptionPtr list)
{
}

void
xf86CollectInputOptions(InputInfoPtr pInfo,
                        const char **defaultOpts)
{
}

InputInfoPtr
xf86AllocateInput(void)
{
    return NULL;
}

ClientPtr serverClient;

Bool
QueueWorkProc(Bool (*function)
              (ClientPtr /* pClient */ , pointer /* closure */ ),
              ClientPtr client, pointer closure)
{
    return FALSE;
}

_X_EXPORT ValuatorMask *
valuator_mask_new(int num_valuators)
{
    return NULL;
}

_X_EXPORT void
valuator_mask_free(ValuatorMask **mask)
{
}

_X_EXPORT int
valuator_mask_get(const ValuatorMask *mask, int valuator)
{
    return 0;
}

_X_EXPORT void
valuator_mask_set(ValuatorMask *mask, int valuator, int data)
{
}

extern _X_EXPORT void
valuator_mask_unset(ValuatorMask *mask, int bit)
{
}

_X_EXPORT int
valuator_mask_num_valuators(const ValuatorMask *mask)
{
    return 0;
}

_X_EXPORT void
valuator_mask_zero(ValuatorMask *mask)
{
}

_X_EXPORT void
valuator_mask_copy(ValuatorMask *dest, const ValuatorMask *src)
{
}

_X_EXPORT void
xf86PostTouchEvent(DeviceIntPtr dev, uint32_t touchid,
                   uint16_t type, uint32_t flags, const ValuatorMask *mask)
{
}
