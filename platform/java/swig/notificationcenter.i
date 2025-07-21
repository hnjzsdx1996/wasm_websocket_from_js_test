/* notificationcenter.i */

%module(directors="1") notificationcenter
%javaconst(1);

// Enable C++11 features and directors for cross-language polymorphism
%feature("c++11");
%feature("director");

%{
#include "SDKManager.h"
#include "BusinessManager.h"
#include "WebsocketEvent.h"
%}

// Standard library support
%include "std_string.i"
%include "std_shared_ptr.i"

// Instantiate templates for each class that will be held by a shared_ptr
%shared_ptr(SDKManager);
%shared_ptr(BusinessManager);
%shared_ptr(WebsocketEvent);

// Rename C++ entities for better Java API
%rename(ConnectionListener) WebsocketEvent;
%rename(destroy) ~SDKManager;
%rename(destroy) ~BusinessManager;

// Ignore methods we don't want to expose
%ignore SDKManager::getTopicManager;
%ignore SDKManager::getWebSocketHolder;

// Process the actual header files
%include "SDKManager.h"
%include "BusinessManager.h"
%include "WebsocketEvent.h" 