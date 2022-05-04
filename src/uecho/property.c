/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/_node.h>
#include <uecho/_property.h>

#include <uecho/misc.h>

/****************************************
* uecho_property_new
****************************************/

uEchoProperty* uecho_property_new(void)
{
  uEchoProperty* prop;

  prop = (uEchoProperty*)malloc(sizeof(uEchoProperty));

  if (!prop)
    return NULL;

  uecho_list_node_init((uEchoList*)prop);

  prop->name = uecho_string_new();
  prop->data = NULL;
  prop->dataSize = 0;

  uecho_property_setparentobject(prop, NULL);
  uecho_property_setattribute(prop, uEchoPropertyAttrReadWrite);

  if (!prop->name) {
    uecho_property_delete(prop);
    return NULL;
  }

  return prop;
}

/****************************************
* uecho_property_delete
****************************************/

bool uecho_property_delete(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if (prop->name) {
    uecho_string_delete(prop->name);
  }

  uecho_property_cleardata(prop);
  uecho_property_remove(prop);

  free(prop);

  return true;
}

/****************************************
 * uecho_property_setparentobject
 ****************************************/

bool uecho_property_setparentobject(uEchoProperty* prop, uEchoObject* obj)
{
  if (!prop)
    return false;

  prop->parent_obj = obj;

  return true;
}

/****************************************
 * uecho_property_getparentobject
 ****************************************/

uEchoObject* uecho_property_getparentobject(uEchoProperty* prop)
{
  if (!prop)
    return NULL;

  return prop->parent_obj;
}

/****************************************
 * uecho_property_setcount
 ****************************************/

bool uecho_property_setcount(uEchoProperty* prop, size_t count)
{
  uecho_property_cleardata(prop);

  if (!prop)
    return false;

  if (count == 0)
    return true;

  prop->data = (byte*)calloc(1, count);
  if (!prop->data)
    return false;

  prop->dataSize = count;

  return true;
}

/****************************************
 * uecho_property_addcount
 ****************************************/

bool uecho_property_addcount(uEchoProperty* prop, size_t count)
{
  size_t new_data_size;

  if (!prop)
    return false;

  if (count == 0)
    return true;

  new_data_size = prop->dataSize + count;
  prop->data = (byte*)realloc(prop->data, new_data_size);
  if (!prop->data)
    return false;

  prop->dataSize = new_data_size;

  return true;
}

/****************************************
 * uecho_property_setdata
 ****************************************/

bool uecho_property_setdata(uEchoProperty* prop, const byte* data, size_t count)
{
  if (!prop)
    return false;

  if (!uecho_property_setcount(prop, count))
    return false;

  if (count == 0)
    return true;

  memcpy(prop->data, data, count);

  // (D) Basic sequence for autonomous notification

  if (uecho_property_isannounceable(prop)) {
    uecho_property_announce(prop);
  }

  return true;
}

/****************************************
 * uecho_property_adddata
 ****************************************/

bool uecho_property_adddata(uEchoProperty* prop, const byte* data, size_t count)
{
  size_t curr_data_size;

  if (!prop)
    return false;

  if (count == 0)
    return true;

  curr_data_size = uecho_property_getdatasize(prop);

  if (!uecho_property_addcount(prop, count))
    return false;

  memcpy((prop->data + curr_data_size), data, count);

  // (D) Basic sequence for autonomous notification

  if (uecho_property_isannounceable(prop)) {
    uecho_property_announce(prop);
  }

  return true;
}

/****************************************
 * uecho_property_addbytedata
 ****************************************/

bool uecho_property_addbytedata(uEchoProperty* prop, const byte data)
{
  return uecho_property_adddata(prop, &data, 1);
}

/****************************************
 * uecho_property_setintegerdata
 ****************************************/

bool uecho_property_setintegerdata(uEchoProperty* prop, int data, size_t data_size)
{
  bool is_success;
  byte* int_byte;

  if (!prop)
    return false;

  int_byte = (byte*)malloc(data_size);
  if (!int_byte)
    return true;

  uecho_integer2byte(data, int_byte, data_size);

  is_success = uecho_property_setdata(prop, int_byte, data_size);

  free(int_byte);

  return is_success;
}

/****************************************
 * uecho_property_getintegerdata
 ****************************************/

bool uecho_property_getintegerdata(uEchoProperty* prop, size_t data_size, int* data)
{
  if (!prop)
    return false;

  if (prop->dataSize != data_size)
    return false;

  *data = uecho_byte2integer(prop->data, data_size);

  return true;
}

/****************************************
 * uecho_property_setbytedata
 ****************************************/

bool uecho_property_setbytedata(uEchoProperty* prop, byte data)
{
  if (!prop)
    return false;

  return uecho_property_setdata(prop, &data, 1);
}

/****************************************
 * uecho_property_getbytedata
 ****************************************/

