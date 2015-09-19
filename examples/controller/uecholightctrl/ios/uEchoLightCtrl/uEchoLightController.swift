/******************************************************************
*
* uEcho for Swift
*
* Copyright (C) The uecho Authors 2015
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

import uEcho

public class uEchoLightController : uEchoController {
  
  override init() {
    super.init()
  }

  deinit {
  }
  
  var objects : [uEchoObject] {
    var objs = [uEchoObject]()
    for node in nodes {
      for obj in node.objects {
        if (obj.classGroupCode == 0x02) && (obj.classCode == 0x91) {
          objs.append(obj)
        }
      }
    }
    return objs
  }
}
