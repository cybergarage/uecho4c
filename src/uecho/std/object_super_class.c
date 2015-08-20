/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/profile.h>

/****************************************
 * uecho_object_superclass_addmandatoryproperties
 ****************************************/

bool uecho_object_superclass_addmandatoryproperties(uEchoObject *obj)
{
  byte zeroPropMap[] = {0};
  
  if (!obj)
    return false;
  
  // Property map properties
  
  if (!uecho_object_addproperty(obj, uEchoObjectSuperClassGetPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  if (!uecho_object_addproperty(obj, uEchoObjectSuperClassSetPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  if (!uecho_object_addproperty(obj, uEchoObjectSuperClassAnnoPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  
  return true;
}

/****************************************
 * uecho_object_superclass_updatepropertymaps
 ****************************************/

bool uecho_object_superclass_updatepropertymaps(uEchoObject *obj)
{
  uEchoProperty *prop;
  
  uecho_object_superclass_clearpropertymapcaches(obj);
  
  // Update property map caches
  
  for (prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
    // Get property map
    if (uecho_property_isreadable(prop)) {
      obj->getPropMapSize++;
      obj->getPropMapBytes = realloc(obj->getPropMapBytes, obj->getPropMapSize);
      if (obj->getPropMapBytes) {
        obj->getPropMapBytes[obj->getPropMapSize-1] = uecho_property_getcode(prop);
      }
    }
    
    // Set property map
    if (uecho_property_iswritable(prop)) {
      obj->setPropMapSize++;
      obj->setPropMapBytes = realloc(obj->setPropMapBytes, obj->setPropMapSize);
      if (obj->setPropMapBytes) {
        obj->setPropMapBytes[obj->setPropMapSize-1] = uecho_property_getcode(prop);
      }
    }
    
    // Announcement status changes property map
    if (uecho_property_isannouncement(prop)) {
      obj->annoPropMapSize++;
      obj->annoPropMapBytes = realloc(obj->annoPropMapBytes, obj->annoPropMapSize);
      if (obj->annoPropMapBytes) {
        obj->annoPropMapBytes[obj->annoPropMapSize-1] = uecho_property_getcode(prop);
      }
    }
  }
  
  // Update property map properties
  
  if (!uecho_object_updateproperty(obj, uEchoProfileSuperClassGetPropertyMap, uEchoPropertyAttrRead, obj->getPropMapBytes, obj->getPropMapSize))
    return false;
  if (!uecho_object_updateproperty(obj, uEchoProfileSuperClassSetPropertyMap, uEchoPropertyAttrRead, obj->setPropMapBytes, obj->setPropMapSize))
    return false;
  if (!uecho_object_updateproperty(obj, uEchoProfileSuperClassAnnoPropertyMap, uEchoPropertyAttrRead, obj->annoPropMapBytes, obj->annoPropMapSize))
    return false;
  
  return true;
}

/****************************************
 * uecho_object_superclass_clearpropertymapcaches
 ****************************************/

void uecho_object_superclass_clearpropertymapcaches(uEchoObject *obj)
{
  if (obj->annoPropMapBytes) {
    free(obj->annoPropMapBytes);
    obj->annoPropMapBytes = NULL;
  }
  obj->annoPropMapSize = 0;
  
  if (obj->setPropMapBytes) {
    free(obj->setPropMapBytes);
    obj->setPropMapBytes = NULL;
  }
  obj->setPropMapSize = 0;
  
  if (obj->getPropMapBytes) {
    free(obj->getPropMapBytes);
    obj->getPropMapBytes = NULL;
  }
  obj->getPropMapSize = 0;
}