bool uecho_property_getbytedata(uEchoProperty* prop, byte* data)
{
  if (!prop)
    return false;

  if (prop->dataSize != 1)
    return false;

  *data = prop->data[0];

  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

bool uecho_property_cleardata(uEchoProperty* prop)
{
  if (!prop)
    return false;

  prop->dataSize = 0;

  if (prop->data) {
    free(prop->data);
    prop->data = NULL;
  }

  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

uEchoProperty* uecho_property_next(uEchoProperty* prop)
{
  return (uEchoProperty*)uecho_list_next((uEchoList*)prop);
}

/****************************************
 * uecho_property_remove
 ****************************************/

void uecho_property_remove(uEchoProperty* prop)
{
  uecho_list_remove((uEchoList*)prop);
}

/****************************************
 * uecho_property_setname
 ****************************************/

void uecho_property_setname(uEchoProperty* prop, const char *name)
{
  uecho_string_setvalue(prop->name, name);
}

/****************************************
 * uecho_property_getname
 ****************************************/

const char *uecho_property_getname(uEchoProperty* prop)
{
  return uecho_string_getvalue(prop->name);
}

/****************************************
 * uecho_property_setcode
 ****************************************/

void uecho_property_setcode(uEchoProperty* prop, uEchoPropertyCode val)
{
  prop->code = val;
}

/****************************************
 * uecho_property_getcode
 ****************************************/

uEchoPropertyCode uecho_property_getcode(uEchoProperty* prop)
{
  return prop->code;
}

/****************************************
 * uecho_property_getdata
 ****************************************/

byte* uecho_property_getdata(uEchoProperty* prop)
{
  return prop->data;
}

/****************************************
 * uecho_property_getdatasize
 ****************************************/

byte uecho_property_getdatasize(uEchoProperty* prop)
{
  return prop->dataSize;
}

/****************************************
 * uecho_property_setattribute
 ****************************************/

void uecho_property_setattribute(uEchoProperty* prop, uEchoPropertyAttr val)
{
  prop->attr = val;
}

/****************************************
 * uecho_property_getattribute
 ****************************************/

uEchoPropertyAttr uecho_property_getattribute(uEchoProperty* prop)
{
  return prop->attr;
}

/****************************************
 * uecho_property_isreadable
 ****************************************/

bool uecho_property_isreadable(uEchoProperty* prop)
{
  return (prop->attr & (uEchoPropertyAttrRead | uEchoPropertyAttrReadRequired)) ? true : false;
}

/****************************************
 * uecho_property_isreadrequired
 ****************************************/

bool uecho_property_isreadrequired(uEchoProperty* prop)
{
  return (prop->attr & uEchoPropertyAttrReadRequired) ? true : false;
}

/****************************************
 * uecho_property_iswritable
 ****************************************/

bool uecho_property_iswritable(uEchoProperty* prop)
{
  return (prop->attr & (uEchoPropertyAttrWrite | uEchoPropertyAttrWriteRequired)) ? true : false;
}

/****************************************
 * uecho_property_iswriterequired
 ****************************************/

bool uecho_property_iswriterequired(uEchoProperty* prop)
{
  return (prop->attr & uEchoPropertyAttrWriteRequired) ? true : false;
}

/****************************************
 * uecho_property_isreadonly
 ****************************************/

bool uecho_property_isreadonly(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if (!(prop->attr & (uEchoPropertyAttrRead | uEchoPropertyAttrReadRequired)))
    return false;
  if ((prop->attr & uEchoPropertyAttrWrite))
    return false;

  return true;
}

/****************************************
 * uecho_property_iswriteonly
 ****************************************/

bool uecho_property_iswriteonly(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if (!(prop->attr & (uEchoPropertyAttrWrite | uEchoPropertyAttrWriteRequired)))
    return false;
  if ((prop->attr & uEchoPropertyAttrRead))
    return false;

  return true;
}

/****************************************
 * uecho_property_isannounceable
 ****************************************/

bool uecho_property_isannounceable(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if ((prop->attr & uEchoPropertyAttrAnno))
    return true;

  return false;
}

/****************************************
 * uecho_property_copy
 ****************************************/

uEchoProperty* uecho_property_copy(uEchoProperty* src_prop)
{
  uEchoProperty* new_prop;

  if (!src_prop)
    return NULL;

  new_prop = uecho_property_new();

  if (!new_prop)
    return NULL;

  uecho_property_setcode(new_prop, uecho_property_getcode(src_prop));
  uecho_property_setattribute(new_prop, uecho_property_getattribute(src_prop));
  uecho_property_setdata(new_prop, uecho_property_getdata(src_prop), uecho_property_getdatasize(src_prop));

  return new_prop;
}

/****************************************
 * uecho_property_equals
 ****************************************/

bool uecho_property_equals(uEchoProperty* prop1, uEchoProperty* prop2)
{
  if (!prop1 || !prop2)
    return false;

  if (uecho_property_getcode(prop1) != uecho_property_getcode(prop2))
    return false;

  if (uecho_property_getattribute(prop1) != uecho_property_getattribute(prop2))
    return false;

  if (uecho_property_getdatasize(prop1) != uecho_property_getdatasize(prop2))
    return false;

  if (memcmp(uecho_property_getdata(prop1), uecho_property_getdata(prop2), uecho_property_getdatasize(prop1)) != 0)
    return false;

  return true;
}

/****************************************
 * uecho_property_getnode
 ****************************************/

uEchoNode* uecho_property_getnode(uEchoProperty* prop)
{
  uEchoObject* parent_obj;
  uEchoNode* parent_node;

  parent_obj = uecho_property_getparentobject(prop);
  if (!parent_obj)
    return NULL;

  parent_node = uecho_object_getparentnode(parent_obj);
  if (!parent_node)
    return NULL;

  return parent_node;
}

/****************************************
 * uecho_property_announce
 ****************************************/

bool uecho_property_announce(uEchoProperty* prop)
{
  uEchoNode* node;
  uEchoObject* obj;

  node = uecho_property_getnode(prop);
  if (!node)
    return false;

  obj = uecho_property_getparentobject(prop);
  if (!obj)
    return false;

  return uecho_node_announceproperty(node, obj, prop);
}
