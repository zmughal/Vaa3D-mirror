#ifndef __CallTest_H__
#define __CallTest_H__

#include "V3DITKPluginDefaultHeader.h"

class CallTestPlugin : public QObject, public V3DPluginInterface
{
  Q_OBJECT
  Q_INTERFACES(V3DPluginInterface)
  V3DITKPLUGIN_DEFAULT_CLASS_DECLARATION_BODY(CallTest);
};

#endif